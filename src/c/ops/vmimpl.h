#define DO_DUP {\
  STACKCHECK(1);a=STACKTOP(1);\
  PUSH(a);\
  pc++;DISPATCH();} /* dup */
#define DO_DROP {\
  STACKCHECK(1);POP();\
  pc++;DISPATCH();} /* drop */
#define DO_SWAP {\
  STACKCHECK(2);a=POP();b=POP();\
  PUSH(a);PUSH(b);\
  pc++;DISPATCH();} /* swap */
#define DO_OVER {\
  STACKCHECK(2);a=STACKTOP(2);PUSH(a);\
  pc++;DISPATCH();} /* over */
#define DO_ROT {\
  STACKCHECK(3);a=POP();b=POP();c=POP();\
  PUSH(b);PUSH(a);PUSH(c);\
  pc++;DISPATCH();} /* rot */
#define DO_CMINDUP {\
  STACKCHECK(1);if (STACKTOP(1)!=0) { a=STACKTOP(1);PUSH(a); };\
  pc++;DISPATCH();} /* -dup */
#define DO_PICK {\
  STACKCHECK(1);b=POP();STACKCHECK(b);a=STACKTOP(b);PUSH(a)\
  pc++;DISPATCH();} /* f */
#define DO_CGTF {\
  STACKCHECK(1);a=POP();FPUSH(a);\
  pc++;DISPATCH();} /* >f */
#define DO_FCGT {\
  FSTACKCHECK(1);a=FPOP();PUSH(a);\
  pc++;DISPATCH();} /* f> */
#define DO_F {\
  FSTACKCHECK(1);a=FPOP();PUSH(a)\
  pc++;DISPATCH();} /* f */
#define DO_FPICK {\
  STACKCHECK(1);b=POP();FSTACKCHECK(b);a=FSTACKTOP(b);PUSH(a)\
  pc++;DISPATCH();} /* f */
#define DO_FDROP {\
  STACKCHECK(1);a=POP();FSTACKCHECK(a);fstop-=a;\
  pc++;DISPATCH();} /* f */
#define DO_CPLU {\
  STACKCHECK(2);a=POP();b=POP();PUSH(a+b);\
  pc++;DISPATCH();} /* + */
#define DO_CMIN {\
  STACKCHECK(2);a=POP();b=POP();PUSH(b-a);\
  pc++;DISPATCH();} /* - */
#define DO_CMUL {\
  STACKCHECK(2);a=POP();b=POP();PUSH(a*b);\
  pc++;DISPATCH();} /* * */
#define DO_CQUO {\
  STACKCHECK(2);a=POP();b=POP();PUSH(b/a);\
  pc++;DISPATCH();} /* / */
#define DO_MOD {\
  STACKCHECK(1);a=POP();b=POP();PUSH(b%a);\
  pc++;DISPATCH();} /* mod */
#define DO_MAX {\
  STACKCHECK(2);a=POP();b=POP();PUSH(b>a?b:a);\
  pc++;DISPATCH();} /* max */
#define DO_MIN {\
  STACKCHECK(2);a=POP();b=POP();PUSH(b<a?b:a);\
  pc++;DISPATCH();} /* min */
#define DO_ABS {\
  STACKCHECK(1);a=POP();\
  PUSH(a<0?-a:a);\
  pc++;DISPATCH();} /* abs */
#define DO_MINUS {\
  STACKCHECK(1);a=POP();\
  PUSH(-a);\
  pc++;DISPATCH();} /* minus */
#define DO_AND {\
  STACKCHECK(2);a=POP();b=POP();PUSH((sb2)((ub2)a & (ub2)b));\
  pc++;DISPATCH();} /* and */
#define DO_OR {\
  STACKCHECK(2);a=POP();b=POP();PUSH((sb2)((ub2)a | (ub2)b));\
  pc++;DISPATCH();} /* or */
#define DO_XOR {\
  STACKCHECK(2);a=POP();b=POP();PUSH((sb2)((ub2)a ^ (ub2)b));\
  pc++;DISPATCH();} /* xor */
