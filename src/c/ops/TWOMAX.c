STACKCHECK(4);da=POP2();db=POP2();
PUSH2(db>da?db:da);
pc++;DISPATCH(); /* 2max */