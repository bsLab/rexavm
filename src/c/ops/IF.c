STACKCHECK(1); a=POP(); if (a==0) pc=(*(ub2*)(&code[pc+1])); else pc+=3;
DISPATCH(); /* if */