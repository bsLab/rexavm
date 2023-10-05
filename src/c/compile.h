#ifndef __COMPILE_H
#define __COMPILE_H
extern sw1 compileToken(ub1  *code,sw1 position, uw1 *pc);

#if COMPILEINCR==1
EXTERN sw1 compile(ub1 *code,sw1 position, uw1 *pc, sb2 steps);
#else
EXTERN sw1 compile(ub1 *code,sw1 position, uw1 *pc);
#endif
// Upper boundary of locked CS range containing words in global dictionary
extern sw1 vmcslocked;
#endif
