#ifndef __VM_H
#define __VM_H
#include "standard.h"
#include "dict.h"

#define VMVERSION "0.32.1"

// Word Literals
#define BIGENDUB2(v)        (((v&0x3F)<<8)|((v>>8)&0xFF))
#define LITERAL(v)          (((v&0x3F)<<8)|((v>>8)&0xFF))
#define STORELIT(code,pc,v)  { code[pc]=(v>>8)&0x3F; code[pc+1]=v&0xFF; }
#define STORELIT2(code,pc,v) { code[pc]=0x40|((v>>24)&0x3F); code[pc+1]=(v>>16)&0xFF; code[pc+2]=(v>>8)&0xFF; code[pc+3]=v&0xFF;}
#define EXTEND(code,pc,mul,add)  { code[pc]=0x80|OP_MULADD; code[pc+1]=(mul<<4)|(add&0xf); }


// Code constants (following a command byte)
#define STOREUB2(code,pc,v)     *(ub2*)(&code[pc])=v;
#define STORESB2(code,pc,v)     *(sb2*)(&code[pc])=v;
#define STOREUB4(code,pc,v)     *(ub4*)(&code[pc])=v;
#define STORESB4(code,pc,v)     *(sb4*)(&code[pc])=v;

extern ub1 vmerror;
extern void vmraiseerror(ub1);

enum VMERRORS {
  EOK,
  ENOTFOUND,
  ESTACK,
  ERSTACK,
  EPC,
  ESYNTAX,
  ESYNTAXNUM,
  ESYNTAXENV,
  EINDEX,
  EFCALL,
  EIO,
  ENOTIMPLEMENTED,
  EDATA,
  EOVERFLOW,
  ENOSPACE
};
enum VMSTATES {
  VMIDLE  = 0x00,
  VMRUN   = 0x01,
  VMEND   = 0x10,
  VMERROR = 0x20,
  VMSUSPEND = 0x40,
};


// Event handler
struct vmevent_s {
  ub4 timeout;
  uw1 *v;
  uw1  c;
};
typedef struct vmevent_s vmevent_t;

  // only one "task"
  extern vmevent_t  vmevent;

// shared by all tasks, threads, and compiler (and comm. IO)
extern ub1  CS[];
extern uw1 cstop; // code sgement pointer
extern uw1 frtop; // frame table index pointer
extern sw1 pctop; // current PC
extern ub1 vmerror;  // error code
extern ub1 vmstate;  // vm state
extern ub1 vmbase;   // number base 0:d/1:h/2:b

 
EXTERN void vmreset(void);
// Return 0: normal exit, <0: error code, >0: timeout in milli seconds
EXTERN sw1 vmrun(ub2 steps);

#endif


