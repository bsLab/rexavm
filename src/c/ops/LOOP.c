FSTACKCHECK(2);FSTACKTOP(1)+=1;
if (FSTACKTOP(1)<FSTACKTOP(2)) pc=(*(ub2*)(&code[pc+1])); else { FPOP2(); pc+=3; }
DISPATCH(); /* loop */