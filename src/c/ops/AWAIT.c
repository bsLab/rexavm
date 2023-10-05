STACKCHECK(2);addr=POP();a=POP();
if (ios[addr].type!='D') { vmerror=EIO; goto onerror; };
vmevent.v=&DIOSDATADEREF(ub2,addr);
vmevent.c=a;
pc++;goto suspend; /* await */