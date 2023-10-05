ub1 v=(ub1)code[pc+1]; STACKCHECK(1); 
if (v&0x8) STACKTOP(1)=STACKTOP(1)*(v>>4)-(8-v&0x7); 
else STACKTOP(1)=STACKTOP(1)*(v>>4)+(v&0x7);
pc+=2;DISPATCH(); /* MULADD */
