STACKCHECK(1);addr=(ub2)(-POP()-1);
if (addr>=IOSIZE || ios[addr].type!='D') { vmerror=EIO; goto onerror; };
if (ios[addr].cells) { STACKCHECK(1); a=POP();
if (a<0 || a>=ios[addr].cells) { vmerror=EINDEX; goto onerror; };
switch (ios[addr].size) {
case 1: STACKCHECK(1); DIOSARRAYINDEX(ub1,addr,a)=(ub1)POP(); break;
case 2: STACKCHECK(1); DIOSARRAYINDEX(sb2,addr,a)=POP();      break;
case 4: STACKCHECK(2); DIOSARRAYINDEX(sb4,addr,a)=POP2();     break;
}
} else {
switch (ios[addr].size) {
case 1: STACKCHECK(1); DIOSDATADEREF(ub1,addr)=(ub1)POP();    break;
case 2: STACKCHECK(1); DIOSDATADEREF(sb2,addr)=POP();         break;
case 4: STACKCHECK(2); DIOSDATADEREF(sb4,addr)=POP2();        break;
}
}
pc++;DISPATCH(); /* write */