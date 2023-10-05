STACKCHECK(2);addr=(ub2)POP();a=POP();b=a/2+a%2;
STACKCHECK(b);*((sb2*)&code[addr])=a;
for(c=1;c<=b;c++) ((ub2*)&code[addr])[c]=(ub2)POP();
pc++;DISPATCH(); /* s! */