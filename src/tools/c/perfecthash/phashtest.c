// Builds the VM loop
#include "phash.h"
#include "lookupa.h"
#include "words.h"
int main() {
  int index=0;
  char *word=words[index];
  while (word) {
    int h = phash(word,strlen(word));
    printf("%2d : %s\n",h,word);
    index++;
    word=words[index];
  }
  return 0;
}
