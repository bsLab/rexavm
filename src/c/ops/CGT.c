STACKCHECK(2);a=POP();b=POP();PUSH(b>a?1:0);
pc++;DISPATCH(); /* > */