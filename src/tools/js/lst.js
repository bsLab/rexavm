/*
  Lexer with Linear Search Tree for word tables (word -> index lookup)
  Lexer table and encoding generator
  Version 1.1.1
*/
var table,
    top,
    tableIndex, 
    partitions,
    wordMap,
    maxlen,
    minChoices,maxChoices,slicesN,avgChoices;

// Create one search slice (one node in the tree)
// words are sorted alphabetically, length is width of word in characters, index the current character position

function init(words) {
  table = [0,0,   // 16 bit
           0,0,
           0,0,
           0,0,
           0,0,
           0,0,
           0,0,
           0,0];
  top   = 16;
  tableIndex = 1; // first word index == 1 (0:not found)
  partitions = [];
  wordMap = [];
  maxlen = 0;
  slicesN=0;
  avgChoices=0;
  for (var i = 0;i<words.length;i++) {
    var len = words[i].length;
    if (!partitions[len-1]) partitions[len-1]=[];
    partitions[len-1].push(words[i]);
    maxlen=len>maxlen?len:maxlen;
  }
}


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
    var start  = top,
        result = sliceA(subp,length,index);
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

function generate (words) {
  var charSum=0;
  init(words);
  words.forEach((w) => { charSum+=w.length });
  for (var i = 0;i<partitions.length;i++) {
    var p = partitions[i];
    if (!p) continue;
    print('  lst table partiiton '+i+' top='+top);
    table[i*2]=top;
    p=p.sort((a,b) => (a<b?-1:1));
    // print(p)
    parse(p,i+1);
  }
  words.forEach((w) => {
    wordMap[lookup(w)]=w;
  });
  return {
    stats : {
      charSum     : charSum,
      minChoices  : minChoices,
      maxChoices  : maxChoices,
      slicesN     : slicesN,
      avgChoices  : avgChoices
    },
    table : table,
    words : words,
    map   : wordMap
  }
}

module.exports = {
  generate  : generate,
  lookup    : lookup,
}
