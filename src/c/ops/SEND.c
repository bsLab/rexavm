STACKCHECK(2); a=POP(); b=POP();SEND(a,b);
pc++;DISPATCH(); /* send */