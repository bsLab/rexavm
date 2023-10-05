
#include "config.h"
#include "standard.h"
#include "vm.h"
#include "stack.h"

// Data Stack
sw1 DS[256];
// Return Stack
sw1 RS[128];
// Loop stack
sw1 FS[128];
uw1 dstop=0,rstop=0,fstop=0;



