#include "config.h"
#include "standard.h"
#include "compile.h"
#include "dict.h"
#include "dsp.h"
#include "stack.h"
#include "vm.h"
#include "vmloop.h"
#include "vmio.h"
#include "diss.h"

#include <stdio.h>


int test() {
  // 1. dictionary hash test
  printf("\n\n------------- Dictionary Test -------------------\n");
  char *identifier="hole";
  int h=dictadd(identifier,strlen(identifier));
  printf("%s %d %d %d\n",identifier,h,dictlookup(identifier,strlen(identifier)),dictcollision);
  identifier="measure";
  h=dictadd(identifier,strlen(identifier));
  printf("%s %d %d %d\n",identifier,h,dictlookup(identifier,strlen(identifier)),dictcollision);
  identifier="foo";
  h=dictadd(identifier,strlen(identifier));
  printf("%s %d %d %d\n",identifier,h,dictlookup(identifier,strlen(identifier)),dictcollision);
  identifier="foo2";
  h=dictadd(identifier,strlen(identifier));
  printf("%s %d %d %d\n",identifier,h,dictlookup(identifier,strlen(identifier)),dictcollision);
  identifier="oof2";
  h=dictadd(identifier,strlen(identifier));
  printf("%s %d %d %d\n",identifier,h,dictlookup(identifier,strlen(identifier)),dictcollision);


  printf("\n\n--------------- Toplevel Computations --------------------------\n");
  // 2. top level computations
  int i;
  ub1 test[256]="-100l 200l 2+ 2. cr yield end";
  sb2 pc=0;
  sb2 pos=0;
  printf("COMPILE %s\n",test);
  pos=compile(test,pos,&pc);
  if (vmerror!=EOK) return -1;
  printf("START Test code size=%d\n",pc);
  for (i=0;i<pc;i++) printf (" %x ",test[i]);
  printf("\n");
  pc=0;
  pc=vmloop(test,pc,65535);
  printf("END pc=%d ss=%d rs=%d fs=%d error=%d\n",pc,dstop,rstop,fstop,vmerror);
  
  // 3. word definition
  printf("\n\n--------------- Word Definition --------------------------\n");
  sprintf(test,": sq dup * ; 20 sq . cr end");  
  pos=0;
  pc=0;
  printf("COMPILE %s\n",test);
  pos=compile(test,pos,&pc);
  if (vmerror!=EOK) return -1;
  printf("START Test code size=%d\n",pc);
  for (i=0;i<pc;i++) printf (" %x ",test[i]);
  printf("\n");
  pc=0;
  pc=vmloop(test,pc,65535);
  printf("END pc=%d ss=%d rs=%d fs=%d error=%d\n",pc,dstop,rstop,fstop,vmerror);



  // 4. if else endif
  printf("\n\n--------------- if else endif --------------------------\n");
  sprintf(test,"1 if 111 . cr else 112 . cr endif 113 . cr end");  
  pos=0;
  pc=0;
  printf("COMPILE %s\n",test);
  pos=compile(test,pos,&pc);
  if (vmerror!=EOK) return -1;
  printf("START Test code size=%d\n",pc);
  for (i=0;i<pc;i++) printf (" %x ",test[i]);
  printf("\n");
  pc=0;
  pc=vmloop(test,pc,65535);
  printf("END pc=%d ss=%d rs=%d fs=%d error=%d\n",pc,dstop,rstop,fstop,vmerror);

  // 5. do loop
  printf("\n\n--------------- do loop  --------------------------\n");
  sprintf(test,"10 0 do i . cr loop 113 . cr end");  
  pos=0;
  pc=0;
  printf("COMPILE %s\n",test);
  pos=compile(test,pos,&pc);
  if (vmerror!=EOK) return -1;
  printf("START Test code size=%d\n",pc);
  for (i=0;i<pc;i++) printf (" %x ",test[i]);
  printf("\n");
  pc=0;
  pc=vmloop(test,pc,50);
  printf("END pc=%d ss=%d rs=%d fs=%d error=%d\n",pc,dstop,rstop,fstop,vmerror);

  printf("\n\n--------------- do +loop --------------------------\n");
  sprintf(test,"10 0 do i . cr 2 +loop 113 . cr end");  
  pos=0;
  pc=0;
  printf("COMPILE %s\n",test);
  pos=compile(test,pos,&pc);
  if (vmerror!=EOK) return -1;
  printf("START Test code size=%d\n",pc);
  for (i=0;i<pc;i++) printf (" %x ",test[i]);
  printf("\n");
  pc=0;
  pc=vmloop(test,pc,50);
  printf("END pc=%d ss=%d rs=%d fs=%d error=%d\n",pc,dstop,rstop,fstop,vmerror);

  printf("\n\n--------------- do do loop loop --------------------------\n");
  sprintf(test,"4 0 do 5 0 do i . j . cr loop loop 113 . cr end");  
  pos=0;
  pc=0;
  printf("COMPILE %s\n",test);
  pos=compile(test,pos,&pc);
  if (vmerror!=EOK) return -1;
  printf("START Test code size=%d\n",pc);
  for (i=0;i<pc;i++) printf (" %x ",test[i]);
  printf("\n");
  pc=0;
  pc=vmloop(test,pc,200);
  printf("END pc=%d ss=%d rs=%d fs=%d error=%d\n",pc,dstop,rstop,fstop,vmerror);


  printf("\n\n--------------- do leave loop --------------------------\n");
  sprintf(test,"10 0 do i . cr i 6 = if leave endif 2 +loop 113 . cr end");  
  pos=0;
  pc=0;
  printf("COMPILE %s\n",test);
  pos=compile(test,pos,&pc);
  if (vmerror!=EOK) return -1;
  printf("START Test code size=%d\n",pc);
  for (i=0;i<pc;i++) printf (" %x ",test[i]);
  printf("\n");
  pc=0;
  pc=vmloop(test,pc,50);
  printf("END pc=%d ss=%d rs=%d fs=%d error=%d\n",pc,dstop,rstop,fstop,vmerror);

  // 6. undefinite loop
  printf("\n\n--------------- begin leave again --------------------------\n");
  sprintf(test,"10 begin 1 - dup 0= if leave endif dup . cr again 113 . cr end");  
  pos=0;
  pc=0;
  printf("COMPILE %s\n",test);
  pos=compile(test,pos,&pc);
  if (vmerror!=EOK) return -1;
  printf("START Test code size=%d\n",pc);
  for (i=0;i<pc;i++) printf (" %x ",test[i]);
  printf("\n");
  pc=0;
  pc=vmloop(test,pc,90);
  printf("END pc=%d ss=%d rs=%d fs=%d error=%d\n",pc,dstop,rstop,fstop,vmerror);
  vmreset();

  printf("\n\n--------------- begin until --------------------------\n");
  sprintf(test,"10 begin dup . cr 1 - dup 5 = until 113 . cr end");  
  pos=0;
  pc=0;
  printf("COMPILE %s\n",test);
  pos=compile(test,pos,&pc);
  if (vmerror!=EOK) return -1;
  printf("START Test code size=%d\n",pc);
  for (i=0;i<pc;i++) printf (" %x ",test[i]);
  printf("\n");
  pc=0;
  pc=vmloop(test,pc,90);
  printf("END pc=%d ss=%d rs=%d fs=%d error=%d\n",pc,dstop,rstop,fstop,vmerror);
  vmreset();
  
  // 7. Variables
  printf("\n\n--------------- var --------------------------\n");
  sprintf(test,"var counter 1234 counter ! counter @ . cr end");  
  pos=0;
  pc=0;
  printf("COMPILE %s\n",test);
  pos=compile(test,pos,&pc);
  if (vmerror!=EOK) return -1;
  printf("START Test code size=%d\n",pc);
  for (i=0;i<pc;i++) printf (" %x ",test[i]);
  printf("\n");
  pc=0;
  pc=vmloop(test,pc,90);
  printf("END pc=%d ss=%d rs=%d fs=%d error=%d\n",pc,dstop,rstop,fstop,vmerror);

  printf("\n\n--------------- var begin until --------------------------\n");
  sprintf(test,"var counter 10 counter ! begin counter @ 1 - dup . cr dup counter ! 5 = until 113 . cr end");  
  pos=0;
  pc=0;
  printf("COMPILE %s\n",test);
  pos=compile(test,pos,&pc);
  if (vmerror!=EOK) return -1;
  printf("START Test code size=%d\n",pc);
  for (i=0;i<pc;i++) printf (" %x ",test[i]);
  printf("\n");
  pc=0;
  pc=vmloop(test,pc,90);
  printf("END pc=%d ss=%d rs=%d fs=%d error=%d\n",pc,dstop,rstop,fstop,vmerror);


  printf("\n\n--------------- literal double --------------------------\n");
  sprintf(test,"1234l 2. cr end");  
  pos=0;
  pc=0;
  printf("COMPILE %s\n",test);
  pos=compile(test,pos,&pc);
  if (vmerror!=EOK) return -1;
  printf("START Test code size=%d\n",pc);
  for (i=0;i<pc;i++) printf (" %x ",test[i]);
  printf("\n");
  pc=0;
  pc=vmloop(test,pc,90);
  printf("END pc=%d ss=%d rs=%d fs=%d error=%d\n",pc,dstop,rstop,fstop,vmerror);


  printf("\n\n--------------- 2var do loop --------------------------\n");
  sprintf(test,"2var counter 0l counter 2! milli 1000 30 * 0 do 100 0 do i j + drop counter 2@ 1l 2+ counter 2! loop loop milli 2swap 2- 2dup 2dup 2. .\"  ms  \" cr counter 2@ 2dup 2. .\"  loops  \" cr 2swap 2/ 1000l 2* 11l 2* 2dup 2. .\"  IPS  \" 1000000l 2/ 2. .\"  MIPS  \" cr end"); 
  pos=0;
  pc=0;
  printf("COMPILE %s\n",test);
  pos=compile(test,pos,&pc);
  if (vmerror!=EOK) return -1;
  printf("START Test code size=%d\n",pc);
  pos=pc;
  for (i=0;i<pc;i++) printf (" %x ",test[i]);
  printf(".\n");
  pc=0;
  pos--;
  vmreset();
  while (!vmerror && pc!=pos) { pc=vmloop(test,pc,50); };
  
  #ifdef PROFILING
    sb4 t=POP2();
    printf("END pc=%d ss=%d rs=%d fs=%d error=%d profileops=%d MIPS=%d\n",pc,dstop,rstop,fstop,vmerror,profileops,profileops/t*1000);
  #else
    printf("END pc=%d ss=%d rs=%d fs=%d error=%d\n",pc,dstop,rstop,fstop,vmerror);  
  #endif
  
  // 8. Console 
  printf("---------------  FIOS--------------------------\n");
  sprintf(test,"milli 2. .\"  MIPS \" cr end");  
  pos=0;
  pc=0;
  printf("COMPILE %s\n",test);
  pos=compile(test,pos,&pc);
  if (vmerror!=EOK) return -1;
  printf("START Test code size=%d\n",pc);
  for (i=0;i<pc;i++) printf (" %x ",test[i]);
  printf("\n");
  pc=0;
  pc=vmloop(test,pc,90);
  printf("END pc=%d ss=%d rs=%d fs=%d error=%d\n",pc,dstop,rstop,fstop,vmerror);

  // 9. DIOS read/write 
  printf("\n\n--------------- DIOS write --------------------------\n");
  sprintf(test,"10 0 do i 100 + i data2 write loop 10 0 do i 100 + i data1 write loop end");  
  pos=0;
  pc=0;
  printf("COMPILE %s\n",test);
  pos=compile(test,pos,&pc);
  if (vmerror!=EOK) return -1;
  printf("START Test code size=%d\n",pc);
  for (i=0;i<pc;i++) printf (" %x ",test[i]);
  printf("\n");
  pc=0;
  pc=vmloop(test,pc,500);
  printf("END pc=%d ss=%d rs=%d fs=%d error=%d\n",pc,dstop,rstop,fstop,vmerror);

  printf("\n\n--------------- DIOS read --------------------------\n");
  sprintf(test,"10 0 do i data2 read . cr loop 10 0 do i data1 read . cr loop end");  
  pos=0;
  pc=0;
  printf("COMPILE %s\n",test);
  pos=compile(test,pos,&pc);
  if (vmerror!=EOK) return -1;
  printf("START Test code size=%d\n",pc);
  for (i=0;i<pc;i++) printf (" %x ",test[i]);
  printf("\n");
  pc=0;
  pc=vmloop(test,pc,500);
  printf("END pc=%d ss=%d rs=%d fs=%d error=%d\n",pc,dstop,rstop,fstop,vmerror);
  
  printf("\n\n--------------- var DIOS read write --------------------------\n");
  sprintf(test,"100 counter write 10 0 do counter read 1 + counter write loop counter read . cr end");  
  pos=0;
  pc=0;
  printf("COMPILE %s\n",test);
  pos=compile(test,pos,&pc);
  if (vmerror!=EOK) return -1;
  printf("START Test code size=%d\n",pc);
  for (i=0;i<pc;i++) printf (" %x ",test[i]);
  printf("\n");
  pc=0;
  pc=vmloop(test,pc,500);
  printf("END pc=%d ss=%d rs=%d fs=%d error=%d\n",pc,dstop,rstop,fstop,vmerror);


  // simple data transfer with out op
  printf("\n\n--------------- out data transfer --------------------------\n");
  vmreset();
  sprintf(test,"1 base 100 90 do i . i out loop -100 . -100 out cr 4 0 do i data2 read out loop cr 0 base end");  
  pos=0;
  pc=0;
  printf("COMPILE %s\n",test);
  pos=compile(test,pos,&pc);
  if (vmerror!=EOK) return -1;
  printf("START Test code size=%d\n",pc);
  for (i=0;i<pc;i++) printf (" %x ",test[i]);
  printf("\n");
  pc=0;
  pc=vmloop(test,pc,500);
  printf("END pc=%d ss=%d rs=%d fs=%d error=%d\n",pc,dstop,rstop,fstop,vmerror);

#if 0
  sprintf(test,"3 0 do i 1000 * dup . cr sleep loop .\" Done. \" cr end");  
  pos=0;
  pc=0;
  printf("COMPILE %s\n",test);
  pos=compile(test,pos,&pc);
  if (vmerror!=EOK) return -1;
  printf("START Test code size=%d\n",pc);
  for (i=0;i<pc;i++) printf (" %x ",test[i]);
  printf("\n");
  pos=pc-1;
  pc=0;
  vmreset();
  while (!vmerror && pc!=pos) { 
    pc=vmloop(test,pc,50); 
    if (pc<0) { 
      /*suspend*/
      if (vmevent.timeout) while (vmevent.timeout>vmMilli()){};
      pc=-pc;vmevent.timeout=0;
    };
  };
  printf("END pc=%d ss=%d rs=%d fs=%d error=%d\n",pc,dstop,rstop,fstop,vmerror);
#endif
 
  // fpmath
  printf("\n\n--------------- fpmath sin log --------------------------\n");
  sprintf(test,"227 log . cr 1500 sin . cr 4700 sin . cr end");  
  pos=0;
  pc=0;
  printf("COMPILE %s\n",test);
  pos=compile(test,pos,&pc);
  if (vmerror!=EOK) return -1;
  printf("START Test code size=%d\n",pc);
  for (i=0;i<pc;i++) printf (" %x ",test[i]);
  printf("\n");
  pos=pc-1;
  pc=0;
  vmreset();
  while (!vmerror && pc!=pos) { 
    pc=vmloop(test,pc,50); 
    if (pc<0) { 
      /*suspend*/
      if (vmevent.timeout) while (vmevent.timeout>vmMilli()){};
      pc=-pc;vmevent.timeout=0;
    };
  };
  printf("END pc=%d ss=%d rs=%d fs=%d error=%d\n",pc,dstop,rstop,fstop,vmerror);

if (0) {
  // compiler benchmark
  printf("\n\n--------------- compiler benchmark --------------------------\n");
  int t0=vmMilli();
  for(i=0;i<1000000;i++) {  
    // 40 word tokens
    sprintf(test,"1 base var x var counter 0 counter ! 100 90 do i . i dup counter @ + counter ! out loop -100 . -100 out cr 4 0 do i data2 read out loop cr 0 base end");  
    pos=0;pc=0;
    compile(test,pos,&pc);
  }
  int t1=vmMilli();
  printf("END %2.2f MCPS\n",(float)40*1000000/(t1-t0)/1000);
}  
  
  // array
  printf("\n\n--------------- array len --------------------------\n");
  sprintf(test,"var x array data 5 data . cr 123 data ! data @ . cr 234 data 1 cell+ ! data 1 cell+ @ . cr end");  
  pos=0;
  pc=0;
  printf("COMPILE %s\n",test);
  pos=compile(test,pos,&pc);
  printf("START Test code size=%d\n",pc);
  for (i=0;i<pc;i++) printf (" %x ",test[i]);
  printf("\n");
  if (vmerror!=EOK) return -1;
  pc=0;
  pc=vmloop(test,pc,500);
  printf("END pc=%d ss=%d rs=%d fs=%d error=%d\n",pc,dstop,rstop,fstop,vmerror);

  printf("\n\n--------------- array {} --------------------------\n");
  sprintf(test,"2var counter var x array data { 1 2 3 4 5 } data . cr 123 data ! data @ . cr data 1 cell+ @ 234 + data 1 cell+ ! data 1 cell+ @ . cr end");  
  pos=0;
  pc=0;
  printf("COMPILE %s\n",test);
  pos=compile(test,pos,&pc);
  printf("START Test code size=%d\n",pc);
  for (i=0;i<pc;i++) printf (" %x ",test[i]);
  printf("\n");
  if (vmerror!=EOK) return -1;
  pc=0;
  pc=vmloop(test,pc,500);
  printf("END pc=%d ss=%d rs=%d fs=%d error=%d\n",pc,dstop,rstop,fstop,vmerror);
  
  return 0;
}

