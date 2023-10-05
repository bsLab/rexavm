#ifndef __VM_CODEIMPL_H
#define __VM_CODEIMPL_H
#define DO_STORE { /* ! [1] */ \
  STACKCHECK(2);addr=(ub2)POP();a=POP();\
  *((sb2*)&code[addr])=a;\
  pc++;DISPATCH(); /* ! */\
  \
}
#define DO_CDOLL { /* $ [2] */ \
  if ((code[pc+1]&0x7f)==OP_CALL) { a=*(sb2*)(&code[pc+2]); pc+=4;}\
  else { a=*(sb1*)(&code[pc+2]); pc+=3; };\
  PUSH(a); DISPATCH(); /* $ */\
  \
}
#define DO_CMUL { /* * [3] */ \
  STACKCHECK(2);a=POP();b=POP();PUSH(a*b);\
  pc++;DISPATCH(); /* * */\
  \
}
#define DO_CPLU { /* + [4] */ \
  STACKCHECK(2);a=POP();b=POP();PUSH(a+b);\
  pc++;DISPATCH(); /* + */\
  \
}
#define DO_CMIN { /* - [5] */ \
  STACKCHECK(2);a=POP();b=POP();PUSH(b-a);\
  pc++;DISPATCH(); /* - */\
  \
}
#define DO_CDOT { /* . [6] */ \
  STACKCHECK(1); CONSOLEFMT(vmbase==0?"%d ":"%x ",vmbase==0?(sb2)POP():(ub2)POP()); pc++;\
  DISPATCH(); /* . */\
  \
}
#define DO_CQUO { /* / [7] */ \
  STACKCHECK(2);a=POP();b=POP();PUSH(b/a);\
  pc++;DISPATCH(); /* / */\
  \
}
#define DO_DEFINE { /* : [8] */ \
  DEBUG("DEFINE BRANCH %dn",(*(sb2*)(&code[pc+1])));\
  pc=pc+(*(sb2*)(&code[pc+1]));DISPATCH(); /* : */\
  \
}
#define DO_DEFRET { /* ; [9] */ \
  DEBUG("RETURNn");\
  RSTACKCHECK(1);pc=RPOP();\
  DISPATCH(); /* return */\
  \
}
#define DO_CLT { /* < [10] */ \
  STACKCHECK(2);a=POP();b=POP();PUSH(b<a?1:0);\
  pc++;DISPATCH(); /* < */\
  \
}
#define DO_CEQ { /* = [11] */ \
  STACKCHECK(2);a=POP();b=POP();PUSH(a==b?1:0);\
  pc++;DISPATCH(); /* = */\
  \
}
#define DO_CGT { /* > [12] */ \
  STACKCHECK(2);a=POP();b=POP();PUSH(b>a?1:0);\
  pc++;DISPATCH(); /* > */\
  \
}
#define DO_FETCH { /* @ [13] */ \
  STACKCHECK(1);addr=(ub2)POP();\
  PUSH(*((sb2*)&code[addr]));\
  pc++;DISPATCH(); /* @ */\
  \
}
#define DO_F { /* f [14] */ \
  FSTACKCHECK(1);a=FSTACKTOP(1);PUSH(a)\
  pc++;DISPATCH(); /* f */\
  \
}
#define DO_I { /* i [15] */ \
  FSTACKCHECK(2);PUSH(FSTACKTOP(1));\
  pc++;DISPATCH(); /* i */\
  \
}
#define DO_J { /* j [16] */ \
  FSTACKCHECK(4);PUSH(FSTACKTOP(3));\
  pc++;DISPATCH(); /* j */\
  \
}
#define DO_R { /* r [17] */ \
  RSTACKCHECK(1);a=RSTACKTOP(1);PUSH(a)\
  pc++;DISPATCH(); /* r */\
  \
}
#define DO_CDOTCSTR { /* ." [18] */ \
  char *str=(char *)&code[pc+1];CONSOLE(str);\
  pc+=(strlen(str)+2);DISPATCH(); /* ." */\
  \
}
#define DO_CDOTS { /* .s [19] */ \
  STACKCHECK(1);a=POP();b=a/2+a%2;\
  STACKCHECK(b);for(c=1;c<=b;c++) { ub2 cc=(ub2)POP(); CONSOLEFMT("%c",(cc>>8)&0xFF); CONSOLEFMT("%c",cc&0xFF);}\
  if (a%2==0) CONSOLEFMT("%c",' ');\
  pc++;DISPATCH(); /* .s */\
  \
}
#define DO_ZEROCLT { /* 0< [20] */ \
  STACKCHECK(1);a=POP();PUSH(a<0?1:0);\
  pc++;DISPATCH(); /* 0< */\
  \
}
#define DO_ZEROCEQ { /* 0= [21] */ \
  STACKCHECK(1);a=POP();PUSH(a==0?1:0);\
  pc++;DISPATCH(); /* 0= */\
  \
}
#define DO_TWOSTORE { /* 2! [22] */ \
  STACKCHECK(3);addr=(ub2)POP();da=POP2();\
  *((sb4*)&code[addr])=da;\
  pc++;DISPATCH(); /* 2! */\
  \
}
#define DO_TWOCMUL { /* 2* [23] */ \
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH2(da*db);\
  pc++;DISPATCH(); /* 2* */\
  \
}
#define DO_TWOCPLU { /* 2+ [24] */ \
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH2(da+db);\
  pc++;DISPATCH(); /* 2+ */\
  \
}
#define DO_TWOCMIN { /* 2- [25] */ \
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH2(db-da);\
  pc++;DISPATCH(); /* 2- */\
  \
}
#define DO_TWOCDOT { /* 2. [26] */ \
  STACKCHECK(2);CONSOLEFMT(vmbase==0?"%d ":"%x ",vmbase==0?(sb4)POP2():(ub4)POP2());\
  pc++;DISPATCH(); /* 2. */\
  \
}
#define DO_TWOCQUO { /* 2/ [27] */ \
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH2(db/da);\
  pc++;DISPATCH(); /* 2/ */\
  \
}
#define DO_TWOCLT { /* 2< [28] */ \
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH(db<da?1:0);\
  pc++;DISPATCH(); /* 2< */\
  \
}
#define DO_TWOCEQ { /* 2= [29] */ \
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH(da==db?1:0);\
  pc++;DISPATCH(); /* 2= */\
  \
}
#define DO_TWOCGT { /* 2> [30] */ \
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH(db>da?1:0);\
  pc++;DISPATCH(); /* 2> */\
  \
}
#define DO_TWOFETCH { /* 2@ [31] */ \
  STACKCHECK(1);addr=(ub2)POP();\
  PUSH2(*((sb4*)&code[addr]));\
  pc++;DISPATCH(); /* 2@ */\
  \
}
#define DO_DEFINECMUL { /* :* [32] */ \
  DEBUG("DEFINE BRANCH %dn",(*(sb2*)(&code[pc+1])));\
  pc=pc+(*(sb2*)(&code[pc+1]));DISPATCH(); /* :* */\
  \
}
#define DO_CGTF { /* >f [33] */ \
  STACKCHECK(1);a=POP();FPUSH(a);\
  pc++;DISPATCH(); /* >f */\
  \
}
#define DO_CGTR { /* >r [34] */ \
  STACKCHECK(1);a=POP();RPUSH(a);\
  pc++;DISPATCH(); /* >r */\
  \
}
#define DO_CR { /* cr [35] */ \
  CONSOLENL();\
  pc++;DISPATCH(); /* cr */\
  \
}
#define DO_DO { /* do [36] */ \
  STACKCHECK(2);FPUSH2U(POP2U());\
  pc+=3;DISPATCH(); /* do */\
  \
}
#define DO_FCGT { /* f> [37] */ \
  FSTACKCHECK(1);a=FPOP();PUSH(a);\
  pc++;DISPATCH(); /* f> */\
  \
}
#define DO_IF { /* if [38] */ \
  STACKCHECK(1); a=POP(); if (a==0) pc=(*(ub2*)(&code[pc+1])); else pc+=3;\
  DISPATCH(); /* if */\
  \
}
#define DO_OR { /* or [39] */ \
  STACKCHECK(2);a=POP();b=POP();PUSH((sb2)((ub2)a | (ub2)b));\
  pc++;DISPATCH(); /* or */\
  \
}
#define DO_RCGT { /* r> [40] */ \
  RSTACKCHECK(1);a=RPOP();PUSH(a);\
  pc++;DISPATCH(); /* r> */\
  \
}
#define DO_SSTORE { /* s! [41] */ \
  STACKCHECK(2);addr=(ub2)POP();a=POP();b=a/2+a%2;\
  STACKCHECK(b);*((sb2*)&code[addr])=a;\
  for(c=1;c<=b;c++) ((ub2*)&code[addr])[c]=(ub2)POP();\
  pc++;DISPATCH(); /* s! */\
  \
}
#define DO_SCSTR { /* s" [42] */ \
  ub1 *str=(ub1 *)&code[pc+1];a=strlen((char*)str);b=a/2+a%2;\
  for(c=b-1;c>=0;c--) PUSH((str[c*2]<<8)+str[c*2+1]); PUSH(a);\
  pc+=(a+2);DISPATCH(); /* s" */\
  \
}
#define DO_SFETCH { /* s@ [43] */ \
  STACKCHECK(1);addr=(ub2)POP();a=*((sb2*)&code[addr]);b=a/2+a%2;\
  for(c=b;c>0;c--) PUSH(((sb2*)&code[addr])[c]); PUSH(a);\
  pc++;DISPATCH(); /* s@ */\
  \
}
#define DO_TWOOR { /* 2or [44] */ \
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH2(da|db);\
  pc++;DISPATCH(); /* 2or */\
  \
}
#define DO_ABS { /* abs [45] */ \
  STACKCHECK(1);a=POP();\
  PUSH(a<0?-a:a);\
  pc++;DISPATCH(); /* abs */\
  \
}
#define DO_AND { /* and [46] */ \
  STACKCHECK(2);a=POP();b=POP();PUSH((sb2)((ub2)a & (ub2)b));\
  pc++;DISPATCH(); /* and */\
  \
}
#define DO_DUP { /* dup [47] */ \
  STACKCHECK(1);a=STACKTOP(1);\
  PUSH(a);\
  pc++;DISPATCH(); /* dup */\
  \
}
#define DO_END { /* end [48] */ \
  goto end; /* end */\
  \
}
#define DO_INP { /* inp [49] */ \
  pc++;DISPATCH(); /* inp */\
  \
}
#define DO_MAX { /* max [50] */ \
  STACKCHECK(2);a=POP();b=POP();PUSH(b>a?b:a);\
  pc++;DISPATCH(); /* max */\
  \
}
#define DO_MIN { /* min [51] */ \
  STACKCHECK(2);a=POP();b=POP();PUSH(b<a?b:a);\
  pc++;DISPATCH(); /* min */\
  \
}
#define DO_MOD { /* mod [52] */ \
  STACKCHECK(1);a=POP();b=POP();PUSH(b%a);\
  pc++;DISPATCH(); /* mod */\
  \
}
#define DO_ONE { /* one [53] */ \
  PUSH(1);\
  pc++;DISPATCH(); /* one */\
  \
}
#define DO_OUT { /* out [54] */ \
  STACKCHECK(1); OUT(POP());\
  pc++;DISPATCH(); /* out */\
  \
}
#define DO_ROT { /* rot [55] */ \
  STACKCHECK(3);c=POP();b=POP();a=POP();\
  PUSH(b);PUSH(c);PUSH(a);\
  pc++;DISPATCH(); /* rot */\
  \
}
#define DO_VAR { /* var [56] */ \
  pc+=3;DISPATCH(); /* var */\
  \
}
#define DO_XOR { /* xor [57] */ \
  STACKCHECK(2);a=POP();b=POP();PUSH((sb2)((ub2)a ^ (ub2)b));\
  pc++;DISPATCH(); /* xor */\
  \
}
#define DO_CMINDUP { /* -dup [58] */ \
  STACKCHECK(1);a=STACKTOP(1);PUSH(a);PUSH(a);\
  pc++;DISPATCH(); /* -dup */\
  \
}
#define DO_TWOABS { /* 2abs [59] */ \
  STACKCHECK(2);da=POP2();\
  PUSH2(da<0?-da:da);\
  pc++;DISPATCH(); /* 2abs */\
  \
}
#define DO_TWOAND { /* 2and [60] */ \
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH2(da&db);\
  pc++;DISPATCH(); /* 2and */\
  \
}
#define DO_TWODUP { /* 2dup [61] */ \
  STACKCHECK(2);da=STACKTOP2(1);\
  PUSH2(da);\
  pc++;DISPATCH(); /* 2dup */\
  \
}
#define DO_TWOEXT { /* 2ext [62] */ \
  STACKCHECK(1);a=POP();PUSH2((sb4)a);\
  pc++;DISPATCH(); /* 2ext */\
  \
}
#define DO_TWOMAX { /* 2max [63] */ \
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH2(db>da?db:da);\
  pc++;DISPATCH(); /* 2max */\
  \
}
#define DO_TWOMIN { /* 2min [64] */ \
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH2(db<da?db:da);\
  pc++;DISPATCH(); /* 2min */\
  \
}
#define DO_TWOMOD { /* 2mod [65] */ \
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH2(db%da);\
  pc++;DISPATCH(); /* 2mod */\
  \
}
#define DO_TWORED { /* 2red [66] */ \
  STACKCHECK(2);da=POP2();PUSH((sb2)da);\
  pc++;DISPATCH(); /* 2red */\
  \
}
#define DO_TWOROT { /* 2rot [67] */ \
  STACKCHECK(6);dc=POP2();db=POP2();da=POP2();\
  PUSH2(db);PUSH2(dc);PUSH2(da);\
  pc++;DISPATCH(); /* 2rot */\
  \
}
#define DO_TWOVAR { /* 2var [68] */ \
  pc+=5;DISPATCH(); /* 2var */\
  \
}
#define DO_TWOXOR { /* 2xor [69] */ \
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH2(da^db);\
  pc++;DISPATCH(); /* 2xor */\
  \
}
#define DO_BASE { /* base [70] */ \
  STACKCHECK(1);vmbase=POP();\
  pc++;DISPATCH(); /* base */\
  \
}
#define DO_DROP { /* drop [71] */ \
  STACKCHECK(1);POP();\
  pc++;DISPATCH(); /* drop */\
  \
}
#define DO_ELSE { /* else [72] */ \
  pc=(*(ub2*)(&code[pc+1]));\
  DISPATCH(); /* else */\
  \
}
#define DO_LOAD { /* load [73] */ \
  vmerror=ENOTIMPLEMENTED; goto onerror;\
   /* load */\
  \
}
#define DO_LOOP { /* loop [74] */ \
  FSTACKCHECK(2);FSTACKTOP(1)+=1;\
  if (FSTACKTOP(1)<FSTACKTOP(2)) pc=(*(ub2*)(&code[pc+1])); else { FPOP2(); pc+=3; }\
  DISPATCH(); /* loop */\
  \
}
#define DO_OVER { /* over [75] */ \
  STACKCHECK(2);a=STACKTOP(2);PUSH(a);\
  pc++;DISPATCH(); /* over */\
  \
}
#define DO_PICK { /* pick [76] */ \
  STACKCHECK(1);b=POP();STACKCHECK(b);a=STACKTOP(b);PUSH(a)\
  pc++;DISPATCH(); /* f */\
  \
}
#define DO_READ { /* read [77] */ \
  STACKCHECK(1);addr=(ub2)(-POP()-1);\
  if (addr>=64 || ios[addr].type!='D') { vmerror=EIO; goto onerror; };\
  if (ios[addr].cells) { STACKCHECK(1); a=POP();\
  if (a<0 || a>=ios[addr].cells) { vmerror=EINDEX; goto onerror; };\
  switch (ios[addr].size) {\
  case 1: PUSH(DIOSARRAYINDEX(ub1,addr,a));   break;\
  case 2: PUSH(DIOSARRAYINDEX(sb2,addr,a));   break;\
  case 4: PUSH2(DIOSARRAYINDEX(sb4,addr,a));  break;\
  }\
  } else {\
  switch (ios[addr].size) {\
  case 1: PUSH(DIOSDATADEREF(ub1,addr));   break;\
  case 2: PUSH(DIOSDATADEREF(sb2,addr));   break;\
  case 4: PUSH2(DIOSDATADEREF(sb4,addr));  break;\
  }\
  }\
  pc++;DISPATCH(); /* read */\
  \
}
#define DO_SAVE { /* save [78] */ \
  vmerror=ENOTIMPLEMENTED; goto onerror;\
   /* save */\
  \
}
#define DO_SEND { /* send [79] */ \
  STACKCHECK(2); a=POP(); b=POP();SEND(a,b);\
  pc++;DISPATCH(); /* send */\
  \
}
#define DO_SWAP { /* swap [80] */ \
  STACKCHECK(2);a=POP();b=POP();\
  PUSH(a);PUSH(b);\
  pc++;DISPATCH(); /* swap */\
  \
}
#define DO_CPLULOOP { /* +loop [81] */ \
  STACKCHECK(1);FSTACKCHECK(2);FSTACKTOP(1)+=(POP());\
  if (FSTACKTOP(1)<FSTACKTOP(2)) pc=(*(ub2*)(&code[pc+1])); else { FPOP2(); pc+=3; };\
  DISPATCH(); /* +loop */\
  \
}
#define DO_TWODROP { /* 2drop [82] */ \
  STACKCHECK(2);POP2();\
  pc++;DISPATCH(); /* 2drop */\
  \
}
#define DO_TWOOVER { /* 2over [83] */ \
  STACKCHECK(4);da=STACKTOP2(2);PUSH2(da);\
  pc++;DISPATCH(); /* 2over */\
  \
}
#define DO_TWOSWAP { /* 2swap [84] */ \
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH2(da);PUSH2(db);pc++;DISPATCH(); /* 2swap */\
  \
}
#define DO_AGAIN { /* again [85] */ \
  pc=(*(ub2*)(&code[pc+1]));\
  DISPATCH(); /* again */\
  \
}
#define DO_ARRAY { /* array [86] */ \
  DEBUG("ARRAY BRANCH %dn",(*(sb2*)(&code[pc+1])));\
  a=*((sb2*)&code[pc+1]); if (a<0) pc=pc+5; else pc=pc+a*2+3;\
  DISPATCH(); /* array */\
  \
}
#define DO_AWAIT { /* await [87] */ \
  STACKCHECK(2);addr=POP();a=POP();\
  if (ios[addr].type!='D') { vmerror=EIO; goto onerror; };\
  vmevent.v=&DIOSDATADEREF(ub2,addr);\
  vmevent.c=a;\
  pc++;goto suspend; /* await */\
  \
}
#define DO_BEGIN { /* begin [88] */ \
  pc+=3;DISPATCH(); /* begin */\
  \
}
#define DO_CELLCPLU { /* cell+ [89] */ \
  STACKCHECK(2);a=POP();addr=POP();PUSH(addr+a*2);\
  pc++;DISPATCH(); /* cell+ */\
  \
}
#define DO_CONST { /* const [90] */ \
  pc++;DISPATCH(); /* const */\
  \
}
#define DO_ENDIF { /* endif [91] */ \
  pc++;DISPATCH(); /* endif */\
  \
}
#define DO_FDROP { /* fdrop [92] */ \
  STACKCHECK(1);a=POP();FSTACKCHECK(a);fstop-=a;\
  pc++;DISPATCH(); /* f */\
  \
}
#define DO_FPICK { /* fpick [93] */ \
  STACKCHECK(1);b=POP();FSTACKCHECK(b);a=FSTACKTOP(b);PUSH(a)\
  pc++;DISPATCH(); /* f */\
  \
}
#define DO_LEAVE { /* leave [94] */ \
  addr=*(ub2*)(&code[pc+1]);\
  DEBUG("LEAVE %x %xn",addr,*(ub2*)(&code[addr]));\
  if (DECODE(code[addr-1])==OP_DO) FPOP2();\
  addr=*(ub2*)(&code[addr]);\
  pc=addr;\
  DISPATCH(); /* leave */\
  \
}
#define DO_MINUS { /* minus [95] */ \
  STACKCHECK(1);a=POP();\
  PUSH(-a);\
  pc++;DISPATCH(); /* minus */\
  \
}
#define DO_SENDN { /* sendn [96] */ \
  STACKCHECK(3); addr=POP(); a=POP(); b=POP(); SENDN(addr,a,b);\
  pc++;DISPATCH(); /* send */\
  \
}
#define DO_SLEEP { /* sleep [97] */ \
  STACKCHECK(1); a=POP(); vmevent.timeout=vmMilli()+a;\
  pc++;goto suspend; /* sleep */\
  \
}
#define DO_UNTIL { /* until [98] */ \
  STACKCHECK(1);a=POP();\
  if (a) pc+=3; else pc=(*(ub2*)(&code[pc+1]));\
  DISPATCH(); /* until */\
  \
}
#define DO_WHILE { /* while [99] */ \
  STACKCHECK(1);a=POP();\
  if (a==0) pc+=3; else pc=(*(ub2*)(&code[pc+1]));\
  pc++;DISPATCH(); /* while */\
  \
}
#define DO_WRITE { /* write [100] */ \
  STACKCHECK(1);addr=(ub2)(-POP()-1);\
  if (addr>=64 || ios[addr].type!='D') { vmerror=EIO; goto onerror; };\
  if (ios[addr].cells) { STACKCHECK(1); a=POP();\
  if (a<0 || a>=ios[addr].cells) { vmerror=EINDEX; goto onerror; };\
  switch (ios[addr].size) {\
  case 1: STACKCHECK(1); DIOSARRAYINDEX(ub1,addr,a)=(ub1)POP(); break;\
  case 2: STACKCHECK(1); DIOSARRAYINDEX(sb2,addr,a)=POP();      break;\
  case 4: STACKCHECK(2); DIOSARRAYINDEX(sb4,addr,a)=POP2();     break;\
  }\
  } else {\
  switch (ios[addr].size) {\
  case 1: STACKCHECK(1); DIOSDATADEREF(ub1,addr)=(ub1)POP();    break;\
  case 2: STACKCHECK(1); DIOSDATADEREF(sb2,addr)=POP();         break;\
  case 4: STACKCHECK(2); DIOSDATADEREF(sb4,addr)=POP2();        break;\
  }\
  }\
  pc++;DISPATCH(); /* write */\
  \
}
#define DO_YIELD { /* yield [101] */ \
  DEBUG("yield\n"); pc++; goto end; /* yield */\
  \
}
#define DO_TWOMINUS { /* 2minus [102] */ \
  STACKCHECK(2);da=POP2();\
  PUSH2(-da);\
  pc++;DISPATCH(); /* 2minus */\
  \
}
#define DO_FORGET { /* forget [103] */ \
  vmerror=ENOTIMPLEMENTED; goto onerror;\
   /* forget */\
  \
}
#define DO_RETURN { /* return [104] */ \
  DEBUG("RETURNn");\
  RSTACKCHECK(1);pc=RPOP();\
  DISPATCH(); /* ; */\
  \
}
#define DO_RECEIVE { /* receive [105] */ \
  vmerror=ENOTIMPLEMENTED; goto onerror;\
   /* receive */\
  \
}
#define DO_CALL { /* call [106] */ \
  addr=*(ub2*)(&code[pc+1]);\
  DEBUG("CALL %dn",a);\
  RPUSH(pc+3);\
  pc=addr;DISPATCH(); /* call */\
  \
}
#define DO_FCALL { /* fcall [107] */ \
  /* must be adapated to all possible FIOS calles!*/\
  pc++;addr=(-(sb1)code[pc])-1;\
  if (addr>64 || ios[addr].type!='F') { vmerror=EFCALL; goto onerror; };\
  if (ios[addr].argsize==2) switch (ios[addr].args) {\
  case 1: STACKCHECK(1); a=POP(); break;\
  case 2: STACKCHECK(2); b=POP(); a=POP(); break;\
  case 3: STACKCHECK(3); c=POP(); b=POP(); a=POP(); break;\
  case 4: STACKCHECK(4); d=POP(); c=POP(); b=POP(); a=POP(); break;\
  case 8: STACKCHECK(7); h = POP(); g=POP(); f=POP(); e=POP(); d=POP(); c=POP(); b=POP(); a=POP(); break;\
  }\
  if (ios[addr].argsize==4) switch (ios[addr].args) {\
  case 1: STACKCHECK(2); da=POP2(); break;\
  case 2: STACKCHECK(4); db=POP2(); da=POP2(); break;\
  case 3: STACKCHECK(6); dc=POP2(); db=POP2(); da=POP2(); break;\
  case 4: STACKCHECK(8); dd=POP2(); dc=POP2(); db=POP2(); da=POP2(); break;\
  }\
  if (ios[addr].retsize==0 && ios[addr].argsize<4) switch (ios[addr].args) {\
  case 0: FIOSCALL0(addr); break;\
  case 1: FIOSCALL0(addr,a); break;\
  case 2: FIOSCALL0(addr,a,b); break;\
  case 3: FIOSCALL0(addr,a,b,c); break;\
  case 4: FIOSCALL0(addr,a,b,c,d); break;\
  case 8: FIOSCALL0(addr,a,b,c,d,e,f,g,h); break;\
  }\
  else if (ios[addr].retsize==0 && ios[addr].argsize==4) switch (ios[addr].args) {\
  case 1: FIOSCALL0(addr,da); break;\
  case 2: FIOSCALL0(addr,da,db); break;\
  case 3: FIOSCALL0(addr,da,db,dc); break;\
  case 4: FIOSCALL0(addr,da,db,dc,dd); break;\
  }\
  else if (ios[addr].retsize==2 && (ios[addr].argsize==2||ios[addr].args==0)) switch (ios[addr].args) {\
  case 0: PUSH(FIOSCALL2(addr)); break;\
  case 1: PUSH(FIOSCALL2(addr,a)); break;\
  case 2: PUSH(FIOSCALL2(addr,a,b)); break;\
  case 3: PUSH(FIOSCALL2(addr,a,b,c)); break;\
  case 4: PUSH(FIOSCALL2(addr,a,b,c,d)); break;\
  }\
  else if (ios[addr].retsize==4 && (ios[addr].argsize==2||ios[addr].args==0)) switch (ios[addr].args) {\
  case 0: PUSH2(FIOSCALL4(addr)); break;\
  case 1: PUSH2(FIOSCALL4(addr,a)); break;\
  case 2: PUSH2(FIOSCALL4(addr,a,b)); break;\
  case 3: PUSH2(FIOSCALL4(addr,a,b,c)); break;\
  case 4: PUSH2(FIOSCALL4(addr,a,b,c,d)); break;\
  }\
  else if (ios[addr].retsize==0 && (ios[addr].argsize==4||ios[addr].args==0)) switch (ios[addr].args) {\
  case 0: FIOSCALL0(addr); break;\
  case 1: FIOSCALL0(addr,da); break;\
  case 2: FIOSCALL0(addr,da,db); break;\
  case 3: FIOSCALL0(addr,da,db,dc); break;\
  case 4: FIOSCALL0(addr,da,db,dc,dd); break;\
  }\
  else if (ios[addr].retsize==2 && (ios[addr].argsize==4||ios[addr].args==0)) switch (ios[addr].args) {\
  case 0: PUSH(FIOSCALL2(addr)); break;\
  case 1: PUSH(FIOSCALL2(addr,da)); break;\
  case 2: PUSH(FIOSCALL2(addr,da,db)); break;\
  case 3: PUSH(FIOSCALL2(addr,da,db,dc)); break;\
  case 4: PUSH(FIOSCALL2(addr,da,db,dc,dd)); break;\
  }\
  else if (ios[addr].retsize==4 && (ios[addr].argsize==4||ios[addr].args==0)) switch (ios[addr].args) {\
  case 0: PUSH2(FIOSCALL4(addr)); break;\
  case 1: PUSH2(FIOSCALL4(addr,da)); break;\
  case 2: PUSH2(FIOSCALL4(addr,da,db)); break;\
  case 3: PUSH2(FIOSCALL4(addr,da,db,dc)); break;\
  case 4: PUSH2(FIOSCALL4(addr,da,db,dc,dd)); break;\
  }\
  if (vmerror) goto onerror; pc++;DISPATCH(); /* fcall */\
  \
}
#define DO_BRANCH { /* branch [108] */ \
  pc=(*(ub2*)(&code[pc+1]));DISPATCH();\
  \
}
#define DO_BRANCHZ { /* branchz [109] */ \
  a=POP();if (a==0) pc=(*(ub2*)(&code[pc+1])); else pc+=3;DISPATCH();\
  \
}
#define DO_INDIRECT { /* indirect [110] */ \
  addr=*(ub2*)(&code[pc+1]);addr=*(ub2*)(&code[addr]);PUSH(addr);\
  pc=pc+3;DISPATCH();\
  \
}
#define DO_MULADD { /* muladd [111] */ \
  ub1 v=(ub1)code[pc+1]; STACKCHECK(1); \
  if (v&0x8) STACKTOP(1)=STACKTOP(1)*(v>>4)-(8-v&0x7); \
  else STACKTOP(1)=STACKTOP(1)*(v>>4)+(v&0x7);\
  pc+=2;DISPATCH(); /* MULADD */\
  \
}
#define DO_NOP { /* nop [112] */ \
  pc++;DISPATCH();\
  \
}
#define DO_LIT { /* lit [113] */ \
  DEBUG("LIT %xn",EXTENDSIGN((code[pc]&0x3F)<<8)|code[pc+1]);\
  PUSH((sb2)((EXTENDSIGN(code[pc]&0x3F)<<8)|code[pc+1])); pc+=2;\
  DISPATCH();\
  \
}
#define DO_TWOLIT { /* 2lit [114] */ \
  DEBUG("TWOLIT %xn",EXTENDSIGN((code[pc]&0x3F)<<24)|(code[pc+1]<<16)|(code[pc+2]<<8)|code[pc+3]);\
  PUSH2((ub4)(EXTENDSIGN((code[pc]&0x3F)<<24)|(code[pc+1]<<16)|(code[pc+2]<<8)|code[pc+3])); pc+=4;\
  DISPATCH();\
  \
}
#endif
