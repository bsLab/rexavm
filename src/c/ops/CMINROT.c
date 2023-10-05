STACKCHECK(3);c=POP();b=POP();a=POP();
PUSH(c);PUSH(a);PUSH(b);
pc++;DISPATCH(); /* -rot */
