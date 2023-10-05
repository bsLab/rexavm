#ifndef __VMLOOP_H
#define __VMLOOP_H
#include "vmcode.h"
#define DECODE(word) (word&0x80?word&0x7F:word&0x40?OP_TWOLIT:OP_LIT)
#if VMLOOP==COMPUTEDGOTO
  #define DISPATCH() PROFILER DEBUG("pc=%x ds=%d rs=%d fs=%d op=%x id=%d\n",pc,dstop,rstop,fstop,code[pc],DECODE(code[pc])); if ((--steps)==0) goto schedule; goto *dispatch_table[DECODE(code[pc])]
#endif
#if VMLOOP==SWITCHCASE
  #define DISPATCH() PROFILER DEBUG("pc=%x ds=%d rs=%d fs=%d op=%x id=%d\n",pc,dstop,rstop,fstop,code[pc],DECODE(code[pc])); if ((--steps)==0) goto schedule; break
#endif
#if VMLOOP==SWITCHCASEGOTO
  #define DISPATCH() PROFILER DEBUG("pc=%x ds=%d rs=%d fs=%d op=%x id=%d\n",pc,dstop,rstop,fstop,code[pc],DECODE(code[pc])); if ((--steps)==0) goto schedule; goto dispatch
#endif
#if VMLOOP==FUNCTABLE
  #define DISPATCH() PROFILER DEBUG("pc=%x ds=%d rs=%d fs=%d op=%x id=%d\n",pc,dstop,rstop,fstop,code[pc],DECODE(code[pc])); if ((--steps)==0) goto schedule;
#endif
#define EXTENDSIGN(v) (v&0x20?v|0xC0:v)

// Main VM loop executing code.
// Code: CS or any other buffer, pc: CS offset address, steps: maximal number of steps
// Returns current (next) pc value; if a negative value is returned, then the current op is suspended (e.g., sleep)
// The vmevent.timeout fields > 0 is the next time when the vmloop may be reentered.
// If (ub4) vmevent.v is not null, the vmloop may be reentered only if *v==vmevent.c is satisfied
// if an error occured, vmerror contains the error code
EXTERN sb2 vmloop(ub1* code, ub2 pc, ub2 steps);
#endif

