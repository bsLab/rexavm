#include "config.h"
#include "standard.h"
#include "vm.h"
#include "vmio.h"
#include "fpsin.h"

// https://www.nullhardware.com/blog/fixed-point-sine-and-cosine-for-embedded-systems/
// x-scale: 1:1000 (0-6283 <=> 0-2PI), y-scale:1:1000 (-1000/+1000 <=> -1/+1)
sb2 fpsin(sb2 i)
{  
    // x-scale input 6282 -> 4*8192=32768 == 2PI 
    i=(sb2)(((sb4)i*5215l)/1000l);
    /* Convert (signed) input to a value between 0 and 8192. (8192 is pi/2, which is the region of the curve fit). */
    /* ------------------------------------------------------------------- */
    i <<= 1;
    ub1 c = i<0; //set carry for output pos/neg

    if(i == (i|0x4000)) // flip input value to corresponding value in range [0..8192)
        i = (1<<15) - i;
    i = (i & 0x7FFF) >> 1;
    /* ------------------------------------------------------------------- */

    /* The following section implements the formula:
     = y * 2^-n * ( A1 - 2^(q-p)* y * 2^-n * y * 2^-n * [B1 - 2^-r * y * 2^-n * C1 * y]) * 2^(a-q)
    Where the constants are defined as follows:
    */
    enum {A1=3370945099UL, B1=2746362156UL, C1=292421UL};
    enum {n=13, p=32, q=31, r=3, a=12};

    ub4 y = (C1*((ub4)i))>>n;
    y = B1 - (((ub4)i*y)>>r);
    y = (ub4)i * (y>>n);
    y = (ub4)i * (y>>n);
    y = A1 - (y>>(p-q));
    y = (ub4)i * (y>>n);
    y = (y+(1UL<<(q-a-1)))>>(q-a); // Rounding

    // y-scale -1/+1 -> -1000/+1000
    return c ? (sb2)(-(y*250)/1024) : (sb2)((y*250)/1024);
}

