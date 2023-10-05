STACKCHECK(2);a=POP();b=POP();PUSH(a==b?1:0);
pc++;DISPATCH(); /* = */