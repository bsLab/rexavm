DEBUG("TWOLIT %xn",EXTENDSIGN((code[pc]&0x3F)<<24)|(code[pc+1]<<16)|(code[pc+2]<<8)|code[pc+3]);
PUSH2((ub4)(EXTENDSIGN((code[pc]&0x3F)<<24)|(code[pc+1]<<16)|(code[pc+2]<<8)|code[pc+3])); pc+=4;
DISPATCH();
