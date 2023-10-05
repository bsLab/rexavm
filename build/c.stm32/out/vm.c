#include "config.h"
#include "standard.h"
#include "stack.h"
#include "vm.h"
#include "vmio.h"
#include "vmloop.h"
#include "vmsys.h"


// Default: frame-less incremental mode (current actice code always on the top)
ub1     CS[5000];
uw1     cstop=0;


sw1     pctop=0;
ub1     vmerror=EOK;
ub1     vmstate=VMIDLE;  // vm state
ub1     vmbase=0;
// ub4     vmsteps=0;

  vmevent_t vmevent;

void    *fcalls[8];
void    *io[8];


void vmraiseerror(ub1 error) {
  vmerror=error;
};

void vmreset() {
    vmevent.timeout=0;
    vmevent.v=NULL;
  
  dstop=0;
  rstop=0;
  fstop=0;
}

// Main vmloop execution handler
// Return 0: normal exit, <0: error code, >0: timeout in milli seconds
sb2 vmrun (ub2 steps) {
  ub4 now=vmMilli();
  if (vmevent.timeout && now >= vmevent.timeout) {
    pctop=-pctop;
    vmevent.timeout=0;
  }
  if (vmevent.v && *vmevent.v == vmevent.c) {
    pctop=-pctop;
    vmevent.v=NULL;
  }
  if (pctop>=0) pctop = vmloop(CS,(uw1)pctop,steps);
  if (pctop<0) { 
    /* suspended */
    now=vmMilli();
    if (vmevent.timeout) return vmevent.timeout-now;
  }
  return vmerror?-vmerror:0;
}



