
#include "config.h"
#include "standard.h"
#include "vm.h"
#include "stack.h"

#if MULTITHREADED==1
// Data Stack
sw1 DS[STACKSIZE*MAXTHREADS];
// Return Stack
sw1 RS[RSTACKSIZE*MAXTHREADS];
// Loop stack
sw1 FS[FSTACKSIZE*MAXTHREADS];
#else
// Data Stack
sw1 DS[STACKSIZE];
// Return Stack
sw1 RS[RSTACKSIZE];
// Loop stack
sw1 FS[FSTACKSIZE];
#endif
uw1 dstop=0,rstop=0,fstop=0;

