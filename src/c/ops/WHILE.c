STACKCHECK(1);a=POP();
if (a==0) pc+=3; else pc=(*(ub2*)(&code[pc+1]));
pc++;DISPATCH(); /* while */