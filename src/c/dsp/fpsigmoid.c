#include "config.h"
#include "standard.h"
#include "vm.h"
#include "vmio.h"
#include "fplog10.h"
#include "fpsigmoid.h"


static ub1 sglut13[] = { 0,
  9,
  19,
  28,
  37,
  54,
  63,
  71,
  78,
  93,
  101,
  114,
  120,
  133,
  144,
  149,
  159,
  169,
  177,
  189,
  199,
  206,
  211,
  219 
};
static ub1 sglut310[] = { 0, 8, 30, 42, 46, 47 };

// y scale 1:1000 [0,1], x scale 1:1000
sb2 fpsigmoid(sb2 x) {
  sb2 y;
  ub1 mirror=x<0?1:0;
  if (mirror) x=-x;
  if (x>=10000) return mirror?0:1000;
  if (x<=1000) {
    y = 500+(((x*231)/1000));
    return mirror?1000-y:y;
  } else if (x<3000) {
    ub2 i10 = ((fplog10((x/5)|0)/2))-65;
    y = ((sb2)sglut13[i10])+731;
    return mirror?1000-y:y;
  } else {
    ub2 i10 = ((fplog10((x/10)|0)/10))-14;
    y = ((sb2)sglut310[i10])+952;
    return mirror?1000-y:y;
  }
  return 0;
}


