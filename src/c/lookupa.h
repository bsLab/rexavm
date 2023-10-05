/*
------------------------------------------------------------------------------
By Bob Jenkins, September 1996.
lookupa.h, a hash function for table lookup, same function as lookup.c.
Use this code in any way you wish.  Public Domain.  It has no warranty.
Source is http://burtleburtle.net/bob/c/lookupa.h
------------------------------------------------------------------------------
*/
#ifndef _H_LOOKUPA
#define _H_LOOKUPA


#ifndef STANDARD
#include "standard.h"
#endif

extern ub1 lookupPH (char *key, ub1 length, sb1* G, ub1 Glength, ub1* V, ub1 Vlength);

#endif /* H_LOOKUPA */