#define DO_CEQ {\
  STACKCHECK(2);a=POP();b=POP();PUSH(a==b?1:0);\
  pc++;DISPATCH();} /* = */
#define DO_CLT {\
  STACKCHECK(2);a=POP();b=POP();PUSH(b<a?1:0);\
  pc++;DISPATCH();} /* < */
#define DO_CGT {\
  STACKCHECK(2);a=POP();b=POP();PUSH(b>a?1:0);\
  pc++;DISPATCH();} /* > */
#define DO_ZEROCLT {\
  STACKCHECK(1);a=POP();PUSH(a<0?1:0);\
  pc++;DISPATCH();} /* 0< */
#define DO_ZEROCEQ {\
  STACKCHECK(1);a=POP();PUSH(a==0?1:0);\
  pc++;DISPATCH();} /* 0= */
#define DO_CDOT {\
  STACKCHECK(1); CONSOLEFMT(vmbase==0?"%d ":"%x ",vmbase==0?(sb2)POP():(ub2)POP()); pc++;\
  DISPATCH();} /* . */
#define DO_CDOTCSTR {\
  char *str=(char *)&code[pc+1];CONSOLE(str);\
  pc+=(strlen(str)+2);DISPATCH();} /* ." */
#define DO_CR {\
  CONSOLENL();\
  pc++;DISPATCH();} /* cr */
#define DO_FETCH {\
  STACKCHECK(1);addr=(ub2)POP();\
  PUSH(*((sb2*)&code[addr]));\
  pc++;DISPATCH();} /* @ */
#define DO_SFETCH {\
  STACKCHECK(1);addr=(ub2)POP();a=*((sb2*)&code[addr]);b=a/2+a%2;\
  printf("s@ %x %d %d\n",addr,a,b);\
  for(c=b;c>0;c--) PUSH(((sb2*)&code[addr])[c]); PUSH(a);\
  pc++;DISPATCH();} /* s@ */
#define DO_SCSTR {\
  ub1 *str=(char *)&code[pc+1];a=strlen(str);b=a/2+a%2;\
  printf("s\" %d %d %s\n",a,b,str);\
  for(c=b-1;c>=0;c--) PUSH((str[c*2]<<8)+str[c*2+1]); PUSH(a);\
  pc+=(a+2);DISPATCH();} /* s" */
#define DO_STORE {\
  STACKCHECK(2);addr=(ub2)POP();a=POP();\
  *((sb2*)&code[addr])=a;\
  pc++;DISPATCH();} /* ! */
#define DO_CELLCPLU {\
  STACKCHECK(2);a=POP();addr=POP();PUSH(addr+a*2);\
  pc++;DISPATCH();} /* cell+ */
#define DO_ARRAY {\
  DEBUG("ARRAY BRANCH %d\n",(*(sb2*)(&code[pc+1])));\
  a=*((sb2*)&code[pc+1]); if (a<0) pc=pc+5; else pc=pc+a*2+3;\
  DISPATCH();} /* array */
#define DO_DEFINE {\
  DEBUG("DEFINE BRANCH %d\n",(*(sb2*)(&code[pc+1])));\
  pc=pc+(*(sb2*)(&code[pc+1]));DISPATCH();} /* : */
#define DO_DEFRET {\
  DEBUG("RETURN\n");\
  RSTACKCHECK(1);pc=RPOP();\
  DISPATCH();} /* return */
#define DO_RETURN {\
  DEBUG("RETURN\n");\
  RSTACKCHECK(1);pc=RPOP();\
  DISPATCH();} /* ; */
#define DO_DO {\
  STACKCHECK(2);FPUSH2U(POP2U());\
  pc+=3;DISPATCH();} /* do */
#define DO_LOOP {\
  FSTACKCHECK(2);FSTACKTOP(1)+=1;\
  if (FSTACKTOP(1)<FSTACKTOP(2)) pc=(*(ub2*)(&code[pc+1])); else { FPOP2(); pc+=3; }\
  DISPATCH();} /* loop */
