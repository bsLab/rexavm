STACKCHECK(2);addr=(ub2)POP();a=POP();
*((sb2*)&code[addr])=a;
pc++;DISPATCH(); /* ! */