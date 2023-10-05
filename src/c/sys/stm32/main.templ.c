#include "config.h"
#include "standard.h"
#include "compile.h"
#include "dict.h"
#include "stack.h"
#include "vm.h"
#include "vmloop.h"
#include "vmio.h"
#include "vmwords.h"

#if DSP==1
#include "fpsin.h"
#include "fplog10.h"
#include "fpsigmoid.h"
#include "fpvec.h"
#endif

// use CONSOLEFMT, CONSOLELINE, or PRINT macros for output 
#pragma IGNORE

static int execute(char *text) {
  sb2 pos;
  uw1 cstop0=cstop;
  memcpy(&CS[cstop0],text,strlen(text));
  cstop=strlen(text);
  CS[cstop]=0;
  cstop++;
  // printf("%s [%d]\n",path,cstop);
  pos=pctop=(sb2)cstop0;
#if COMPILEINCR==1
  pos=compile(&CS[cstop0],pos,(uw1*)&pctop,0);
#else
  pos=compile(&CS[cstop0],pos,(uw1*)&pctop);
#endif
  if (vmerror!=EOK) {
    PRINT("Compilation failed with error %d at character position %d\n",vmerror,-pos);
    return -1;
  }
  // printf("Code size: %d Byte(s)\n",pctop);    
  pctop=0;
  vmstate=0;
  while (vmstate!=VMEND && !vmerror) pctop=vmloop(CS,(uw1)pctop,1000);
  PRINT("END pc=%x state=%x ds=%d rs=%d fs=%d err=%d\n",pctop,vmstate,dstop,rstop,fstop,vmerror);
  return vmerror;
}

/*
int main() {
   execute("1 2 . cr end");
}

*/
