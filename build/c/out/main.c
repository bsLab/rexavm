#include "config.h"
#include "standard.h"
#include "compile.h"
#include "dict.h"
#include "stack.h"
#include "vm.h"
#include "vmloop.h"
#include "vmio.h"
#include "vmsys.h"
#include "vmwords.h"
#include "diss.h"

#include <stdio.h>

#include "fprelu.h"
#include "fpsin.h"
#include "fplog10.h"
#include "fpsigmoid.h"
#include "fpvec.h"

#include "readline.h"

char *VMERRORNAME[] = {
  "EOK",
  "ENOTFOUND",
  "ESTACK",
  "ERSTACK",
  "EPC",
  "ESYNTAX",
  "ESYNTAXNUM",
  "ESYNTAXENV",
  "EINDEX",
  "EFCALL",
  "EIO",
  "ENOTIMPLEMENTED",
  "EDATA",
  "EOVERFLOW",
  "ENOSPACE"
};

ub2 myData2[100];
ub1 myData1[100];
ub2 myCounter=0;
ub2 mySample[8192];

ub1 test=0,
    profile=0,
    printasm=0,
    verbose=0;

static void printErrorPos(char *path,int pos,int before,int after) {
  FILE * file;
  int i=0,
      nread,
      line=0;
  char buf[2];
  file = fopen( path , "r");  
  if (file) {
    while ((nread = fread(buf, 1, 1, file)) > 0) {
      i+=nread;
      if (buf[0]=='\n') line++;
      if (i>=(pos-before) && i<=(pos+after)) {
        if (i>pos && buf[0]=='\n') break;
        printf("%c",buf[0]);
      }
    }
    fclose(file);
    printf(" ( in line %d )\n",line);
  }
} 

static int execute(char *path) {
  FILE * file;
  sw1 nread,pos;
  uw1 cstop0=cstop;
  file = fopen( path , "r");  
  if (file) {
    while ((nread = fread(&CS[cstop], 1, 1024, file)) > 0) {
      cstop+=(uw1)nread;
    }
    CS[cstop]=0;
    cstop++;
    fclose(file);
    printf("%s [%d]\n",path,cstop);
    pos=pctop=(sw1)cstop0;
    pos=compile(CS,pos,(uw1*)&pctop);
    if (vmerror!=EOK) {
      printf("Compilation failed with error %d (%s) at character position %d\n",vmerror,VMERRORNAME[vmerror],-pos);
      printErrorPos(path,-pos,5,5);
      return -1;
    }
    printf("Code size: %d Byte(s)\n",pctop);    
    pctop=0;
    vmstate=0;
    while (vmstate!=VMEND && !vmerror) pctop=vmloop(CS,(uw1)pctop,1000);
    printf("END pc=%x state=%x ds=%d rs=%d fs=%d err=%d\n",pctop,vmstate,dstop,rstop,fstop,vmerror);
    if (vmerror)  printf("Exeuction failed with error %d (%s).\n",vmerror,VMERRORNAME[vmerror]);
    return vmerror;
  } else {
    printf("Error opening file %s\n",path);
    return -1;
  }
}

static int diss(char *path) {
  FILE * file;
  int nread;
  sw1 pos;
  cstop=0;
  file = fopen( path , "r");  
  if (file) {
    while ((nread = fread(&CS[cstop], 1, 1024, file)) > 0) {
      cstop+=(uw1)nread;
    }
    printf("%s [%d]\n",path,cstop);
    pos=0;
    pctop=0;
    pos=compile(CS,pos,(uw1*)&pctop);
    if (vmerror!=EOK) {
      printf("Compilation failed with error %d (%s) at character position %d\n",vmerror,VMERRORNAME[vmerror],-pos);
      printErrorPos(path,-pos,5,5);
      return -1;
    }
    printf("Code size %d Bytes\n",pctop);
    return disassemble(0,(ub2)(pctop-1));
  } else {
    printf("Error opening file %s\n",path);
    return -1;
  }
}

