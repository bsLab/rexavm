
#include "standard.h"
#include "config.h"
#include "vmio.h"

// linear table; can be replaced with phash
ub1 iostop=0;
ios_t ios[64];

void iosInit() {
  memset((void *)&ios[0],0,sizeof(ios_t)*64);
  iostop=0;
}
// (name,FIOSCALLBACK(function),01234,0124,0124)
ub1 fiosAdd(char *name, void (*callback)(), ub1 args, ub1 argsize, ub1 retsize) {
  ios_t *io=&ios[iostop];
  ub1 length=strlen(name);
  if (length>DICTNAMELENGTH) length=DICTNAMELENGTH;
  strncpy(io->name,name,DICTNAMELENGTH);
  io->type='F';
  io->callback=(void *)callback;
  io->args=args;
  io->argsize=argsize;
  io->retsize=retsize;
  iostop++;
  return iostop-1;
}



// (name,IOSDATA(data),01..,124)
ub1 diosAdd(char *name, void *data, ub2 cells, ub1 size) {
  ios_t *io=&ios[iostop];
  ub1 length=strlen(name);
  if (length>DICTNAMELENGTH) length=DICTNAMELENGTH;
  strncpy(io->name,name,DICTNAMELENGTH);
  io->type='D';
  io->data=(void *)data;
  io->cells=cells;
  io->size=size;
  iostop++;
  return iostop-1;
}




