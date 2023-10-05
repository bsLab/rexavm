DEBUG("LIT %xn",EXTENDSIGN((code[pc]&0x3F)<<8)|code[pc+1]);
PUSH((sb2)((EXTENDSIGN(code[pc]&0x3F)<<8)|code[pc+1])); pc+=2;
DISPATCH();
