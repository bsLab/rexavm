STACKCHECK(6);dc=POP2();db=POP2();da=POP2();
PUSH2(db);PUSH2(dc);PUSH2(da);
pc++;DISPATCH(); /* 2rot */
