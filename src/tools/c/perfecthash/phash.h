/* Perfect hash definitions */
#ifndef STANDARD
#include "standard.h"
#endif /* STANDARD */
#ifndef PHASH
#define PHASH

extern ub1 tab[];
#define PHASHLEN 0x10  /* length of hash mapping table */
#define PHASHNKEYS 20  /* How many keys were hashed */
#define PHASHRANGE 32  /* Range any input might map to */
#define PHASHSALT 0xdaa66d2b /* internal, initialize normal hash */

ub4 phash();

#endif  /* PHASH */

