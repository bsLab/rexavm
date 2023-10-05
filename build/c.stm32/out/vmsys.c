/*
VM system call API

Programming and interaction with VM only via vmsys wrapper API
*/
#include "config.h"
#include "standard.h"
#include "vmsys.h"


// Wrapper for system millisecond clock returning current (ub4) millisecods
ub4 vmMilli(void){
  return 0;
}



