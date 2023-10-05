STACKCHECK(4);da=POP2();db=POP2();
PUSH(da==db?1:0);
pc++;DISPATCH(); /* 2= */