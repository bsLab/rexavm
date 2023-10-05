STACKCHECK(3);addr=(ub2)POP();da=POP2();
*((sb4*)&code[addr])=da;
pc++;DISPATCH(); /* 2! */