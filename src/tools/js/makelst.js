/*
  Lexer: Linear Search Tree for word tables (word -> index lookup)
*/
var fs = require('fs');
print=console.log

process.argv.shift()
process.argv.shift()

var wordFile   = 'wordlist.txt',
    outputFile = 'lexer',
    name       = '';
if (process.argv.length<1) {
  print('usage: makelst.js -o <output.c> -n <name> <wordlist.txt>')
  process.exit()
}
for(var i=0;i<process.argv.length;i++) {
  switch (process.argv[i]) {
    case '-o': i++; outputFile=process.argv[i];  break;
    case '-n': i++; name=process.argv[i]; break;
    default:
      wordFile=process.argv[i]; break;
  }
}
if (/\..+$/.test(outputFile)) outputFile=outputFile.replace(/\..+$/,'');
var fd_out,
    outputFileC = outputFile+'.c',
    outputFileH = outputFile+'.h';

function output (line) {
  fs.writeSync(fd_out,line+'\n');
}
function open (file) {
  fd_out = fs.openSync(file,'w')
}
function close () {
  fs.closeSync(fd_out);
}

var words    = fs.readFileSync(wordFile,'utf8').split('\n').filter(s => s),
    charSum  = 0;
    
words.forEach((w) => { charSum+=w.length });
var table = [0,0,   // 16 bit
             0,0,
             0,0,
             0,0,
             0,0,
             0,0,
             0,0,
             0,0],
    top   = 16,
    tableIndex = 1, // first word index == 1 (0:not found)
    partitions = [],
    maxlen = 0;
for (var i = 0;i<words.length;i++) {
  var len = words[i].length;
  if (!partitions[len-1]) partitions[len-1]=[];
  partitions[len-1].push(words[i]);
  maxlen=len>maxlen?len:maxlen;
}
// Create one search slice (one node in the tree)
// words are sorted alphabetically, length is width of word in characters, index the current character position
var minChoices,maxChoices,slicesN=0,avgChoices=0;

function sliceA(words,length,index) {
  var current = '',
      tokens = [];
  if (index<(length-1)) {
    // inner node
    for (var i=0;i<words.length;i++) {
      if (words[i].charAt(index)!=current) {
        current = words[i].charAt(index);
        tokens.push(current);
        table[top]=current; top++;
        table[top]=0;       top++; // adjusted later
      }
    }
  } else {
     // end node, returns word index
     for (var i=0;i<words.length;i++) {
      if (words[i].charAt(index)!=current) {
        current = words[i].charAt(index);
        tokens.push(current);
        table[top]=current; top++;
        table[top]=tableIndex++; top++; 
      }
    }
  }
  table[top]=255; top++; // Not found
  return {top:top,tokens:tokens};
}

function parse(words,length) {
  function generate(index, prefix) {
    var subp = prefix?words.filter((w) => {
      return prefix==w.slice(0,prefix.length)
    }):words;
    var start = top,
        result=sliceA(subp,length,index);
    if (index<(length-1)) {
      if (!minChoices) minChoices=result.tokens.length;
      if (!maxChoices) maxChoices=result.tokens.length;
      minChoices=Math.min(result.tokens.length,minChoices);
      maxChoices=Math.max(result.tokens.length,maxChoices);
      avgChoices+=result.tokens.length;
      slicesN++;
      result.tokens.forEach((token,tokenIndex) => {
        var _prefix=prefix+token;
        var _start = top,result = generate(index+1,_prefix);
        // last node slice
        var _index = start+tokenIndex*2+1;
        table[_index]=_start-_index+1;
        if (table[_index]>255) print('Warning (Error): relative jump > 255',length,prefix,token);
      });
    } else {
      // done
      minChoices=Math.min(result.tokens.length,minChoices);
      maxChoices=Math.max(result.tokens.length,maxChoices);
      avgChoices+=result.tokens.length;
    }
  }
  generate(0,'');  
}
function lookup (word) {
  var len = word.length,
      pos = table[(len-1)*2],
      index = 0;
  while (index!=len) {
    // print(index,pos,table[pos],table[pos+1]);
    if (table[pos]==word.charAt(index)) {
      index++;
      if (index<len)
        pos=pos+table[pos+1];   // next node slice start position
      else return table[pos+1]; // end node, word index
    } else {
      pos+=2;
      if (table[pos]==255) return -1;  // not found
    }
  }
  return -1;
}
function test () {
  words.forEach((w) => {
    print(w,lookup(w));
  });
}
function escapeC(str) {
  return str.replace(/"/g,'\\"')
}
function emitLST_C(name) {
  var index=0;
  output('ub1 '+name+'LexerTable['+table.length+']={');
    var line = '  ';
    for(var i=0;i<8;i++) {
      // 16 bit addresses
      line += ((table[i*2] & 0xFF)+','+(table[i*2]>>8)+',');
    }
    output(line);
    index=16;
    while(index<table.length) {
      var line = '  ';
      for(var i=index;i<Math.min(index+16,table.length);i++) {
        var byte = table[i];
        line += ((typeof byte=='number'?byte:byte.charCodeAt(0))+(i<(table.length-1)?',':''));       
      }
      output(line);
      index+=16;
    }
  output('};');
}
function emitLST_H(name) {
  output('ub1 '+name+'LexerTable['+table.length+'];')
  var wordListEnc = []
  words.forEach((w) => {
    wordListEnc[lookup(w)]=w;
  });
  output("#ifdef WORDLIST");
  output('const char* '+name+'WordList[]={');
  output('  NULL');
  for(var i in wordListEnc) {
    output('  , "'+escapeC(wordListEnc[i])+'" // '+i)
  }
  output('};');
  output('#endif /* WORDLIST */');
}
function emitLST_JS(name) {
  output('var WordList={');
  output('  null');
  for(var i in wordListEnc) {
    output('  , "'+escapeC(wordListEnc[i])+'" // '+i)
  }
  output('};');
}
function main() {
  for (var i = 0;i<partitions.length;i++) {
    var p = partitions[i];
    if (!p) continue;
    table[i*2]=top;
    p=p.sort((a,b) => (a<b?-1:1));
    // print(p)
    parse(p,i+1);
  }
  print('words',words.length,'charSum',charSum,'table',table.length);
  print('minChoices',minChoices,'maxChoices',maxChoices,'slicesN',slicesN,'avgChoices',avgChoices/slicesN);
  
  open(outputFileC);
  emitLST_C(name);
  close();
  open(outputFileH);
  emitLST_H(name);
  close();
  open(outputFileJS);
  emitLST_JS(name);
  close();
  
  
  // words.push('test');
  // test();
}

main()
