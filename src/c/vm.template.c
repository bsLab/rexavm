#include "phash.h"
#include "vmcode.h"
int vmloop(unsigned char* code, int pc) {
  static void* dispatch_table[] = { 
    &&do_UNTIL,
    &&do_FORGET,
    &&do_LOOP,
    &&do_OR,
    &&do_LEAVE,
    &&do_MAX,
    &&do_CGT,
    &&do_REPEAT,
    &&do_ZEROCLT,
    &&do_R,
    &&do_RECEIVE,
    &&do_AND,
    &&do_CGTR,
    &&do_CQUO,
    &&do_I,
    &&do_MIN,
    &&do_RCGT,
    &&do_OVER,
    &&do_CMIN,
    &&do_CEXC,
    &&do_VAR,
    &&do_CSEM,
    &&do_END,
    &&do_CEQ,
    &&do_MINUS,
    &&do_ARRAY,
    &&do_LOAD,
    &&do_SEND,
    &&do_CALL,
    &&do_DUP,
    &&do_SAVE,
    &&do_CONST,
    &&do_CDOT,
    &&do_SWAP,
    &&do_ALLOT,
    &&do_WHILE,
    &&do_CMUL,
    &&do_DROP,
    &&do_ABS,
    &&do_DO,
    &&do_READ,
    &&do_WRITE,
    &&do_CLT,
    &&do_CAT,
    &&do_IF,
    &&do_MOD,
    &&do_BEGIN,
    &&do_ROT,
    &&do_CR,
    &&do_CPLU,
    &&do_ELSE,
    &&do_XOR,
    &&do_CMINDUP,
    &&do_ZEROCEQ,
    &&do_ENDIF,
    &&do_CCOL,
    &&do_NOP,
    &&do_NOP,
    &&do_NOP,
    &&do_NOP,
    &&do_NOP,
    &&do_NOP,
    &&do_NOP,
    &&do_NOP,
    &&do_NOP,
    &&do_NOP,
    &&do_NOP,
    &&do_TWOOR,
    &&do_NOP,
    &&do_TWOMAX,
    &&do_TWOCGT,
    &&do_NOP,
    &&do_NOP,
    &&do_TWOR,
    &&do_NOP,
    &&do_TWOAND,
    &&do_TWOCGTR,
    &&do_TWOCQUO,
    &&do_NOP,
    &&do_TWOMIN,
    &&do_TWORCGT,
    &&do_TWOOVER,
    &&do_TWOCMIN,
    &&do_TWOCEXC,
    &&do_NOP,
    &&do_NOP,
    &&do_NOP,
    &&do_TWOCEQ,
    &&do_TWOMINUS,
    &&do_NOP,
    &&do_NOP,
    &&do_NOP,
    &&do_NOP,
    &&do_TWODUP,
    &&do_NOP,
    &&do_NOP,
    &&do_TWOCDOT,
    &&do_TWOSWAP,
    &&do_NOP,
    &&do_NOP,
    &&do_TWOCMUL,
    &&do_TWODROP,
    &&do_TWOABS,
    &&do_NOP,
    &&do_NOP,
    &&do_NOP,
    &&do_TWOCLT,
    &&do_TWOCAT,
    &&do_NOP,
    &&do_TWOMOD,
    &&do_NOP,
    &&do_TWOROT,
    &&do_NOP,
    &&do_TWOCPLU,
    &&do_NOP,
    &&do_TWOXOR,
    &&do_NOP,
    &&do_NOP,
    &&do_NOP,
    &&do_NOP,
    &&do_NOP,
    &&do_NOP,
    &&do_NOP,
    &&do_NOP,
    &&do_NOP,
    &&do_NOP,
    &&do_LIT,
    &&do_TWOLIT
  };
  DISPATCH();
  while(1) {
    do_UNTIL: DO_UNTIL;
    do_FORGET: DO_FORGET;
    do_LOOP: DO_LOOP;
    do_OR: DO_OR;
    do_LEAVE: DO_LEAVE;
    do_MAX: DO_MAX;
    do_CGT: DO_CGT;
    do_REPEAT: DO_REPEAT;
    do_ZEROCLT: DO_ZEROCLT;
    do_R: DO_R;
    do_RECEIVE: DO_RECEIVE;
    do_AND: DO_AND;
    do_CGTR: DO_CGTR;
    do_CQUO: DO_CQUO;
    do_I: DO_I;
    do_MIN: DO_MIN;
    do_RCGT: DO_RCGT;
    do_OVER: DO_OVER;
    do_CMIN: DO_CMIN;
    do_CEXC: DO_CEXC;
    do_VAR: DO_VAR;
    do_CSEM: DO_CSEM;
    do_END: DO_END;
    do_CEQ: DO_CEQ;
    do_MINUS: DO_MINUS;
    do_ARRAY: DO_ARRAY;
    do_LOAD: DO_LOAD;
    do_SEND: DO_SEND;
    do_CALL: DO_CALL;
    do_DUP: DO_DUP;
    do_SAVE: DO_SAVE;
    do_CONST: DO_CONST;
    do_CDOT: DO_CDOT;
    do_SWAP: DO_SWAP;
    do_ALLOT: DO_ALLOT;
    do_WHILE: DO_WHILE;
    do_CMUL: DO_CMUL;
    do_DROP: DO_DROP;
    do_ABS: DO_ABS;
    do_DO: DO_DO;
    do_READ: DO_READ;
    do_WRITE: DO_WRITE;
    do_CLT: DO_CLT;
    do_CAT: DO_CAT;
    do_IF: DO_IF;
    do_MOD: DO_MOD;
    do_BEGIN: DO_BEGIN;
    do_ROT: DO_ROT;
    do_CR: DO_CR;
    do_CPLU: DO_CPLU;
    do_ELSE: DO_ELSE;
    do_XOR: DO_XOR;
    do_CMINDUP: DO_CMINDUP;
    do_ZEROCEQ: DO_ZEROCEQ;
    do_ENDIF: DO_ENDIF;
    do_CCOL: DO_CCOL;
    do_TWOOR: DO_TWOOR;
    do_TWOMAX: DO_TWOMAX;
    do_TWOCGT: DO_TWOCGT;
    do_TWOR: DO_TWOR;
    do_TWOAND: DO_TWOAND;
    do_TWOCGTR: DO_TWOCGTR;
    do_TWOCQUO: DO_TWOCQUO;
    do_TWOMIN: DO_TWOMIN;
    do_TWORCGT: DO_TWORCGT;
    do_TWOOVER: DO_TWOOVER;
    do_TWOCMIN: DO_TWOCMIN;
    do_TWOCEXC: DO_TWOCEXC;
    do_TWOCEQ: DO_TWOCEQ;
    do_TWOMINUS: DO_TWOMINUS;
    do_TWODUP: DO_TWODUP;
    do_TWOCDOT: DO_TWOCDOT;
    do_TWOSWAP: DO_TWOSWAP;
    do_TWOCMUL: DO_TWOCMUL;
    do_TWODROP: DO_TWODROP;
    do_TWOABS: DO_TWOABS;
    do_TWOCLT: DO_TWOCLT;
    do_TWOCAT: DO_TWOCAT;
    do_TWOMOD: DO_TWOMOD;
    do_TWOROT: DO_TWOROT;
    do_TWOCPLU: DO_TWOCPLU;
    do_TWOXOR: DO_TWOXOR;
    do_LIT: DO_LIT;
    do_TWOLIT: DO_TWOLIT;
    do_NOP: DISPATCH();
  };
  return pc;
};
