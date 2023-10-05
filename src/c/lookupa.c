/*
--------------------------------------------------------------------
--------------------------------------------------------------------
*/
#include "standard.h"
#include "lookupa.h"
// Calculates a distinct hash function for a given string. Each value of the
// integer d results in a different hash value.
static ub4 hash(sb4 d, char *str, ub1 length) {
  ub1 i;
  if(d == 0) { d = 0x811c9dc5; }
  for(i = 0; i < length; i++) {
    // http://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
    // http://isthe.com/chongo/src/fnv/hash_32.c
    // multiply by the 32 bit FNV magic prime mod 2^32
    d += (d << 1) + (d << 4) + (d << 7) + (d << 8) + (d << 24);
    // xor the bottom with the current octet
    d ^= str[i];
  }
  return d & 0x7fffffff;
};

ub1 lookupPH (char *key, ub1 length, sb1* G, ub1 Glength, ub1* V, ub1 Vlength) {
  sb4 d = G[ hash(0, key, length) % Glength ];
  if (d < 0) return V[ 0-d-1 ];
  return V[hash(d, key, length) % Vlength];
};
