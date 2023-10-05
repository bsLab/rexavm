STACKCHECK(1);a=POP();b=POP();PUSH(b%a);
pc++;DISPATCH(); /* mod */