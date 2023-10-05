STACKCHECK(2);a=POP();addr=POP();PUSH(addr+a*2);
pc++;DISPATCH(); /* cell+ */