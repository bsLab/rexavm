#define _FPVEC_H

extern void vecload (sb2 src, sb2 srcoff, sb2 dst);
extern void vecscale(sb2 src, sb2 dst, sb2 k);
extern void vecfold (sb2 src, sb2 weights, sb2 dst, sb2 k);
extern void vecconv(sb2 src, sb2 weights, sb2 dst, sb2 k, sb2 swidth, sb2 kwidth, sb2 stride, sb2 pad);
extern void vecmap  (sb2 src, sb2 dst, sb2 func, sb2 k);
extern void vecadd  (sb2 op1, sb2 op2, sb2 dst, sb2 k);
extern void vecmul  (sb2 op1, sb2 op2, sb2 dst, sb2 k);
extern sb4  vecreduce(sb2 vec, sb2 off, sb2 len, sb2 op);
extern void vecreshape (sb2 src, sb2 dst, sb2 k);
extern void vecprint(sb2 vec);

#define OPMIN 1 // 256
#define OPMAX 2 // 512
#define OPAVG 4 // 1024
#define OPSUM 8 // 2048

#define FPVECINIT   fiosAdd("vecload",IOSCALLBACK(vecload),3,2,0);\
  fiosAdd("vecscale",IOSCALLBACK(vecscale),3,2,0);\
  fiosAdd("vecfold",IOSCALLBACK(vecfold),4,2,0);\
  fiosAdd("vecconv",IOSCALLBACK(vecconv),8,2,0);\
  fiosAdd("vecmap",IOSCALLBACK(vecmap),4,2,0);\
  fiosAdd("vecadd",IOSCALLBACK(vecadd),4,2,0);\
  fiosAdd("vecmul",IOSCALLBACK(vecmul),4,2,0);\
  fiosAdd("vecreduce",IOSCALLBACK(vecreduce),4,2,4);\
  fiosAdd("vecreshape",IOSCALLBACK(vecreshape),3,2,0);\
  fiosAdd("vecprint",IOSCALLBACK(vecprint),1,2,0);\


