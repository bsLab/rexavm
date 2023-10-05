#include "config.h"
#include "standard.h"
#include "compile.h"
#include "dict.h"
#include "stack.h"
#include "vm.h"
#include "vmloop.h"
#include "vmio.h"
#include "vmcode.h"
#include "vmwords.h"

#include <stdio.h>

// Disassemble code and return number of words (tokens, including literals and plus auxilliray values, e.g., array initializers)
ub2 disassemble(ub2 start, ub2 end) {
  sw1 pc=start;
  sw1 addr;
  uw1 a,i;
  uw2 da;
  char *str;
  ub1 *code=CS;
  ub1 indef=0;
  ub2 words=0;
  ub1 afterend=0;
  
  while (pc <= end) {
    ub1 opcode = DECODE(code[pc]);
    if (pc>indef) indef=0;
    if (!afterend) words++;
    switch (opcode) {
      case OP_ARRAY     : addr=*(ub2*)(&code[pc+1]);
                          if (addr<0) {
                            // Non-initialized data space at code segment end
                            printf("%s%0.4x : %s (%d)\n",indef?"  ":"",pc,"array",-addr);
                            pc=pc+5;
                            words++; // size field
                          } else {
                            // Initialized data space following this word
                            printf("%s%0.4x : %s [%d]\n",indef?"  ":"",pc,"array",addr);
                            pc=pc+addr*2+3;
                            words+=(addr);
                          }
                          break;
      case OP_CALL      : addr=*(ub2*)(&code[pc+1]); 
                          printf("%s%0.4x : %s (%0.4x)\n",indef?"  ":"",pc,"CALL",addr); 
                          pc+=3; 
                          break;
      case OP_CDOLL     : addr=pc;
                          if ((code[pc+1]&0x7f)==OP_CALL) { 
                            a=*(sb2*)(&code[pc+2]); pc+=4;
                            printf("%s%0.4x : $ (%0.4x)\n",indef?"  ":"",addr,a);
                          } else { 
                            a=*(sb1*)(&code[pc+2]); pc+=3;
                            printf("%s%0.4x : $ (@%0.2x) ",indef?"  ":"",addr,a&0xff);
                            a=-((sb1)a)-1;
                            for(i=0;i<DICTNAMELENGTH;i++) {
                              if (ios[a].name[i]==0) break;
                              printf("%c",ios[a].name[i]);
                            }
                            printf("\n");
                          };
                          break;
      case OP_CDOTCSTR  : str=(char *)&code[pc+1];
                          printf("%s%0.4x : %s (\"%s\")\n",indef?"  ":"",pc,".\"",str);
                          pc=pc+(strlen(str)+2); break;
      case OP_CPLULOOP  : addr=*(ub2*)(&code[pc+1]);
                          printf("%s%0.4x : %s (%0.4x)\n",indef?"  ":"",pc,"loop+",addr); 
                          pc+=3; break;
      case OP_DEFINE    : addr=*(sb2*)(&code[pc+1]);
                          printf("%s%0.4x : %s (#%0.4x) $%0.4x-$%0.4x\n",indef?"  ":"",pc,"FUNC",addr,pc+3,pc+addr-1);
                          indef=pc+addr-1;
                          pc+=3;
                          words++; // function name 
                          break;
#if DEFINELOCAL==1
      case OP_DEFINECMUL  : addr=*(sb2*)(&code[pc+1]);
                          printf("%s%0.4x : %s (#%0.4x) $%0.4x-$%0.4x\n",indef?"  ":"",pc,"FUNC",addr,pc+3,pc+addr-1);
                          indef=pc+addr-1;
                          pc+=3;
                          words++; // function name  
                          break;
#endif
      case OP_DO        : addr=*(sb2*)(&code[pc+1]);
                          printf("%s%0.4x : %s (!%0.4x)\n",indef?"  ":"",pc,"do",addr,pc+addr);
                          pc+=3; break;
      case OP_FCALL     : addr=code[pc+1];
                          printf("%s%0.4x : %s (@%0.2x) ",indef?"  ":"",pc,"FCALL",addr);
                          a=-((sb1)addr)-1;
                          for(i=0;i<DICTNAMELENGTH;i++) {
                            if (ios[a].name[i]==0) break;
                            printf("%c",ios[a].name[i]);
                          } 
                          printf("\n");
                          pc+=2; break;
      case OP_IF        : addr=*(sb2*)(&code[pc+1]);
                          printf("%s%0.4x : %s ($%0.4x)\n",indef?"  ":"",pc,"if",addr,pc+addr);
                          pc+=3; break;
      case OP_INDIRECT  : addr=*(ub2*)(&code[pc+1]);addr=*(ub2*)(&code[addr]);
                          printf("%s%0.4x : %s (%0.4x)\n",indef?"  ":"",pc,"INDIRECT",addr); 
                          pc+=3; break;
      case OP_MULADD    : a=(ub1)(code[pc+1]);
                          printf("%s%0.4x : %s (*%d+(%d))\n",indef?"  ":"",pc,"MULADD",a>>4,a&0x8?-(8-a&0x7):a&0xf); 
                          pc+=2; break;
      case OP_LOOP      : addr=*(ub2*)(&code[pc+1]);
                          printf("%s%0.4x : %s (%0.4x)\n",indef?"  ":"",pc,"loop",addr); 
                          pc+=3; break;
      case OP_LIT       : a=(ub2)(EXTENDSIGN(code[pc]&0x3F)<<8|code[pc+1]);
                          printf("%s%0.4x : %s (%d)\n",indef?"  ":"",pc,"LIT",a); 
                          pc+=2; break;
      case OP_NOP       : printf("%s%0.4x : %s\n",indef?"  ":"","NOP"); break;
      case OP_SCSTR  :    str=(char *)&code[pc+1];
                          printf("%s%0.4x : %s (\"%s\")\n",indef?"  ":"",pc,"s\"",str);
                          pc=pc+(strlen(str)+2); break;
      case OP_TWOLIT    : da=(ub4)(EXTENDSIGN(code[pc]&0x3F)<<24|code[pc+1]<<16|code[pc+2]<<8|code[pc+3]);
                          printf("%s%0.4x : %s (%d)\n",indef?"  ":"",pc,"2LIT",da); 
                          pc+=4; break;
      case OP_TWOVAR    : addr=pc+1;
                          printf("%s%0.4x : %s $%0.4x\n",indef?"  ":"",pc,"2var",addr);
                          pc+=5;
                          words++; // var name                             
                          break;
      case OP_VAR       : addr=pc+1;
                          printf("%s%0.4x : %s $%0.4x\n",indef?"  ":"",pc,"var",addr);
                          pc+=3;
                          words++; // var name  
                          break;
      default: {
        #ifdef WORDSTRINGS
          const char *opname =  OPSTRTABLE[opcode];
          printf("%s%0.4x : %s\n",indef?"  ":"",pc,opname); pc+=1;
        #else
          printf("%s%0.4x : %0.2x\n",indef?"  ":"",pc,opcode); pc+=1;
        #endif
      }
    }
    if (opcode==OP_END) afterend=1;
    if (afterend) break;
  }
  printf("-------------\n#words=%d\n-------------\n",words);
  return words;
}
