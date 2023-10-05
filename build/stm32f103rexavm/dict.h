#ifndef __DICT_H
#define __DICT_H
#include "standard.h"

#ifndef DICTNAMELENGTH
#define DICTNAMELENGTH  8
#endif
#define ALLOCATE  0x01
#define ARRAY     0x02
#define INDIRECT  0x04
#define FUNCTION  0x08
#define NOTFOUND  0xFFFF

struct dict_cell {
   char name[DICTNAMELENGTH];
   uw1  addr;   // CS
   uw1  size;
   ub1  flags;
   sb1  next;
};

typedef struct dict_cell dict_cell_t;
// Main word dictionary
extern dict_cell_t dictionary[];
extern ub2 dictcollision;
extern ub2 dicthash(char *name, ub2 length, ub2 tablesize);
extern ub2 dictlookup(dict_cell_t *dictionary, ub2 dictsize, char *name, ub2 length);
extern ub2 dictadd(dict_cell_t *dictionary, ub2 dictsize, char *name, ub2 length);
extern ub2 dictrem(dict_cell_t *dictionary, ub2 dictsize, char *name, ub2 length);
extern void dictclean(dict_cell_t *dictionary, ub2 dictsize, ub2 start, ub2 end);
extern void dictinit(dict_cell_t *dictionary, ub2 dictsize);
#endif



