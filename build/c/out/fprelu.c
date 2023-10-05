#include "config.h"
#include "standard.h"
#include "vm.h"
#include "vmio.h"
#include "fprelu.h"

sb2 fprelu(sb2 i) {
  return i<0?0:i;
}

