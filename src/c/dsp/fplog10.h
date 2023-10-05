#ifndef _FPLOG10_H
#define _FPLOG10H_

// x-scale is 1:10 and log10-scale is 1:100
extern sb2 fplog10(sb2 x);

#define FPLOG10INIT fiosAdd("log",IOSCALLBACK(fplog10),1,2,2);

#endif

