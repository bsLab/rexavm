// Builds the VM loop
#include "corehash.h"
#include "lookupa.h"
#include "corewords.h"
#include <string.h>

// -L -C -CG -F

void makeLabel(char *word,char *wordlabel) {
  int i,j,h = corehash(word,strlen(word));
  j=0;
  for(i=0;i<strlen(word);i++) {
    if (word[i]>='a'&&word[i]<='z') { wordlabel[j]=word[i]-32; j++; }
    else switch (word[i]) {
      case '0' : strcpy(&wordlabel[j],"ZERO");  j+=4; break;
      case '1' : strcpy(&wordlabel[j],"ONE");   j+=3; break;
      case '2' : strcpy(&wordlabel[j],"TWO");   j+=3; break;
      case '3' : strcpy(&wordlabel[j],"THREE"); j+=5; break;
      case '4' : strcpy(&wordlabel[j],"FOUR");  j+=4; break;
      case '5' : strcpy(&wordlabel[j],"FIVE");  j+=4; break;
      case '6' : strcpy(&wordlabel[j],"SIX");   j+=3; break;
      case '7' : strcpy(&wordlabel[j],"SEVEN"); j+=5; break;
      case '8' : strcpy(&wordlabel[j],"EIGHT"); j+=5; break;
      case '9' : strcpy(&wordlabel[j],"NINE");  j+=4; break;
      case '=' : strcpy(&wordlabel[j],"CEQ");    j+=3; break;
      case '<' : strcpy(&wordlabel[j],"CLT");    j+=3; break;
      case '>' : strcpy(&wordlabel[j],"CGT");    j+=3; break;
      case '*' : strcpy(&wordlabel[j],"CMUL");   j+=4; break;
      case '+' : strcpy(&wordlabel[j],"CPLU");   j+=4; break;
      case '-' : strcpy(&wordlabel[j],"CMIN");   j+=4; break;
      case '/' : strcpy(&wordlabel[j],"CQUO");   j+=4; break;
      case '!' : strcpy(&wordlabel[j],"STORE");  j+=5; break;
      case ',' : strcpy(&wordlabel[j],"CCOM");   j+=4; break;
      case ';' : strcpy(&wordlabel[j],"DEFRET"); j+=6; break;
      case '.' : strcpy(&wordlabel[j],"CDOT");   j+=4; break;
      case '@' : strcpy(&wordlabel[j],"FETCH");    j+=5; break;
      case ':' : strcpy(&wordlabel[j],"DEFINE");   j+=6; break;
      case '"' : strcpy(&wordlabel[j],"CSTR");   j+=4; break;
      case '$' : strcpy(&wordlabel[j],"CDOLL");   j+=5; break;
    };
  }
  wordlabel[j]=0;
}

