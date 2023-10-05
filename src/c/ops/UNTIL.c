STACKCHECK(1);a=POP();
if (a) pc+=3; else pc=(*(ub2*)(&code[pc+1]));
DISPATCH(); /* until */