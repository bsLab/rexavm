#define _FPSIGMOID_H

 // y scale 1:1000 [0,1], x scale 1:1000
extern sb2 fpsigmoid(sb2 x);

#define FPSIGMOIDINIT   fiosAdd("sigmoid",IOSCALLBACK(fpsigmoid),1,2,2)