static int profileVM(char *path) {
  FILE * file;
  int i,
      runs=1000000,
      nread,pos,off=0,
      words=diss(path);
  char *buf=malloc(10000);
  cstop=0;
  file = fopen( path , "r");
  ub4 start = vmMilli();
  if (file) {
    while ((nread = fread(&buf[off], 1, 1024, file)) > 0) {
      off+=nread;
    }
    for(i=0;i<runs;i++) {
      pos=0;
      pctop=0;
      cstop=(uw1)off;
      memcpy(CS,buf,off+1);
      pos=compile(CS,pos,(uw1*)&pctop);
      if (vmerror) return -1;      
    }
  }
  ub4 stop = vmMilli();
  printf("%d nanosec/compiled word\n",((stop-start)*1000)/(runs/1000*words));
  return 0;
}

// ( -- a b ) 
static sb4 myRandom2() {
  sb2 a=-2000,b=-100;
  return (sb4)(((ub2)a<<16)+(ub2)b);
}
// ( -- a b ) 
static sb4 myRandomDouble() {
  sb4 a=-123456;
  return a;
}


int main(int argc, char *argv[]) {
  int disass=0,i;
  int status=0;
  char *file=NULL;
  cstop=0;
  pctop=0;
  
  iosInit();
  fiosAdd("milli",IOSCALLBACK(vmMilli),0,0,4);
  fiosAdd("random2",IOSCALLBACK(myRandom2),0,0,4);
  fiosAdd("2random",IOSCALLBACK(myRandomDouble),0,0,4);
  // 1 Math
  FPRELUINIT;
  FPSININIT;
  FPLOG10INIT;
  FPSIGMOIDINIT;
  FPVECINIT;
  
  for(i=0;i<8192;i++) mySample[i]=i;
  diosAdd("data2",IOSDATA(myData2),100,2);
  diosAdd("data1",IOSDATA(myData1),100,1);
  diosAdd("sample",IOSDATA(mySample),8192,2);
  diosAdd("counter",IOSDATA(&myCounter),0,2);
  for(i=1;i<argc;i++) {
    if (strcmp(argv[i],"-t")==0) test=1; 
    if (strcmp(argv[i],"-v")==0) verbose++; 
    if (strcmp(argv[i],"-p")==0) profile=1;
    if (strcmp(argv[i],"-d")==0) printasm=1;
    if (argv[i][0]!='-') file=argv[i];
  }
  if (file!=NULL) {
    if (printasm) 
      status = diss(file);
    else if (profile)
      status = profileVM(file);
    else
      status = execute(file); 
  } 
  else {
    char *line="";
    uw1 cstop0=cstop,len;
    sw1 pos;
    while (line) {
      line=readline("> ");
      if (line) {
        add_history(line);
        len=strlen(line);
        pos=pctop=(sw1)cstop0;
        memcpy(&CS[cstop0],line,len+1);
        cstop=cstop0+len;
        // printf("<%s>\n",&CS[cstop0]);
        // printf("cstop0:%d\n",cstop0);
        pos=compile(CS,pos,(uw1*)&pctop);
        if (vmerror!=EOK) {
          printf("Compilation failed with error %d (%s) at character position %d\n",vmerror,VMERRORNAME[vmerror],cstop0-pos);
          continue;
        }
        vmstate=0;
        if (verbose) disassemble(0,(ub2)(pctop-1));
        pctop=(sw1)cstop0;
        // Global word definition added?
        if (vmcslocked>0) cstop0=(uw1)vmcslocked;
        while (vmstate!=VMEND && !vmerror) pctop=vmloop(CS,(uw1)pctop,1000);
        if (verbose) printf("END pc=%x state=%x ds=%d rs=%d fs=%d err=%d\n",pctop,vmstate,dstop,rstop,fstop,vmerror);
        if (vmerror)  printf("Exeuction failed with error %d (%s).\n",vmerror,VMERRORNAME[vmerror]);
      }
    }
    printf("\nExit.\n");
  }
  if (verbose) {
    printf("VM Dictionary Collisions: %d\n",dictcollision);
  }
  return status;
}