#define DO_CPLULOOP {\
  STACKCHECK(1);FSTACKCHECK(2);FSTACKTOP(1)+=(POP());\
  if (FSTACKTOP(1)<FSTACKTOP(2)) pc=(*(ub2*)(&code[pc+1])); else { FPOP2(); pc+=3; };\
  DISPATCH();} /* +loop */
#define DO_LEAVE {\
  addr=*(ub2*)(&code[pc+1]);\
  DEBUG("LEAVE %x %x\n",addr,*(ub2*)(&code[addr]));\
  if (DECODE(code[addr-1])==OP_DO) FPOP2();\
  addr=*(ub2*)(&code[addr]);\
  pc=addr;\
  DISPATCH();} /* leave */
#define DO_I {\
  FSTACKCHECK(2);PUSH(FSTACKTOP(1));\
  pc++;DISPATCH();} /* i */
#define DO_J {\
  FSTACKCHECK(4);PUSH(FSTACKTOP(3));\
  pc++;DISPATCH();} /* j */
#define DO_IF {\
  STACKCHECK(1); a=POP(); if (a==0) pc=(*(ub2*)(&code[pc+1])); else pc+=3;\
  DISPATCH();} /* if */
#define DO_ELSE {\
  pc=(*(ub2*)(&code[pc+1]));\
  DISPATCH();} /* else */
#define DO_ENDIF {\
  pc++;DISPATCH();} /* endif */
#define DO_BEGIN {\
  pc+=3;DISPATCH();} /* begin */
#define DO_AGAIN {\
  pc=(*(ub2*)(&code[pc+1]));\
  DISPATCH();} /* again */
#define DO_UNTIL {\
  STACKCHECK(1);a=POP();\
  if (a) pc+=3; else pc=(*(ub2*)(&code[pc+1]));\
  DISPATCH();} /* until */
#define DO_WHILE {\
  STACKCHECK(1);a=POP();\
  if (a==0) pc+=3; else pc=(*(ub2*)(&code[pc+1]));\
  pc++;DISPATCH();} /* while */
#define DO_VAR {\
  \
  pc+=3;DISPATCH();} /* var */
#define DO_CONST {\
  \
  pc++;DISPATCH();} /* const */
#define DO_FORGET {\
  vmerror=ENOTIMPLEMENTED; goto onerror;\
  } /* forget */
#define DO_SEND {\
  STACKCHECK(2); a=POP(); b=POP();SEND(a,b);\
  pc++;DISPATCH();} /* send */
#define DO_SENDN {\
  STACKCHECK(3); addr=POP(); a=POP(); b=POP(); SENDN(addr,a,b);\
  pc++;DISPATCH();} /* send */
#define DO_RECEIVE {\
  vmerror=ENOTIMPLEMENTED; goto onerror;\
  } /* receive */
#define DO_READ {\
  STACKCHECK(1);addr=(ub2)(-POP()-1);\
  if (addr>=IOSIZE || ios[addr].type!='D') { vmerror=EIO; goto onerror; };\
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
  pc++;DISPATCH();} /* read */
#define DO_WRITE {\
  STACKCHECK(1);addr=(ub2)(-POP()-1);\
  if (addr>=IOSIZE || ios[addr].type!='D') { vmerror=EIO; goto onerror; };\
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
  pc++;DISPATCH();} /* write */
#define DO_LOAD {\
  vmerror=ENOTIMPLEMENTED; goto onerror;\
  } /* load */
#define DO_SAVE {\
  vmerror=ENOTIMPLEMENTED; goto onerror;\
  } /* save */
#define DO_OUT {\
  STACKCHECK(1); OUT(POP());\
  pc++;DISPATCH();} /* out */
#define DO_INP {\
  \
  pc++;DISPATCH();} /* inp */
