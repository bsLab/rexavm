DEBUG("DEFINE BRANCH %dn",(*(sb2*)(&code[pc+1])));
pc=pc+(*(sb2*)(&code[pc+1]));DISPATCH(); /* :* */
