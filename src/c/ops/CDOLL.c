if ((code[pc+1]&0x7f)==OP_CALL) { a=*(sb2*)(&code[pc+2]); pc+=4;}
else { a=*(sb1*)(&code[pc+2]); pc+=3; };
PUSH(a); DISPATCH(); /* $ */