#define DO_SLEEP {\
  STACKCHECK(1); a=POP(); vmevent.timeout=vmMilli()+a;\
  pc++;goto suspend;} /* sleep */
#define DO_FCALL {\
  pc++;addr=(-(sb1)code[pc])-1;\
  if (addr>IOSIZE || ios[addr].type!='F') { vmerror=EFCALL; goto onerror; };\
  if (ios[addr].argsize==2) switch (ios[addr].args) {\
    case 1: STACKCHECK(1); a=POP(); break;\
    case 2: STACKCHECK(2); b=POP(); a=POP(); break;\
    case 3: STACKCHECK(3); c=POP(); b=POP(); a=POP(); break;\
    case 4: STACKCHECK(4); d=POP(); c=POP(); b=POP(); a=POP(); break;\
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
  if (vmerror) goto onerror; pc++;DISPATCH();} /* fcall */
#define DO_YIELD {\
  DEBUG("yield\n"); pc++; goto end;} /* yield */
#define DO_BASE {\
  STACKCHECK(1);vmbase=POP();\
  pc++;DISPATCH();} /* base */
#define DO_ONE {\
  PUSH(1);\
  pc++;DISPATCH();} /* one */
#define DO_AWAIT {\
  STACKCHECK(2);addr=POP();a=POP();\
  if (ios[addr].type!='D') { vmerror=EIO; goto onerror; };\
  vmevent.v=&DIOSDATADEREF(ub2,addr);\
  vmevent.c=a;\
  pc++;goto suspend;} /* await */
#define DO_END {\
  goto end;} /* end */
#define DO_TWODUP {\
  STACKCHECK(2);da=STACKTOP2(1);\
  PUSH2(da);\
  pc++;DISPATCH();} /* 2dup */
#define DO_TWODROP {\
  STACKCHECK(2);POP2();\
  pc++;DISPATCH();} /* 2drop */
#define DO_TWOSWAP {\
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH2(da);PUSH2(db);pc++;DISPATCH();} /* 2swap */
#define DO_TWOOVER {\
  vmerror=ENOTIMPLEMENTED; goto onerror;\
  } /* 2over */
#define DO_TWOROT {\
  vmerror=ENOTIMPLEMENTED; goto onerror;\
  } /* 2rot */
#define DO_TWOCPLU {\
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH2(da+db);\
  pc++;DISPATCH();} /* 2+ */
#define DO_TWOCMIN {\
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH2(db-da);\
  pc++;DISPATCH();} /* 2- */
#define DO_TWOCMUL {\
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH2(da*db);\
  pc++;DISPATCH();} /* 2* */
#define DO_TWOCQUO {\
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH2(db/da);\
  pc++;DISPATCH();} /* 2/ */
#define DO_TWOMOD {\
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH2(db%da);\
  pc++;DISPATCH();} /* 2mod */
#define DO_TWOMAX {\
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH2(db>da?db:da);\
  pc++;DISPATCH();} /* 2max */
#define DO_TWOMIN {\
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH2(db<da?db:da);\
  pc++;DISPATCH();} /* 2min */
#define DO_TWOABS {\
  STACKCHECK(2);da=POP2();\
  PUSH2(da<0?-da:da);\
  pc++;DISPATCH();} /* 2abs */
#define DO_TWOMINUS {\
  STACKCHECK(2);da=POP2();\
  PUSH2(-da);\
  pc++;DISPATCH();} /* 2minus */
#define DO_TWOAND {\
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH2(da&db);\
  pc++;DISPATCH();} /* 2and */
#define DO_TWOOR {\
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH2(da|db);\
  pc++;DISPATCH();} /* 2or */
#define DO_TWOXOR {\
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH2(da^db);\
  pc++;DISPATCH();} /* 2xor */
#define DO_TWOCEQ {\
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH(da==db?1:0);\
  pc++;DISPATCH();} /* 2= */
#define DO_TWOCLT {\
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH(db<da?1:0);\
  pc++;DISPATCH();} /* 2< */
