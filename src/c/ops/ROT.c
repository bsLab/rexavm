STACKCHECK(3);c=POP();b=POP();a=POP();
PUSH(b);PUSH(c);PUSH(a);
pc++;DISPATCH(); /* rot */
