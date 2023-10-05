/* 

  Text-to-bytecode compiler 
  
  Ver. 1.3.4

  Byetcode compilation is processed in-place! Assumption: text code position is always higher than bytecode pc 
  bytecode Format
  
  [CC|6]               Instruction
  [CC|6][8]            Short Literal
  [CC|6][8][8][8]      Long  Literal
  
  CC  
  --
  00 Short 14 Bit constant value
  01 Long 30 Bit constant value
  10 Short number command
  11 Long number command
  
  Run: while (pos>=0) pos=compile(code,pos,&pc);
  
*/


#include "config.h"
#include "standard.h"
#include "vmwords.h"
#include "lookupl.h"
#include "vm.h"
#include "vmcode.h"
#include "dict.h"
#include "stack.h"
#include "vmio.h"
#include "compile.h"
#include <string.h>

#define ISSEP(c)                (c==' ' || c==0 || c=='\n')
#define ISNUM(c)                (c>='0' && c<='9')
#define SKIPSEP(code,position)  while(ISSEP(code[position])) position++
#define SKIPSEPCOMMENT(code,position)  { \
  ub1 incomm=0;\
  while(ISSEP(code[position]) || code[position]=='(' || incomm) {\
   if (incomm && code[position]==')') incomm=0;\
   else if (!incomm && code[position]=='(') incomm=1;\
   position++;\
}}
#define SKIPCOMMENT(code,position)  if (code[position]=='(') { while(code[position] && code[position]!=')') position++; return position+1; }

OPSTRTABLEGEN

static ub1 lastop=0,inop=0,dictcurrent=0;
static ub2 env[8];
static ub1 envtype[8];
static ub1 envtop=0;
static ub1 vmallocate=0;
// Upper boundary of locked CS range containing words in global dictionary
sw1 vmcslocked;

// Temporary dictionary LUT for variables and optionally local words
static dict_cell_t lut[64];

static void printWord(char *word,ub1 length) {
  ub1 i;
  for(i=0;i<length;i++) {
    if (!word[i]) break;
    printf("%c",word[i]);
  }
}
static sw2 compileNumber(ub1 *code,sw1 *position) {
  sw2 v=0; 
  ub1 s=0;
  if (code[*position]=='-') { s=1; *position+=1; };
  while (ISNUM(code[*position])) {
    v*=10;
    v+=(code[*position]-48);
    *position+=1; 
  }
  return s?-v:v;
}

