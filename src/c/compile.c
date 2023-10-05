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
#if LEXER == PHLEXER
#include "vmwords.h"
#include "lookupa.h"
#endif
#if LEXER == LSTLEXER
#include "vmwords.h"
#include "lookupl.h"
#endif
#include "vm.h"
#include "vmcode.h"
#include "dict.h"
#include "stack.h"
#include "vmio.h"
#include "compile.h"
#include <string.h>

// #define ISSEP(c)                (c==' ' || c==0 || c=='\n')
#define ISSEP(c)                (c==' ' || c=='\n')
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

#ifdef WORDSTRINGS
OPSTRTABLEGEN
#endif

static ub1 lastop=0,inop=0,dictcurrent=0;
static ub2 env[ENVDEPTH];
static ub1 envtype[ENVDEPTH];
static ub1 envtop=0;
static ub1 vmallocate=0;
// Upper boundary of locked CS range containing words in global dictionary
sw1 vmcslocked;

// Temporary dictionary LUT for variables and optionally local words
static dict_cell_t lut[LUTSIZE];

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

// Compile one programm code token
// Generates position independent code (TODO PIC)
//   All data and most code referencess are relative
sw1 compileToken(ub1 *code,sw1 position, uw1 *pc) {
  uw1 h,i,n,start,addr;
  sw2 v;
  ub1 cells=0,is_num,sign,len;
  
  char *word;

  /* Compile one sourec code word (token) */
  while (code[position] && ISSEP(code[position])) position++;
  DEBUG("compile pos=%d pc=%d lastop=%x envtop=%d env=%x\n",position,*pc,lastop,envtop,envtop?envtype[envtop-1]:OP_NOP);
  if (code[position]==0) return -1;

  is_num=(ISNUM(code[position]) && (ISNUM(code[position+1]) || ISSEP(code[position+1]) || code[position+1]=='l')) ||
         (code[position]=='-'  && ISNUM(code[position+1]) && 
          (ISNUM(code[position+2]) || ISSEP(code[position+2]) || code[position+1]=='l'));
  if (!is_num) { 
    // var ident
    // array ident #cells
    // : ident .. ;
    // Identifier or command

#if COMMENTS==1
    SKIPCOMMENT(code,position);
#endif
    
    // -1. get identifier
    word=(char *)&code[position];
    start=position;
    while(code[position]!=0 && !ISSEP(code[position])) position++;
    len=position-start;
    if (len>8) len=8;
    DEBUG("$ %d start=%d pos=%d pc=%d\n",len,start,position,*pc);
        
    // 0. Check for pending VAR/ARRAY/DEFINE ops
#if TWOWORDS==1
    if (lastop==OP_VAR||lastop==OP_TWOVAR) {
#else
    if (lastop==OP_VAR) {
#endif
      h=dictadd(lut,LUTSIZE,word,len);
      if (h==NOTFOUND) { vmerror=ENOSPACE; return -position; }
      DEBUG("LUT + %s\n",lut[h].name);
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
        h=dictadd(lut,LUTSIZE,word,len);
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
        h=dictadd(lut,LUTSIZE,word,len);
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
#if DEFINELOCAL==1
   } else if (lastop==OP_DEFINE || lastop==OP_DEFINECMUL) {
      // Insert size after (sb2) ..
      *pc+=2;
      inop=lastop;
      if (lastop==OP_DEFINE) {
        h=dictadd(dictionary,DICTSIZE,word,len);
        if (h==NOTFOUND) {
          vmerror=ENOSPACE;
          return -position;
        }
        dictionary[h].addr=*pc;
        DEBUG("def dict %d pos=%d pc=%d\n",h,position,*pc);
        // after ; we can update the branch and the LUT
        // save dirctionary entry
        dictcurrent=h;      
      } else {
        h=dictadd(lut,LUTSIZE,word,len);
        if (h==NOTFOUND) {
          vmerror=ENOSPACE;
          return -position;
        }
        lut[h].addr=*pc;
        DEBUG("def lut %d pos=%d pc=%d\n",h,position,*pc);
        // after ; we can update the branch and the LUT;
        // save dirctionary entry
        dictcurrent=h;      
      }
      lastop=OP_NOP;
#else
   } else if (lastop==OP_DEFINE) {
      // Insert size after (sb2) ..
      *pc+=2;
      // Register or update word in dictionary
      h=dictadd(dictionary,DICTSIZE,word,len);
      if (h==NOTFOUND) {
        vmerror=ENOSPACE;
        return -position;
      }
      dictionary[h].addr=*pc;
      DEBUG("def %d pos=%d pc=%d\n",h,position,*pc);
      // after ; we can update the branch and the LUT;
      // save dirctionary entry
      dictcurrent=h;
      lastop=OP_NOP;
#endif
    } else {
      // 1. Check core word (highest hit probability)
#if LEXER == PHLEXER
      h=lookupPH(word,len,CoreLexerTable1,CoreLexerTable1Length,CoreLexerTable2,CoreLexerTable2Length);
      if (strncmp(word,OPSTRTABLE[h],len)==0) {
#endif
#if LEXER == LSTLEXER
      h=lookupLST(word,len,CoreLexerTable);
      if (h!=255) { // found word
#endif
        // It is a core word
        DEBUG("OPCODE %x (%x)\n",h,h+0x80);
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
#if DEFINELOCAL==1
            if (inop==OP_DEFINE) {
              // global word
              dictionary[h].size=*pc-dictionary[h].addr-1;
              *(ub2*)&code[dictionary[h].addr-2]=dictionary[h].size+4; // branch to end of word code (pc+size+4)
              DEBUG("DEFRET %d addr=%d size=%d\n",h,dictionary[h].addr,dictionary[h].size); 
              vmcslocked=*pc; 
            } else {
              // local word
              lut[h].size=*pc-lut[h].addr-1;
              *(ub2*)&code[lut[h].addr-2]=lut[h].size+4; // branch to end of word code (pc+size+4)
              lut[h].flags |= FUNCTION;
              DEBUG("DEFRET %d addr=%d size=%d\n",h,dictionary[h].addr,lut[h].size);                
            }
#else
            dictionary[h].size=*pc-dictionary[h].addr-1;
            *(ub2*)&code[dictionary[h].addr-2]=dictionary[h].size+4; // branch to end of word code (pc+size+4)
            DEBUG("DEFRET %d addr=%d size=%d\n",h,dictionary[h].addr,dictionary[h].size);  
#endif
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
      h=dictlookup(dictionary,DICTSIZE,word,len);
      if (h!=NOTFOUND) {
        DEBUG("[%x] DICT CALL %d addr=%d\n",*pc,h,dictionary[h].addr);
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
      h=dictlookup(lut,LUTSIZE,word,len);
      if (h!=NOTFOUND) {
        DEBUG("[%x] LUT var %d addr=%d %c%c%c %c%c%c\n",*pc,h,lut[h].addr,lut[h].name[0],lut[h].name[1],lut[h].name[2],word[0],word[1],word[2]);
        // if it is a variable, push CS address literal
        if (lut[h].flags & FUNCTION) {
          DEBUG("[%x] LUT CALL %d addr=%d\n",*pc,h,dictionary[h].addr);
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
#if DATAWIDTH==16
#if CODESIZE<=8192
          STORELIT(code,*pc,lut[h].addr);
#else if CODESIZE<=16384
          if (lut[h].addr<8192) {
            STORELIT(code,*pc,lut[h].addr);
          } else {
            STORELIT(code,*pc,lut[h].addr/2);
            *pc+=2;
            // MULADD
            EXTEND(code,*pc,2,lut[h].addr%2);
          }
#else
 #error "CS size not supported on 16 Bits VM"
#endif
#endif
        }
        *pc+=2;
        return position;      
      }
      // 4. Check IOS TODO
      //    LUT is either linear table or dictionary (perfect hash/lst?)
      DEBUG("IOS top %d\n",iostop);
      for(h=0;h<iostop;h++) {
        if (strncmp(ios[h].name,word,len)==0 && ios[h].name[len]==0) {
          DEBUG("[%x] IOS %d %c%c%c %c\n",*pc,h,ios[h].name[0],ios[h].name[1],ios[h].name[2],ios[h].type);
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
    DEBUG("LIT %d cells=%d pos=%d pc=%d\n",v,cells,position,*pc);
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
#if 0      
      code[*pc+3] = v&0xff;
      v=v>>8;
      code[*pc+2] = v&0xff;
      v=v>>8;
      code[*pc+1] = v&0xff;
      v=v>>8;
      code[*pc]   = 0x40 + (v&0x3F);
#endif
      *pc+=4;
    }
  }
  return position;
}

// Interruptable (real-time capable) compiler (steps limits number of compiled tokens in one schedule)
// steps>=0 : First or full run
// steps<0  : Following partial runs 
#if COMPILEINCR==1
sb2 compile(ub1 *code, sb2 position, ub2 *pc, sb2 steps) {
#else
sw1 compile(ub1 *code, sw1 position, uw1 *pc) {
#endif
  int i,lastpos;
#if COMPILEINCR==1
  if (steps>=0) {
#endif
    lastpos     = 0;
    lastop      = OP_NOP;
    envtop      = 0;
    vmerror     = EOK;
    vmallocate  = 0;
    vmcslocked	= -1;
    inop        = OP_NOP;
    dictinit(lut,LUTSIZE);
#if COMPILEINCR==1
  }
  if (steps>0) steps=-steps;
#endif
  while (position>=0) {
#if COMPILEINCR==1
    if (steps<0) { steps++; if (steps==0) return position; /* scheduling point reached */ }
#endif
    position=compileToken(code,position,pc);
    if (vmerror!=EOK) break;
    if (position>=0) lastpos=position;
  }
  // compilation failed or finished
  if (vmerror!=EOK) {
    PRINT("Compiler failed: pos=%d pc=%x err=%d\n",-position,*pc,vmerror); return position; 
  } else {
    if ((code[*pc-1]&0x7F)!=OP_END) {
      code[*pc]=0x80|OP_END;
      *pc+=1;
    }
  }
  // Need finally extension of data space at the end of this code frame (non-initialized array data)
  if (vmallocate) for(i=0;i<LUTSIZE;i++) {
#if DICTLINEAR==1
    if (i==dicttop) break;
#endif
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
