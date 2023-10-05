addr=*(ub2*)(&code[pc+1]);
DEBUG("CALL %dn",a);
RPUSH(pc+3);
pc=addr;DISPATCH(); /* call */
