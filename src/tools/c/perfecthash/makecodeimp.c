// Builds the code compiler
#include "corehash.h"
#include "lookupa.h"
#include "corewords.h"
#include <string.h>


void makeLabel(char *word,char *wordlabel) {
  int i,j,h = corehash(word,strlen(word));
  j=0;
  for(i=0;i<strlen(word);i++) {
    if (word[i]>='a'&&word[i]<='z') { wordlabel[j]=word[i]-32; j++; }
    else switch (word[i]) {
      case '0' : strcpy(&wordlabel[j],"ZERO");  j+=4; break;
      case '1' : strcpy(&wordlabel[j],"ONE");   j+=3; break;
      case '2' : strcpy(&wordlabel[j],"TWO");   j+=3; break;
      case '3' : strcpy(&wordlabel[j],"THREE"); j+=5; break;
      case '4' : strcpy(&wordlabel[j],"FOUR");  j+=4; break;
      case '5' : strcpy(&wordlabel[j],"FIVE");  j+=4; break;
      case '6' : strcpy(&wordlabel[j],"SIX");   j+=3; break;
      case '7' : strcpy(&wordlabel[j],"SEVEN"); j+=5; break;
      case '8' : strcpy(&wordlabel[j],"EIGHT"); j+=5; break;
      case '9' : strcpy(&wordlabel[j],"NINE");  j+=4; break;
      case '=' : strcpy(&wordlabel[j],"CEQ");    j+=3; break;
      case '<' : strcpy(&wordlabel[j],"CLT");    j+=3; break;
      case '>' : strcpy(&wordlabel[j],"CGT");    j+=3; break;
      case '*' : strcpy(&wordlabel[j],"CMUL");   j+=4; break;
      case '+' : strcpy(&wordlabel[j],"CPLU");   j+=4; break;
      case '-' : strcpy(&wordlabel[j],"CMIN");   j+=4; break;
      case '/' : strcpy(&wordlabel[j],"CQUO");   j+=4; break;
      case '!' : strcpy(&wordlabel[j],"STORE");  j+=5; break;
      case ',' : strcpy(&wordlabel[j],"CCOM");   j+=4; break;
      case ';' : strcpy(&wordlabel[j],"DEFRET"); j+=6; break;
      case '.' : strcpy(&wordlabel[j],"CDOT");   j+=4; break;
      case '@' : strcpy(&wordlabel[j],"FETCH");    j+=5; break;
      case ':' : strcpy(&wordlabel[j],"DEFINE");   j+=6; break;
      case '"' : strcpy(&wordlabel[j],"CSTR");   j+=4; break;
      case '$' : strcpy(&wordlabel[j],"CDOLL");   j+=5; break;
    };
  }
  wordlabel[j]=0;
}

int main() {
  int i,j,index=0;
  char *word;
  char wordlabel[32];
  char sep=' ';
  printf("// Automatically created on %s\n",__DATE__);
  // printf("#include \"phash.h\"\n");
  index=0;
  word=words[index];
  while (word) {
    int h = corehash(word,strlen(word));
    makeLabel(word,wordlabel);
    printf("#define DO_%s {\\\n  \\\n  pc++;DISPATCH();} /* %s */\n",wordlabel,word);
    index++;
    word=words[index];
  }
#if 0
  printf("#define DO_BRANCHZ {\\\n  \\\n  a=POP();if (a==0) pc=(*(ub2*)(&code[pc+1])); else pc+=3;DISPATCH();}\n");
  printf("#define DO_BRANCH {\\\n  \\\n   pc=(*(ub2*)(&code[pc+1]));DISPATCH();}\n");
#endif
  printf("#define DO_CALL {\\\n  \\\n   addr=*(ub2*)(&code[pc+1]);RPUSH(pc+3);pc=addr;DISPATCH();}\n");
  printf("#define DO_FCALL { /* TODO */ }\n");
  printf("#define DO_BRANCH {\\\n  pc=(*(ub2*)(&code[pc+1]));DISPATCH();}\n");
  printf("#define DO_INDIRECT {\\\n  \\\n   addr=*(ub2*)(&code[pc+1]);addr=*(ub2*)(&code[addr]);PUSH(addr);DISPATCH();}\n");
  printf("#define DO_NOP {\\\n  \\\n  pc++;DISPATCH();}\n");
  printf("#define DO_LIT {\\\n  PUSH((ub2)(EXTENDSIGN(code[pc]&0x3F)<<8|code[pc+1])); pc+=2;\\\n  DISPATCH();}\n");
  printf("#define DO_TWOLIT {\\\n  PUSH2((ub4)(EXTENDSIGN(code[pc]&0x3F)<<24|code[pc+1]<<16|code[pc+2]<<8|code[pc+3])); pc+=4;\\\n  DISPATCH();}\n");
  return 0;
}