// Generates position independent code (TODO PIC)
// All data and most code referencess are relative
sw1 compileToken(ub1 *code,sw1 position, uw1 *pc) {
  uw1 h,i,n,start,addr;
  sw2 v;
  ub1 cells=0,is_num,sign,len;
  
  char *word;

  /* Compile one sourec code word (token) */
  while (code[position] && ISSEP(code[position])) position++;
  ;
  if (code[position]==0) return -1;

  is_num=(ISNUM(code[position]) && (ISNUM(code[position+1]) || ISSEP(code[position+1]) || code[position+1]=='l')) ||
         (code[position]=='-'  && ISNUM(code[position+1]) && 
          (ISNUM(code[position+2]) || ISSEP(code[position+2]) || code[position+1]=='l'));
  if (!is_num) { 
    // var ident
    // array ident #cells
    // : ident .. ;
    // Identifier or command

    SKIPCOMMENT(code,position);
    
    // -1. get identifier
    word=(char *)&code[position];
    start=position;
    while(code[position]!=0 && !ISSEP(code[position])) position++;
    len=position-start;
    if (len>8) len=8;
    ;
        
    // 0. Check for pending VAR/ARRAY/DEFINE ops
    if (lastop==OP_VAR||lastop==OP_TWOVAR) {
      h=dictadd(lut,64,word,len);
      if (h==NOTFOUND) { vmerror=ENOSPACE; return -position; }
      ;
      len=lastop==OP_VAR?1:2;
      memset(&code[*pc],0,len*2);
      lut[h].addr=*pc; // start of data
      lut[h].size=len*2;
      // allocate size in-place
      *pc+=(len*2);
      lastop=OP_NOP;
    } else if (lastop==OP_ARRAY) {
      // array <ident> <numcells>
      // Non-initialized arrays are appended at the end of the code area (later)
      // => <array> <-size> <addr> ..
      // Immediately initialized arrays are placed here
      // array <ident> { <value1> <value2> ... <valuen> }
      // => <array> <size> <d1> <d2> .. <dsize> ..
      SKIPSEP(code,position);
      if (code[position]=='{') {
        position++;
        SKIPSEP(code,position);
        // reserve space for length field
        *pc+=2;
        h=dictadd(lut,64,word,len);
        if (h==NOTFOUND) { vmerror=ENOSPACE; return -position; }
        // data start, direct address
        lut[h].addr=*pc;  
        // get values, count length
        n=0;
        while (code[position] && code[position]!='}') {
          v=compileNumber(code,&position);
          // store value (single word for now)
          STORESB2(code,*pc,v); *pc+=2;
          SKIPSEPCOMMENT(code,position);
          n++;
        }
        if (code[position]!='}') {
          vmerror=ESYNTAX;
          return -position;
        }
        position++;
        lut[h].size=n*2;  // nothing to be allocated at the end, direct address!
        lut[h].flags|=ARRAY;
        lastop=OP_NOP;
        // Size of array in word cell units
        STORESB2(code,lut[h].addr-2,n);
      } else if (ISNUM(code[position])) {
        // reserve space for length field, here negative to indicate no data follows
        *pc+=2;
        // but the data address is not known in advance! we need indirect addresing
        h=dictadd(lut,64,word,len);
        if (h==NOTFOUND) { vmerror=ENOSPACE; return -position; }
        lut[h].addr=*pc; // start of address field (filled later)
        STORESB2(code,*pc,0);
        *pc+=2;
        // get number of cells
        v=compileNumber(code,&position);
        // Allocate size cells later at the end of code
        lut[h].size=v*2;  
        lut[h].flags|=(ALLOCATE|ARRAY|INDIRECT);

        vmallocate++;

        // Store size field (in cell units)
        STORESB2(code,lut[h].addr-2,-v);
        lastop=OP_NOP;
        // indirect addressing
        // insert size after (sb2) .. 
      } else {
        vmerror=ESYNTAX;
        return -position;
      }
      lastop=OP_NOP;
   } else if (lastop==OP_DEFINE || lastop==OP_DEFINECMUL) {
      // Insert size after (sb2) ..
      *pc+=2;
      inop=lastop;
      if (lastop==OP_DEFINE) {
        h=dictadd(dictionary,256,word,len);
        if (h==NOTFOUND) {
          vmerror=ENOSPACE;
          return -position;
        }
        dictionary[h].addr=*pc;
        ;
        // after ; we can update the branch and the LUT
        // save dirctionary entry
        dictcurrent=h;      
      } else {
        h=dictadd(lut,64,word,len);
        if (h==NOTFOUND) {
          vmerror=ENOSPACE;
          return -position;
        }
        lut[h].addr=*pc;
        ;
        // after ; we can update the branch and the LUT;
        // save dirctionary entry
        dictcurrent=h;      
      }
      lastop=OP_NOP;
    } else {
      // 1. Check core word (highest hit probability)
      h=lookupLST(word,len,CoreLexerTable);
      if (h!=255) { // found word
        // It is a core word
        ;
        code[*pc]=h+0x80; *pc+=1;
        lastop=h;
        // Handle control ops, the most complex compiler logic
        switch (h) {
          case OP_SCSTR:
          case OP_CDOTCSTR:
            // find terminating "
            position++;
            while (code[position]!=0 && code[position]!='"') {
              code[*pc]=code[position];
              *pc+=1;
              position++;
            }
            if (code[position]!='"') { vmerror=ESYNTAX; return -position; };
            position++;
            code[*pc-1]=0;
            break;
          case OP_DEFRET:
            // dictcurrent holds last dictionary row index stored by DEFINE; update size field of DEFINE op
            h=dictcurrent;
            if (inop==OP_DEFINE) {
              // global word
              dictionary[h].size=*pc-dictionary[h].addr-1;
              *(ub2*)&code[dictionary[h].addr-2]=dictionary[h].size+4; // branch to end of word code (pc+size+4)
              ; 
              vmcslocked=*pc; 
            } else {
              // local word
              lut[h].size=*pc-lut[h].addr-1;
              *(ub2*)&code[lut[h].addr-2]=lut[h].size+4; // branch to end of word code (pc+size+4)
              lut[h].flags |= FUNCTION;
              ;                
            }
            inop=OP_NOP; 
            break;
          case OP_IF:
            env[envtop]=*pc;
            envtype[envtop]=h;
            envtop++;
            // reserve branch address 
            *pc+=2;
            break;
          case OP_ELSE:
            if (envtop==0 || envtype[envtop-1]!=OP_IF) { vmerror=ESYNTAXENV; return -position; };
            addr=env[envtop-1];
            // remember else for control block end (ENDIF)
            envtype[envtop-1]=h;
            env[envtop-1]=*pc;
            // reserve branch address 
            *pc+=2;
            // update IF branch address
            *((ub2*)&code[addr])=*pc;  // Code starts after branch address
            break;
          case OP_ENDIF:
            if (envtop==0 || (envtype[envtop-1]!=OP_IF && envtype[envtop-1]!=OP_ELSE)) { vmerror=ESYNTAXENV; return -position; };
            envtop--;
            *pc-=1;  // virtual op
            // update IF branch address
            *((ub2*)&code[env[envtop]])=*pc;
            break;
          case OP_DO:
            // DS: b a -- => FS: -- b a
            // save loop end address immed. for leave
            *pc+=2;
            env[envtop]=*pc;
            envtype[envtop]=h;
            envtop++;
            break;
          case OP_LOOP:
          case OP_CPLULOOP:
            if (envtop==0 || envtype[envtop-1]!=OP_DO) { vmerror=ESYNTAXENV; return -position; };
            envtop--;
            // FS b a --
            // increements a; branches to DO if a<b
            // set LOOP branch address to DO immed.
            *((ub2*)&code[*pc])=env[envtop];
            *pc+=2;
            // update DO branch to end address, too
            *((ub2*)&code[env[envtop]-2])=*pc;
            break;
          case OP_BEGIN:
            // save loop end address immed. for leaves
            *pc+=2;
            env[envtop]=*pc;
            envtype[envtop]=h;
            envtop++;
            break;
          case OP_AGAIN:
          case OP_UNTIL:
            if (envtop==0 || envtype[envtop-1]!=OP_BEGIN) { 
              vmerror=ESYNTAXENV; 
              return -position; 
            };
            // unconditional loop
            // set AGAIN branch address to BEGIN immed.
            *((ub2*)&code[*pc])=env[envtop-1];
            *pc+=2;
            // update BEGIN branch to end address, too
            *((ub2*)&code[env[envtop-1]-2])=*pc;
            break;
          case OP_LEAVE:
            // find next OP_BEGIN/OP_DO
            i=envtop;
            while (i && envtype[i-1]!=OP_BEGIN && envtype[i-1]!=OP_DO) i--;
            if (i==0) { 
              vmerror=ESYNTAXENV; 
              return -position; 
            };
            // indirect branching, loop end address is later saved here
            *((ub2*)&code[*pc])=env[i-1]-2;
            // reserve branch address            
            *pc+=2;
            break;
        }
        return position;
      }
      // 2. Check global code word dictionary
      h=dictlookup(dictionary,256,word,len);
      if (h!=NOTFOUND) {
        ;
        // add call op first (prefix op)
        code[*pc]=OP_CALL+0x80;
        // TODO Relative branch?
        // TODO indirect addressing (always lookup dict on call)?
        // *(sb2*)(&code[*pc+1])=dictionary[h].addr-*pc; 
        *(ub2*)(&code[*pc+1])=dictionary[h].addr;
        *pc+=3;
        if (len<3) position++; // skip white space if len < 3 TODO fails if this was the last word in code!!!
        return position;
      }
      // 3. Check local LUT (data/word?)
      h=dictlookup(lut,64,word,len);
      if (h!=NOTFOUND) {
        ;
        // if it is a variable, push CS address literal
        if (lut[h].flags & FUNCTION) {
          ;
          // add call op first (prefix op)
          code[*pc]=OP_CALL+0x80;
          // TODO Relative branch?
          // TODO indirect addressing (always lookup dict on call)?
          // *(sb2*)(&code[*pc+1])=dictionary[h].addr-*pc; 
          *(ub2*)(&code[*pc+1])=lut[h].addr;
          *pc+=3;
          if (len<3) position++; // skip white space if len < 3 TODO fails if this was the last word in code!!!
          return position;          
        } else if (lut[h].flags & INDIRECT) {
          // unintialized arrays
          // needs indirect address resolution op
          // address offset is positive (forward)
          code[*pc]=OP_INDIRECT+0x80; *pc+=1;
          STOREUB2(code,*pc,lut[h].addr);
        } else {
          // Scalar variable or initialized array
          // LIT is limited to value +8191!
          // Address value literal
          STORELIT(code,*pc,lut[h].addr);
        }
        *pc+=2;
        return position;      
      }
      // 4. Check IOS TODO
      //    LUT is either linear table or dictionary (perfect hash/lst?)
      ;
      for(h=0;h<iostop;h++) {
        if (strncmp(ios[h].name,word,len)==0 && ios[h].name[len]==0) {
          ;
          if (ios[h].type=='F') {
            // fcall
            code[*pc]=OP_FCALL+0x80;*pc+=1;
            // -index
            code[*pc]=(ub1)-(h+1);*pc+=1;
          } else if (ios[h].type=='D') {
            // push index (negative value to distiguish from CS addresses)
            STORELIT(code,*pc,-(h+1));
            *pc+=2;
          }
          return position;
        }
      }
      printWord(word,8); printf("\n");
      vmerror=ENOTFOUND;
      return -position;            
    }
  } else {
    // number
    // v=0;
    lastop=OP_NOP;
    v=compileNumber(code,&position);
    if (code[position]=='l')  {
      position++;
      cells=2;
    } else cells=1;
    if (!ISSEP(code[position])) { vmerror=ESYNTAXNUM; return -position; };
    position++; // one digit numbers can overwrite following separator; skip 
    ;
    if (cells==1) {
      /* single number word */
      /* MSB LSB byte order!!! */
      if (v>8191 || v<-8192) {
        if (v>16383 || v<-16384) {
          STORELIT(code,*pc,v/4);
          *pc+=2;
          EXTEND(code,*pc,4,v%4);
        } else {
          STORELIT(code,*pc,v/2);
          *pc+=2;
          EXTEND(code,*pc,2,v%2);
        }
      } else {
        STORELIT(code,*pc,v);
      }
      *pc+=2;
    } else {
      /* double number word */
      /* MSB .. LSB byte order!!! */
      STORELIT2(code,*pc,v);
      *pc+=4;
    }
  }
  return position;
}