#define DO_TWOCGT {\
  STACKCHECK(4);da=POP2();db=POP2();\
  PUSH(db>da?1:0);\
  pc++;DISPATCH();} /* 2> */
#define DO_TWOCDOT {\
  STACKCHECK(2);CONSOLEFMT(vmbase==0?"%d ":"%x ",vmbase==0?(sb4)POP2():(ub4)POP2());\
  pc++;DISPATCH();} /* 2. */
#define DO_CDOTS {\
  STACKCHECK(1);a=POP();b=a/2+a%2;\
  STACKCHECK(b);for(c=1;c<=b;c++) { ub2 cc=(ub2)POP(); CONSOLEFMT("%c",(cc>>8)&0xFF); CONSOLEFMT("%c",cc&0xFF);}\
  if (a%2==0) CONSOLEFMT("%c",' ');\
  pc++;DISPATCH();} /* .s */
#define DO_TWOFETCH {\
  STACKCHECK(1);addr=(ub2)POP();\
  PUSH2(*((sb4*)&code[addr]));\
  pc++;DISPATCH();} /* 2@ */
#define DO_TWOSTORE {\
  STACKCHECK(3);addr=(ub2)POP();da=POP2();\
  *((sb4*)&code[addr])=da;\
  pc++;DISPATCH();} /* 2! */
#define DO_SSTORE {\
  STACKCHECK(2);addr=(ub2)POP();a=POP();b=a/2+a%2;\
  STACKCHECK(b);*((sb2*)&code[addr])=a;\
  for(c=1;c<=b;c++) ((ub2*)&code[addr])[c]=(ub2)POP();\
  pc++;DISPATCH();} /* s! */
#define DO_TWOEXT {\
  STACKCHECK(1);a=POP();PUSH2((sb4)a);\
  pc++;DISPATCH();} /* 2ext */
#define DO_TWORED {\
  STACKCHECK(2);da=POP2();PUSH((sb2)da);\
  pc++;DISPATCH();} /* 2red */
#define DO_CDOLL {\
  if ((code[pc+1]&OP_CALL)==OP_CALL) { a=*(sb2*)(&code[pc+2]); pc+=4;}\
  else { a=*(sb1*)(&code[pc+2]); pc+=3; };\
  PUSH(a); DISPATCH();} /* $ */
#define DO_TWOVAR {\
  \
  pc+=5;DISPATCH();} /* 2var */
#define DO_BRANCHZ {\
  \
  a=POP();if (a==0) pc=(*(ub2*)(&code[pc+1])); else pc+=3;DISPATCH();}
#define DO_BRANCH {\
  \
   pc=(*(ub2*)(&code[pc+1]));DISPATCH();}
#define DO_CALL {\
  /* TODO rel. branch addr=pc+*(sb2*)(&code[pc+1])*/\
  addr=*(ub2*)(&code[pc+1]);\
  DEBUG("CALL %d\n",a);\
  RPUSH(pc+3);\
  pc=addr;DISPATCH();} /* call */
#define DO_INDIRECT {\
  addr=*(ub2*)(&code[pc+1]);addr=*(ub2*)(&code[addr]);PUSH(addr);\
  pc=pc+3;DISPATCH();}
#define DO_NOP {\
  \
  pc++;DISPATCH();}
#define DO_LIT {\
  DEBUG("LIT %x\n",EXTENDSIGN(code[pc]&0x3F)<<8|code[pc+1]);\
  PUSH((ub2)(EXTENDSIGN(code[pc]&0x3F)<<8|code[pc+1])); pc+=2;\
  DISPATCH();}
#define DO_TWOLIT {\
  DEBUG("TWOLIT %x\n",EXTENDSIGN(code[pc]&0x3F)<<24|code[pc+1]<<16|code[pc+2]<<8|code[pc+3]);\
  PUSH2((ub4)(EXTENDSIGN(code[pc]&0x3F)<<24|code[pc+1]<<16|code[pc+2]<<8|code[pc+3])); pc+=4;\
  DISPATCH();}
  
