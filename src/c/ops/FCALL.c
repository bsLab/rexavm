#if DATAWIDTH==16
/* must be adapated to all possible FIOS calles!*/
pc++;addr=(-(sb1)code[pc])-1;
if (addr>IOSIZE || ios[addr].type!='F') { vmerror=EFCALL; goto onerror; };
if (ios[addr].argsize==2) switch (ios[addr].args) {
case 1: STACKCHECK(1); a=POP(); break;
case 2: STACKCHECK(2); b=POP(); a=POP(); break;
case 3: STACKCHECK(3); c=POP(); b=POP(); a=POP(); break;
case 4: STACKCHECK(4); d=POP(); c=POP(); b=POP(); a=POP(); break;
case 8: STACKCHECK(7); h = POP(); g=POP(); f=POP(); e=POP(); d=POP(); c=POP(); b=POP(); a=POP(); break;
}
if (ios[addr].argsize==4) switch (ios[addr].args) {
case 1: STACKCHECK(2); da=POP2(); break;
case 2: STACKCHECK(4); db=POP2(); da=POP2(); break;
case 3: STACKCHECK(6); dc=POP2(); db=POP2(); da=POP2(); break;
case 4: STACKCHECK(8); dd=POP2(); dc=POP2(); db=POP2(); da=POP2(); break;
}
if (ios[addr].retsize==0 && ios[addr].argsize<4) switch (ios[addr].args) {
case 0: FIOSCALL0(addr); break;
case 1: FIOSCALL0(addr,a); break;
case 2: FIOSCALL0(addr,a,b); break;
case 3: FIOSCALL0(addr,a,b,c); break;
case 4: FIOSCALL0(addr,a,b,c,d); break;
case 8: FIOSCALL0(addr,a,b,c,d,e,f,g,h); break;
}
else if (ios[addr].retsize==0 && ios[addr].argsize==4) switch (ios[addr].args) {
case 1: FIOSCALL0(addr,da); break;
case 2: FIOSCALL0(addr,da,db); break;
case 3: FIOSCALL0(addr,da,db,dc); break;
case 4: FIOSCALL0(addr,da,db,dc,dd); break;
}
else if (ios[addr].retsize==2 && (ios[addr].argsize==2||ios[addr].args==0)) switch (ios[addr].args) {
case 0: PUSH(FIOSCALL2(addr)); break;
case 1: PUSH(FIOSCALL2(addr,a)); break;
case 2: PUSH(FIOSCALL2(addr,a,b)); break;
case 3: PUSH(FIOSCALL2(addr,a,b,c)); break;
case 4: PUSH(FIOSCALL2(addr,a,b,c,d)); break;
}
else if (ios[addr].retsize==4 && (ios[addr].argsize==2||ios[addr].args==0)) switch (ios[addr].args) {
case 0: PUSH2(FIOSCALL4(addr)); break;
case 1: PUSH2(FIOSCALL4(addr,a)); break;
case 2: PUSH2(FIOSCALL4(addr,a,b)); break;
case 3: PUSH2(FIOSCALL4(addr,a,b,c)); break;
case 4: PUSH2(FIOSCALL4(addr,a,b,c,d)); break;
}
else if (ios[addr].retsize==0 && (ios[addr].argsize==4||ios[addr].args==0)) switch (ios[addr].args) {
case 0: FIOSCALL0(addr); break;
case 1: FIOSCALL0(addr,da); break;
case 2: FIOSCALL0(addr,da,db); break;
case 3: FIOSCALL0(addr,da,db,dc); break;
case 4: FIOSCALL0(addr,da,db,dc,dd); break;
}
else if (ios[addr].retsize==2 && (ios[addr].argsize==4||ios[addr].args==0)) switch (ios[addr].args) {
case 0: PUSH(FIOSCALL2(addr)); break;
case 1: PUSH(FIOSCALL2(addr,da)); break;
case 2: PUSH(FIOSCALL2(addr,da,db)); break;
case 3: PUSH(FIOSCALL2(addr,da,db,dc)); break;
case 4: PUSH(FIOSCALL2(addr,da,db,dc,dd)); break;
}
else if (ios[addr].retsize==4 && (ios[addr].argsize==4||ios[addr].args==0)) switch (ios[addr].args) {
case 0: PUSH2(FIOSCALL4(addr)); break;
case 1: PUSH2(FIOSCALL4(addr,da)); break;
case 2: PUSH2(FIOSCALL4(addr,da,db)); break;
case 3: PUSH2(FIOSCALL4(addr,da,db,dc)); break;
case 4: PUSH2(FIOSCALL4(addr,da,db,dc,dd)); break;
}
if (vmerror) goto onerror; pc++;DISPATCH(); /* fcall */
#endif