// Interruptable (real-time capable) compiler (steps limits number of compiled tokens in one schedule)
// steps>=0 : First or full run
// steps<0  : Following partial runs 
sw1 compile(ub1 *code, sw1 position, uw1 *pc) {
  int i,lastpos;
    lastpos     = 0;
    lastop      = OP_NOP;
    envtop      = 0;
    vmerror     = EOK;
    vmallocate  = 0;
    vmcslocked	= -1;
    inop        = OP_NOP;
    dictinit(lut,64);
  while (position>=0) {
    position=compileToken(code,position,pc);
    if (vmerror!=EOK) break;
    if (position>=0) lastpos=position;
  }
  // compilation failed or finished
  if (vmerror!=EOK) {
    { char buf[100]; sprintf(buf,"Compiler failed: pos=%d pc=%x err=%d\n",-position,*pc,vmerror); serialPrint(buf) }; return position; 
  } else {
    if ((code[*pc-1]&0x7F)!=OP_END) {
      code[*pc]=0x80|OP_END;
      *pc+=1;
    }
  }
  // Need finally extension of data space at the end of this code frame (non-initialized array data)
  if (vmallocate) for(i=0;i<64;i++) {
    if (!vmallocate) break;
    if (lut[i].flags & ALLOCATE) {
      lut[i].flags &= ~ALLOCATE;
      // head: array size in word units
      STORESB2(code,*pc,lut[i].size/2);
      *pc+=2;
      memset(&code[*pc],0,lut[i].size);
      if (lut[i].flags & ARRAY) {
        // update (indirect) data address field
        STORESB2(code,lut[i].addr,*pc);
      }
      *pc+=lut[i].size;
      vmallocate--;
    }
  }
  return position;
}


