addr=*(ub2*)(&code[pc+1]);addr=*(ub2*)(&code[addr]);PUSH(addr);
pc=pc+3;DISPATCH();