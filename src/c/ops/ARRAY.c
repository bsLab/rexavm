DEBUG("ARRAY BRANCH %dn",(*(sb2*)(&code[pc+1])));
a=*((sb2*)&code[pc+1]); if (a<0) pc=pc+5; else pc=pc+a*2+3;
DISPATCH(); /* array */
