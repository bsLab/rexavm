STACKCHECK(1);addr=(ub2)POP();
PUSH(*((sb2*)&code[addr]));
pc++;DISPATCH(); /* @ */