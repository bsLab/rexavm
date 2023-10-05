#ifndef __VMIO_H
#define __VMIO_H

#define DICTNAMELENGTH  8

struct ios {
  char name[DICTNAMELENGTH+1];
  union {
    void *callback;
    void *data;
  };
  char  type;
  union {
    ub2  cells;
    ub1  args;
  };
  union {
    ub1  size;
    ub1  argsize;
  };
  ub1 retsize;
};
typedef struct ios ios_t;

#define IOSCALLBACK(f) (void (*)())&f
#define IOSCALLBACKPTR(p) (void (*)())p
#define IOSDATA(d) (void *)d

extern ub1 iostop;
extern ios_t ios[];
EXTERN void iosInit(void);
EXTERN ub1 fiosAdd(char *name, void (*callback)(), ub1 args, ub1 argsize, ub1 retsize);
EXTERN ub1 diosAdd(char *name, void *data, ub2 cells, ub1 size);

#define FIOSCALL0(index,...) (*(void (*)())ios[index].callback)(__VA_ARGS__)
#define FIOSCALL1(index,...) (*(ub1 (*)())ios[index].callback)(__VA_ARGS__)
#define FIOSCALL2(index,...) (*(ub2 (*)())ios[index].callback)(__VA_ARGS__)
#define FIOSCALL4(index,...) (*(ub4 (*)())ios[index].callback)(__VA_ARGS__)
#define DIOSARRAYINDEX(type,index,offset)  (((type* )ios[index].data)[offset])
#define DIOSDATADEREF(type,index)          (*((type* )ios[index].data))
#define DIOSDATADPTR(type,index)           ((type* )ios[index].data)
#endif



