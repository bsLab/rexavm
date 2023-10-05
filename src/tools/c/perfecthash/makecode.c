// Builds the code compiler
#define CODESIZE  4096
#define STACKSIZE 1024
#define FRAMES    16

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
      case '"' : strcpy(&wordlabel[j],"CSTR");     j+=4; break;
      case '$' : strcpy(&wordlabel[j],"CDOLL");   j+=5; break;
    };
  }
  wordlabel[j]=0;
}

int main() {
  int i,j,k,index=0;
  char *word;
  char wordlabel[32];
  char *lut[128];
  char sep=' ';
  int wordmaxlen=0;
  memset(lut,0,128*sizeof(int));
  printf("// Automatically created on %s\n",__DATE__);
  printf("#ifndef __VM_CODE_H\n#define __VM_CODE_H\n");
  // printf("#include \"corehash.h\"\n");
  printf("enum OP_CODES {");
  word=words[index];
  while (word) {
    int h = corehash(word,strlen(word));
    makeLabel(word,wordlabel);
    lut[h]=word;
    if (strlen(word)>wordmaxlen) wordmaxlen=strlen(word);
    printf("%c\n  OP_%s=0x%x       /* %s -> %d */",sep,wordlabel,h,word,h);
    index++;
    word=words[index];
    sep=',';
  }
  // Internal OPs, not externally programmable
  printf(",\n  OP_CALL=0x%x /* prefix op: call word */",index++);
  printf(",\n  OP_FCALL=0x%x /* prefix op: call word */",index++);
  printf(",\n  OP_BRANCH=0x%x /* prefix op: call word */",index++);
  printf(",\n  OP_INDIRECT=0x%x /* prefix op: indirect address resolving */",index++);
  printf(",\n  OP_NOP=0x%x /* nop */",index++);
  printf(",\n  OP_LIT=0x%x /* short literal */",index++);
  printf(",\n  OP_TWOLIT=0x%x /* long literal */",index++);
  printf("\n};\n");
  printf("extern char OPSTRTABLE[][%d];\n",wordmaxlen+1);
  printf("#define OPSTRTABLEGEN char OPSTRTABLE[][%d]={\\\n",wordmaxlen+1);
  index=0;
  for (i=0;i<128;i++) {
    char buf[32];
    k=0;
    if (lut[i]==NULL) { break; }
    else {
      for(j=0;j<strlen(lut[i]);j++) {
        if (lut[i][j]!='"') { buf[k++]=lut[i][j]; }
        else { buf[k++]='\\'; buf[k++]=lut[i][j]; }; 
      }
      buf[k]=0;
      printf("%s\n    \"%s\" /*%d*/",i>0?",\\":"\\",buf,i);
    }
  }
  printf("\\\n  };\n");
  printf("#endif\n");
  return 0;
}
