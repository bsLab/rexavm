#ifndef _FPSIN_H
#define _FPSIN_H
// x-scale: 1:1000 (0-6283 <=> 0-2PI), y-scale:1:1000 (-1000/+1000 <=> -1/+1)
extern sb2 fpsin(sb2 i);

#define FPSININIT fiosAdd("sin",IOSCALLBACK(fpsin),1,2,2)

#endif

