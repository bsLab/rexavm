/*
VM system call API

Programming and interaction with VM only via vmsys wrapper API
*/
#include "config.h"
#include "standard.h"
#include "vmsys.h"


// Wrapper for system millisecond clock returning current (ub4) millisecods
ub4 vmMilli(void){
  // ub4 vmMilli() { .. }
  long long milli;
  struct timeval tv;
  gettimeofday(&tv,NULL);
  milli=(((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
  return (ub4)milli;
}



