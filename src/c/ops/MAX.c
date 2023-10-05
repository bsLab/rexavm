STACKCHECK(2);a=POP();b=POP();PUSH(b>a?b:a);
pc++;DISPATCH(); /* max */