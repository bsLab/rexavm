STACKCHECK(1);addr=(ub2)(-POP()-1);
if (addr>=IOSIZE || ios[addr].type!='D') { vmerror=EIO; goto onerror; };
if (ios[addr].cells) { STACKCHECK(1); a=POP();
if (a<0 || a>=ios[addr].cells) { vmerror=EINDEX; goto onerror; };
switch (ios[addr].size) {
case 1: PUSH(DIOSARRAYINDEX(ub1,addr,a));   break;
case 2: PUSH(DIOSARRAYINDEX(sb2,addr,a));   break;
case 4: PUSH2(DIOSARRAYINDEX(sb4,addr,a));  break;
}
} else {
switch (ios[addr].size) {
case 1: PUSH(DIOSDATADEREF(ub1,addr));   break;
case 2: PUSH(DIOSDATADEREF(sb2,addr));   break;
case 4: PUSH2(DIOSDATADEREF(sb4,addr));  break;
}
}
pc++;DISPATCH(); /* read */