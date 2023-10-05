STACKCHECK(1);addr=(ub2)POP();
PUSH2(*((sb4*)&code[addr]));
pc++;DISPATCH(); /* 2@ */