#include <sys/time.h>
static ub4 myMilli() {
  long long milli;
  struct timeval tv;
  gettimeofday(&tv,NULL);
  milli=(((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
  return (ub4)milli;
};
ub2 myData2[100];
ub1 myData1[100];
ub2 myCounter=0;
ub2 mySample[8192];

static int execute(char *path) {
  FILE * file;
  int nread,pos;
  cstop=0;
  file = fopen( path , "r");  
  if (file) {
    while ((nread = fread(&CS[cstop], 1, 1024, file)) > 0) {
      cstop+=nread;
    }
    printf("%s [%d]\n",path,cstop);
    pos=0;
    pctop=0;
    pos=compile(CS,pos,&pctop);
    if (vmerror!=EOK) {
      printf("Compilation failed with error %d at character position %d\n",vmerror,pos);
      return -1;
    }
    printf("Code size %d Bytes\n",pctop);    
    pctop=0;
    while (vmstate!=VMEND && !vmerror) pctop=vmloop(CS,pctop,1000);
    printf("END pc=%d vst=%x ds=%d rs=%d fs=%d err=%d\n",pctop,vmstate,dstop,rstop,fstop,vmerror);
    return vmerror;
  } else {
    printf("Error opening file %s\n",path);
    return -1;
  }
}

static int diss(char *path) {
  FILE * file;
  int nread,pos;
  cstop=0;
  file = fopen( path , "r");  
  if (file) {
    while ((nread = fread(&CS[cstop], 1, 1024, file)) > 0) {
      cstop+=nread;
    }
    printf("%s [%d]\n",path,cstop);
    pos=0;
    pctop=0;
    pos=compile(CS,pos,&pctop);
    if (vmerror!=EOK) {
      printf("Compilation failed with error %d at character position %d\n",vmerror,pos);
      return -1;
    }
    printf("Code size %d Bytes\n",pctop);
    disassemble(0,pctop-1);  
    return 0;
  } else {
    printf("Error opening file %s\n",path);
    return -1;
  }
}

int main(int argc, char *argv[]) {
  int disass=0,i;

  iosInit();
  fiosAdd("milli",IOSCALLBACK(myMilli),0,0,4);
  // DSP Math
  dspinit();
  for(i=0;i<8192;i++) mySample[i]=i;
  diosAdd("data2",IOSDATA(myData2),100,2);
  diosAdd("data1",IOSDATA(myData1),100,1);
  diosAdd("sample",IOSDATA(mySample),8192,2);
  diosAdd("counter",IOSDATA(&myCounter),0,2);
  if (argc>1) {
    char *file=argv[argc-1];
    if (strcmp(argv[1],"-d")==0) 
      return diss(file);
    else 
      return execute(file); 
  }
  return 0;
}
