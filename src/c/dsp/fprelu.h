#ifndef _FPRELU_H
#define _FPRELU_H
// x-scale: 1:1000 (0-6283 <=> 0-2PI), y-scale:1:1000 (-1000/+1000 <=> -1/+1)
extern sb2 fprelu(sb2 i);

#define FPRELUINIT fiosAdd("relu",IOSCALLBACK(fprelu),1,2,2)

#endif
