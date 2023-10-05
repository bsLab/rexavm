STACKCHECK(2);da=POP2();
PUSH2(da<0?-da:da);
pc++;DISPATCH(); /* 2abs */