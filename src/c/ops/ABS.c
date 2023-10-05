STACKCHECK(1);a=POP();
PUSH(a<0?-a:a);
pc++;DISPATCH(); /* abs */