int main(int argc, char *argv[]) {
  int i,j,top=0,index=0;
  char *word;
  char *wordLabel;
  char *lut[128];
  char buf[32];
  char sep=' ';
  char mode='C';
  
  if (argc>1 && strcmp(argv[1],"-C")==0)  mode='C';
  if (argc>1 && strcmp(argv[1],"-CG")==0) mode='G';
  if (argc>1 && strcmp(argv[1],"-L")==0)  mode='L';
  if (argc>1 && strcmp(argv[1],"-F")==0)  mode='F';
  
  index=0;
  word=words[index];
  memset(lut,0,128*sizeof(int));
  while (word) {
    wordLabel=malloc(32);
    int h = corehash(word,strlen(word));
    makeLabel(word,wordLabel);
    lut[h]=wordLabel;
    top=h>top?h:top;
    index++;
    word=words[index];
  }

  lut[index++]="CALL";
  lut[index++]="FCALL";
  lut[index++]="BRANCH";
  lut[index++]="INDIRECT";
  lut[index++]="NOP";
  lut[index++]="LIT";
  lut[index++]="TWOLIT";
  
  printf("// Automatically created on %s\n",__DATE__);
  // printf("#include \"phash.h\"\n");
  printf("#include \"config.h\"\n");
  printf("#include \"standard.h\"\n");
  printf("#include \"vm.h\"\n");
  printf("#include \"stack.h\"\n");
  printf("#include \"compile.h\"\n");
  printf("#include \"vmcode.h\"\n");
  printf("#include \"vmio.h\"\n");
  printf("#include \"vmimpl.h\"\n");
  printf("#include \"fpmath.h\"\n");
  printf("#include \"vmloop.h\"\n");
  if (mode=='F') {
    printf("static ub1 *code; static ub2 addr,pc,steps; static sb2 a,b,c,d; static sb4 da,db,dc,dd;\n");   
    for (i=0;i<128;i++) {
      if (lut[i]!=NULL) printf("static sb8 do_%s() { DO_%s; return 0; end: onerror: return -1;};\n",lut[i],lut[i]);
      else              break;
    }
    printf("ub2 vmloop(ub1 *_code, ub2 _pc, ub2 _steps) {\n");
    printf("  code=_code;pc=_pc;steps=_steps;\n");
    printf("  void* dispatch_table[] = {");
    sep=' ';
    for (i=0;i<126;i++) {
      if (lut[i]!=NULL) printf("%c\n    (void *)&do_%s /*%d*/",sep,lut[i],i);
      else              break;
      sep=',';
    }
    printf("\n  };\n");
  } else {
    printf("sb2 vmloop(ub1 *code, ub2 pc, ub2 steps) {\n");
    printf("  ub2 addr; sb2 a,b,c,d; sb4 da,db,dc,dd;\n");   
  }
  printf("  vmstate|=VMRUN;\n");
  switch (mode) {
    case 'F':
      printf("  sb1 status=0;\n");
      printf("  while(status==0) status=(*(ub1 (*)())dispatch_table[DECODE(code[pc])])();\n");
      break;
    case 'L':
      printf("  void* dispatch_table[] = {");
      sep=' ';
      for (i=0;i<126;i++) {
        if (lut[i]!=NULL) printf("%c\n    &&do_%s /*%d*/",sep,lut[i],i);
        else              break;
        sep=',';
      }
      printf("\n  };\n");
      printf("  DISPATCH();\n");
      // printf("  while(1) { \n");
      for (i=0;i<128;i++) {
        if (lut[i]!=NULL) printf("    do_%s: DO_%s;\n",lut[i],lut[i]);
        else              break;
      }
      // printf("  };\n");
      break;
    case 'C':
      printf("  while(1) switch(DECODE(code[pc])) {\n");
      for (i=0;i<128;i++) {
        if (lut[i]!=NULL) printf("    case %d: DO_%s;\n",i,lut[i]);
        else              break;
      }
      printf("    default: pc++;\n",i);
      printf("  };\n");      
      break;
    case 'G':
      printf("  dispatch: switch(DECODE(code[pc])) {\n");
      for (i=0;i<128;i++) {
        if (lut[i]!=NULL) printf("    case %d: goto do_%s;\n",i,lut[i]);
        else              break;
      }
      printf("  default: pc++;DISPATCH();\n");
      printf("  };\n");      
      // printf("  while(1) { \n");
      for (i=0;i<128;i++) {
        if (lut[i]!=NULL) printf("    do_%s: DO_%s;\n",lut[i],lut[i]);
        else              break;
      }  
      // printf("  };\n");
      break;
  }
  if (mode=='F') printf("  return pc;\n");
  else {
    printf("  onerror:  vmstate=VMERROR;   return pc;\n");
    printf("  suspend:  vmstate=VMSUSPEND; return -pc;\n");
    printf("  schedule: return pc;\n");
    printf("  end:      vmstate=VMEND;     return pc;\n");
  }
  printf("};\n");
  return 0;
}
