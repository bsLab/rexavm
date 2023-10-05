STACKCHECK(1);addr=(ub2)POP();a=*((sb2*)&code[addr]);b=a/2+a%2;
for(c=b;c>0;c--) PUSH(((sb2*)&code[addr])[c]); PUSH(a);
pc++;DISPATCH(); /* s@ */
