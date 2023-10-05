addr=*(ub2*)(&code[pc+1]);
DEBUG("LEAVE %x %xn",addr,*(ub2*)(&code[addr]));
if (DECODE(code[addr-1])==OP_DO) FPOP2();
addr=*(ub2*)(&code[addr]);
pc=addr;
DISPATCH(); /* leave */
