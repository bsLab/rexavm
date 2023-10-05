#include "config.h"
#include "standard.h"

ub1 lookupLST (char *word, ub2 len, ub1 *table) {
  ub2 pos = ((ub2 *)table)[(len-1)],
      index = 0;
      
   while (index!=len) {
    // print(index,pos,table[pos],table[pos+1]);
    if (table[pos]==word[index]) {
      index++;
      if (index<len)
        pos=pos+table[pos+1];   // next node slice start position
      else return table[pos+1]; // end node, word index
    } else {
      pos+=2;
      if (table[pos]==255) return 255;  // not found
    }
  }
  return 255; // not found
}



