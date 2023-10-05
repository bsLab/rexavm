#ifndef __STACK_H
#define __STACK_H
#include "standard.h"
extern sw1 DS[];  // data stack 
extern sw1 RS[];  // return stack
extern sw1 FS[];  // loop stack
extern uw1 dstop; // data stack segment pointer
extern uw1 rstop; // data stack segment pointer
extern uw1 fstop; // loop stack segment pointer

// Long number (double word) storage depends on CPU endianess: Little Endian. LSW MSW -- or Bif Endian. MSW LSW --
#define POP()         DS[(dstop--)-1]
#define RPOP()        RS[(rstop--)-1]
#define FPOP()        FS[(fstop--)-1]
#define PUSH(v)       DS[dstop++]=v;
#define RPUSH(v)      RS[rstop++]=v;
#define FPUSH(v)      FS[fstop++]=v;
#define STACKTOP(n)   DS[dstop-n]
#define FSTACKTOP(n)  FS[fstop-n]
#define RSTACKTOP(n)  RS[dstop-n]
#define STACKCHECK(n)   if (n>dstop) { vmerror=ESTACK; goto onerror; }
#define FSTACKCHECK(n)  if (n>fstop) { vmerror=ESTACK; goto onerror; }
#define RSTACKCHECK(n)  if (n>rstop) { vmerror=ERSTACK; goto onerror; }
#define DEBUGDS() {int i;for(i=0;i<dstop;i++) printf(" %x ",DS[i]); printf("\n"); }

#define POP2()        (*(sw2*)(&DS[dstop-=2]))
#define POP2U()       (*(uw2*)(&DS[dstop-=2]))
#define RPOP2()       (*(sw2*)(&RS[rstop-=2]))
#define FPOP2()       (*(sw2*)(&FS[fstop-=2]))
#define PUSH2(v)      *(sw2*)(&DS[dstop])=v; dstop+=2;
#define RPUSH2(v)     *(sw2*)(&RS[rstop])=v; rstop+=2;
#define FPUSH2(v)     *(sw2*)(&FS[fstop])=v; fstop+=2;
#define FPUSH2U(v)    *(uw2*)(&FS[fstop])=(ub4)v; fstop+=2;
#define STACKTOP2(n)  (*(sw2*)(&DS[dstop-n-1]))
#define FSTACKTOP2(n) (*(sw2*)(&FS[fstop-n-1])) 
#define RSTACKTOP2(n) (*(sw2*)(&RS[rstop-n-1]))

#endif


