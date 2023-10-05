STACKCHECK(4);da=POP2();db=POP2();
PUSH(db>da?1:0);
pc++;DISPATCH(); /* 2> */