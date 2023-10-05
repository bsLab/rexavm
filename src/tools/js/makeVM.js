/*
  VM Generator (C)
  Lexer with Linear Search Tree for word tables (word -> index lookup)
  Files generated:
  
   vmcode.h
   vmloop.c
   vmimpl.h
   wordsCore.c
   wordsCore.h
   wordsCore.js
   vmimpl.h
   vmcode.h
   
   and all source/header files from DB
   
  Version 1.5.1
*/
var fs    = require('fs'),
    path  = require('path'),
    db    = JSON.parse(FileEmbedded('rexavm.json','utf8')),
    pre   = Require('compiler'),
    perfecthash = Require('perfecthash'),
    lst   = Require('lst');

print=console.log

process.argv.shift()
process.argv.shift()

var version = "1.5.2",
    wordFile,
    configFile,
    dbFile,
    lang = 'C',
    dbaux,
    outputDir  = '.',
    constants = {
      COMPUTEDGOTO  : 1,
      SWITCHCASE    : 2,
      LSTLEXER      : 1,
      PHLEXER       : 2
    }
    macros  = {
      DEBUG : '(...)',
      PRINT : '(...) printf(__VA_ARGS__)',
      CONSOLE     : '(msg)           printf("%s",msg)',
      CONSOLEFMT  : '(...)           printf(__VA_ARGS__)',
      CONSOLELINE : '(msg)           printf("%s\\n",msg)',
      CONSOLELINEFMT : '(...)        printf(__VA_ARGS__); printf("\\n")',
      CONSOLENL   : '()              printf("\\n")',
      OUT         : '(v)             printf("$%04x",(ub2)v)',
      SEND        : '(dst,v)         printf("$%02x:%04x",dst,(ub2)v)',
      SENDN       : '(dst,n,addr)    {int i;printf("$%02x:%02x:",dst,(ub2)n);for(i=0;i<n;i++) printf("%04x",((ub2*)&CS[addr])[i]);}',
    },
    config     = {
      COMMENTS    : 1,      // Support FORTh comments
      CODESIZE    : 16384,  // CS size (16 Bits: max. 32768)
      COMPILEINCR : 0,      // Incremental compiling    
      DATAWIDTH   : 16,     // Core data width 16 or 32 Bits
      DSP					: 0,
      ENDIANESS   : 'LE',
      ENVDEPTH    : 8,      // Depth of nested control structures (loops)
      DEFINELOCAL : 1,      // Local word definitions
      DICTLINEAR  : 0,      // Linear dicttionary (else hash table)
      DICTSIZE    : 256,    // Main word dirctionary size (rows)
      FSTACKSIZE  : 1024,   // Loop stack sizes (words)
      IOSIZE      : 64,     // IOS table size
      LEXER       : constants.LSTLEXER,   // Lexer algorithm for core words
      LUTSIZE     : 128,    // Local data and function dictionary
      MULTITASKING  : 0,
      MULTITHREADED : 0,
      PROFILER    : "",     // Profiler
      READLINE    : 0,
      RSTACKSIZE  : 1024,   // Return stack size (words)
      STACKSIZE   : 8192,   // Data stack size (words)
      SYSARCH     : 'unix',   // stm32, ...
      SDK         : 'generic',  // Arduino, IAR, ...
      TWOWORDS    : 1,      // Support for double size words
      VMLOOP      : constants.SWITCHCASE, // Main VM loop architecture
      VMRUNSLICE  : 50,     // Maximal number of consecutive VM loop iterations
      WORDSTRINGS : 1,      // Store core word string table (diss)
    };
    
function usage() {
  print('usage: makeVM [-c(onfig) <config.json/h>]')
  print('              [-a(rchitecture) unix|emb|rtl]')
  print('              [-l(ang) C|JS|VHDL] ')
  print('              [-d db.json] [--with-dsp] ')
  print('              [-D NAME=VAL] [-o(utput) <dir>')
  print('              <wordlist.txt> | default')
  print('default: arch=unix, lang=C, out=., db=internal, conf=internal')
  print('')
  var targets = Object.keys(db)
  print('Internal DB targets')
  print('===================')  
  for(var i in targets) {
    print(targets[i]+':');
    var t = db[targets[i]]
    for(var p in t.header) print('  '+p);
    for(var p in t.source) print('  '+p);
    var ops = []
    for(var p in t.ops) ops.push(p);
    print(ops.join(' '));
    if (t.sys) for(var p in t.sys) print ('  sys/'+p);
  }
}

function fail (err) {
  print(err)
  process.exit(-1)
}
function maybeNumber(s) {
  if (!isNaN(Number(s))) return Number(s);
  else return s;
}

for(var i=0;i<process.argv.length;i++) {
  switch (process.argv[i]) {
    case '-c': i++; configFile=process.argv[i];  break;
    case '-d': i++; dbFile=process.argv[i];  break;
    case '-D': i++; var tokens=process.argv[i].split('='); if (tokens.length==2) config[tokens[0]]=maybeNumber(tokens[1]); break;
    case '-a': i++; config.SYARCH=process.argv[i];  break;
    case '-o': i++; outputDir=process.argv[i];  break;
    case '-h': usage(); process.exit(); break;
    case '--with-dsp': config.DSP=1; break;
    case '-t': _test(); break;
    default:
      wordFile=process.argv[i]; break;
  }
}


var fd_out;

function output (line) {
  fs.writeSync(fd_out,line+'\n');
}
function open (file) {
  fd_out = fs.openSync(file,'w')
}
function close () {
  fs.closeSync(fd_out);
}
    

function test () {
  words.forEach((w) => {
    print(w,lookup(w));
  });
}

function escapeC(str) {
  return str.replace(/"/g,'\\"')
}

function sprintf(format,arg1,arg2,arg3,arg4) {
  var i=0,args=[arg1,arg2,arg3,arg4];
  return format.replace(/%([0-9]*)([a-z])/g,function (all,s,m) {
    s=Number(s)
    function pad(str,pre,post) {
      var p=''
      if (!isNaN(s)) for(var i=0;i<s-str.length;i++) p+=(pre||post); 
      return pre?p+str:str+p
    }
    switch (m) {
      case 'x': return pad(args[i++].toString(16),'0');
      case 'd': return pad(String(Math.round(args[i++])),'0');
      case 'f': return pad(String(args[i++]),'0');
      case 's': return pad(String(args[i++]),'',' ');
    }
    return String(args[i++])
  })
}

function makeLabel(word) {
  var wordlabel=[];
  word=word.split('')
  function strcpy (dst,src) { src=src.split(''); for(var i=0;i<src.length;i++) dst.push(src[i]) }
  for(var i=0;i<word.length;i++) {
    if (word[i]>='a'&&word[i]<='z') { wordlabel.push(word[i].toUpperCase()); }
    else switch (word[i]) {
      case '0' : strcpy(wordlabel,"ZERO");      break;
      case '1' : strcpy(wordlabel,"ONE");   break;
      case '2' : strcpy(wordlabel,"TWO");   break;
      case '3' : strcpy(wordlabel,"THREE"); break;
      case '4' : strcpy(wordlabel,"FOUR");  break;
      case '5' : strcpy(wordlabel,"FIVE");  break;
      case '6' : strcpy(wordlabel,"SIX");   break;
      case '7' : strcpy(wordlabel,"SEVEN"); break;
      case '8' : strcpy(wordlabel,"EIGHT"); break;
      case '9' : strcpy(wordlabel,"NINE");  break;
      case '=' : strcpy(wordlabel,"CEQ");   break;
      case '<' : strcpy(wordlabel,"CLT");   break;
      case '>' : strcpy(wordlabel,"CGT");   break;
      case '*' : strcpy(wordlabel,"CMUL");  break;
      case '+' : strcpy(wordlabel,"CPLU");  break;
      case '-' : strcpy(wordlabel,"CMIN");  break;
      case '/' : strcpy(wordlabel,"CQUO");  break;
      case '!' : strcpy(wordlabel,"STORE");  break;
      case ',' : strcpy(wordlabel,"CCOM");   break;
      case ';' : strcpy(wordlabel,"DEFRET"); break;
      case '.' : strcpy(wordlabel,"CDOT");   break;
      case '@' : strcpy(wordlabel,"FETCH");  break;
      case ':' : strcpy(wordlabel,"DEFINE"); break;
      case '"' : strcpy(wordlabel,"CSTR");   break;
      case '$' : strcpy(wordlabel,"CDOLL");  break;
    };
  }
  return wordlabel.join('')
}

/*
 Create LST Lexer Table
 
 ub1 NAMELexerTable[n]= {
   1,2,3,...
 };
*/
function emitLST_C(name,table,wordMap) {
  var index=0;
  [
  '#include "config.h"',
  '#include "standard.h"',
  '#include "vmwords.h"'
  ].forEach(output);
  output('ub1 '+name+'LexerTable['+table.length+']={');
    var line = '  ';
    for(var i=0;i<8;i++) {
      // 16 bit addresses
      if (config.ENDIANESS=='LE') 
        line += ((table[i*2] & 0xFF)+','+(table[i*2]>>8)+',');
      else
        line += ((table[i*2]>>8)+','+(table[i*2] & 0xFF)+',');
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

/*
  Create LST header with:
  1.   ub1 NAMELexerTable[];
  2.   Word string list table (table index is opcode number, starting with index 1)
    const char* NAMEWordList[]={
      NULL,
      "ABC",
      ..
    }
*/
function emitLST_H(name,table,wordMap) {
  output('#ifndef __'+name+'LEXER_H');
  output('#define __'+name+'LEXER_H');
  output('extern ub1 '+name+'LexerTable['+table.length+'];')
  output('extern const char* OPSTRTABLE[];');
  output('#define OPSTRTABLEGEN const char* OPSTRTABLE[]={\\');
  output('  "", \\');
  for(var i in wordMap) {
    var sep = wordMap.length-1==i?'':','
    output('  "'+escapeC(wordMap[i])+'"'+sep+' /* '+i+' */ \\')
  }
  output('};');
  output('#endif');
}

/*
 Create PH Lexer Table
 
 sb1 NAMELexerTable1[n]= {
   1,2,3,-1,-2,...
 };
 ub1 NAMELexerTable2[n]= {
   1,2,3,...
 };
 
*/
function emitPH_C(name,table,wordMap) {
  var index=0;
  [
  '#include "config.h"',
  '#include "standard.h"',
  '#include "vmwords.h"'
  ].forEach(output);
  output('sb1 '+name+'LexerTable1['+table[0].length+']={');
  while(index<table[0].length) {
    var line = '  ';
    for(var i=index;i<Math.min(index+16,table[0].length);i++) {
      var v = table[0][i]||0;
      line += (v+(i<(table[0].length-1)?',':''));  
    }
    index+=16;
    output(line)
  }
  output('};');
  output('ub1 '+name+'LexerTable2['+table[1].length+']={');
  index=0;
  while(index<table[0].length) {
    var line = '  ';
    for(var i=index;i<Math.min(index+16,table[1].length);i++) {
      var v = table[1][i]||0;
      line += (v+(i<(table[1].length-1)?',':''));  
    }
    index+=16;
    output(line)
  }
  output('};');
}

/*
  Create PH header with:
  1.   sb1 NAMELexerTable1[];
  2.   ub1 NAMELexerTable2[]
  3.   Word string list table (table index is opcode number, starting with index 1)
    const char* NAMEWordList[]={
      NULL,
      "ABC",
      ..
    }
*/
function emitPH_H(name,table,wordMap) {
  output('#ifndef __'+name+'LEXER_H');
  output('#define __'+name+'LEXER_H');
  output('sb1 '+name+'LexerTable1['+table[0].length+'];')
  output('ub1 '+name+'LexerTable2['+table[1].length+'];')
  output('extern const char* OPSTRTABLE[];');
  output('#define '+name+'LexerTable1Length '+table[0].length);
  output('#define '+name+'LexerTable2Length '+table[1].length);
  output('#define OPSTRTABLEGEN const char* OPSTRTABLE[]={\\');
  output('  "", \\');
  for(var i in wordMap) {
    var sep = wordMap.length-1==i?'':','
    output('  "'+escapeC(wordMap[i])+'"'+sep+' /* '+i+' */ \\')
  }
  output('};');
  output('#endif');
}

/*
 Create OP code list
  enum OP_CODES {
    OP_ABC = 0xab,
    ..
  };
*/
function emitCODE_H(name,table,wordMap) {
  output('#ifndef __VM_CODE_H');
  output('#define __VM_CODE_H');
  output('enum OP_CODES {');
  for(var i in wordMap) {
    var code = sprintf('0x%x',Number(i));
    var sep = wordMap.length-1==i?'':','
    output('  OP_'+makeLabel(wordMap[i])+'='+code+sep+' // '+wordMap[i]+' = '+i)
  }
  output('};');
  output('#endif');
}

/*
 Create OP code list
 #define DO_ABC {\
  ..\
 }
*/
function emitCODEIMPL_H(name,table,wordMap) {
  output('#ifndef __VM_CODEIMPL_H');
  output('#define __VM_CODEIMPL_H');
  for(var i in wordMap) {
    var op = wordMap[i],
        label = makeLabel(op);
    if (db.C.ops && db.C.ops[label]) {
      output('#define DO_'+label+' { /* '+op+' ['+i+'] */ \\')
      var code = compile(db.C.ops[label],'C',0,1);
      output(code.split('\n').map(function (line) {
        return '  '+line+'\\'
      }).join('\n'))
      output('}');
    } else {
      print('Warning: No ops definition found for '+label+' // '+op);
      output('#define OP_'+label+' { /* '+op+' ['+i+'] */ \\')
      output('}');
    }
  }
  output('#endif');
}

/*
 Create vmloop
*/
function emitLOOP_C(name,table,wordMap) {
  [
  '#include "config.h"',
  '#include "standard.h"',
  '#include "vm.h"',
  '#include "stack.h"',
  '#include "compile.h"',
  '#include "vmcode.h"',
  '#include "vmio.h"',
  '#include "vmimpl.h"',
  '#include "vmloop.h"',
  '#include "vmsys.h"',
  'sw1 vmloop(ub1 *code, uw1 pc, ub2 steps) {',
  '  uw1 addr; sw1 a,b,c,d,e,f,g,h; sw2 da,db,dc,dd,de,df,dg,dh;',
  '  vmstate|=VMRUN;',
  ].forEach(output);
  switch (config.VMLOOP) {
    case constants.SWITCHCASE:
      output('  while(1) switch(DECODE(code[pc])) {');
      if (!wordMap[0]) output('    case 0: DO_NOP;');
      for(var i in wordMap) {
        output('    case '+i+': DO_'+makeLabel(wordMap[i])+';');
      }
      output('    default: pc++;');
      output('  };');
      [
        '  onerror:  vmstate=VMERROR;   return pc;',
        '  suspend:  vmstate=VMSUSPEND; return -pc;',
        '  schedule: return pc;',
        '  end:      vmstate=VMEND;     return pc;'
      ].map(output);
      break;
  }
  output('};');
}

function emitCONFIG_H() {
  output('#ifndef __VM_CONFIG_H');
  output('#define __VM_CONFIG_H');
  [
    "#ifdef __cplusplus",
    "#define EXTERN extern \"C\"",
    "#else",
    "#define EXTERN extern",
    "#endif",  
  ].map(output);
  for(var p in db.C.include) {
    var c = db.C.include[p]
    output('#include <'+c+'>')
  }
  for(var p in constants) {
    var c = constants[p]
    output('#define '+sprintf("%20s %s",p,typeof c=='string'?'"'+c+'"':c))
  }
  for(var p in config) {
    var c = config[p]
    output('#define '+sprintf("%20s %s",p,typeof c=='string'?'"'+c+'"':c))
  }
  for(var p in macros) {
    var c = macros[p].match(/(\([^\)]*\))[ ]*(.*)/);
    output('#define '+sprintf("%20s %s",p+c[1],c[2]||''))
  }
  if (config.DATAWIDTH==16) {
    output('typedef signed short int    sw1;');
    output('typedef unsigned short int  uw1;');
    output('typedef signed long  int    sw2;');
    output('typedef unsigned long  int  uw2;');
  }
  output('#endif');
}

function emitWORDS(table,wordMap) {
  for(var i in wordMap) {
    output(wordMap[i]+' : '+makeLabel(wordMap[i]))
  }
}



function emitLST_JS(name,table,wordMap) {
  output('var WordList=[');
  output('  null');
  for(var i in wordMap) {
    var sep = wordMap.length-1==i?'':','
    output('  "'+escapeC(wordMap[i])+'"'+sep+' // '+i)
  }
  output('];');
}

function emitMK(lang) {
  switch (lang) {
    case 'C':
    	output('CC = gcc')
    	output('CCFLAGS = -O3')
    	output('LDFLAGS = -O3')
    	output('AR = ar')
    	output('ARFLAGS = cr')
    	output('RM = rm -rf')
    	
    	output('OBJ = \\')
    	for (var file in db.C.source) {
    		if (/^lookup/.test(file)) {
    			var ext = config.LEXER==constants.LSTLEXER?/^lookupl/:/^lookupa/;
    			if (!ext.test(file)) {
    				continue;
    			}
    		}
    		output('  '+file.replace(/\.c$/,'.o')+'\\');
    	}
  		if (config.READLINE && db.C.sys.readline) {
  		  output('  readline.o\\');
  		}
    	output('  vmloop.o\\');
    	output('  vmwords.o');
    	output('%.o: %.c')
    	output('\t$(CC) $(CCFLAGS) -o $@ -c $<');
    	output('vm.a:$(OBJ)')
    	output('\t$(AR) $(ARFLAGS) $@ $^') 
  		if (db.C.sys[config.SYSARCH] && db.C.sys[config.SYSARCH].source["main.c"]) {
		  	output('main.o: main.c')
		  	output('\t$(CC) $(CCFLAGS) -o $@ -c $<');
    		output('vm:$(OBJ) main.o')
    		output('\t$(CC) $(LDFLAGS) -o $@ $^')   			
  		}
    	output('clean:')
    	output('\t$(RM) *.o *.a')
      break;
  }
}

function compile(code,lang,strict,nomacros) {
  var opts = {
    macros    : nomacros?{}:Object.assign({},macros),
    constants : {},
    noInclude : true,           // do not include files
    noDefine  : true,           // ignore and preserve definitions in file
    strict    : strict||false,  // evaluate only pre-pro condtionals with defined variables
  }
  Object.assign(opts.constants,constants)
  Object.assign(opts.constants,config)
  if (db[lang]) {
    // macro overrides must be compiled here (code)
    Object.assign(opts.constants,db[lang].sys[config.SYSARCH].macros||{})
  }
  var c = new pre.Compiler(opts)
  var result = c.compile(code);
  if (result instanceof Error) {
    throw result
  }
  return result
}

function setup(lang) {
  console.log('preprocess',lang,config.SYSARCH,config.SDK);
  if (db[lang]) {
    // Object.assign(macros,db[lang].sys[config.SYSARCH].macros||{})
    if (config.SDK && db[lang].sys[config.SYSARCH].constants &&
                      db[lang].sys[config.SYSARCH].constants[config.SDK.toUpperCase()]) {
      // SDK specific configuration header (config overrides)
      var _configH = db[lang].sys[config.SYSARCH].constants[config.SDK.toUpperCase()],
          _opts = {macros:{},constants:{}},
          _c = new pre.Compiler(_opts),
          _result = _c.compile(_configH);
      // console.log(_c.defines)
      for(var d in _c.defines) {
        // console.log(d,_c.defines[d])
        if (_c.defines[d].content) {
          macros[d]='('+_c.defines[d].args.join(',')+') '+
            _c.defines[d].content.map((token,i) => {
              if (_c.defines[d].args[i] &&
                _c.defines[d].args[i]!='...') return token+_c.defines[d].args[i];
              else return token;
            }).join('')
          console.log(d,macros[d])
        }
      }
    }
  }
}

function testCompiler() {
var text = [
'#ifndef GP_GETCWD',
'#  if defined(HAVE_GETCWD)',
'#   define GP_GETCWD(path,len) getcwd (path, len)',
'#  else',
'#   define GP_GETCWD(path,len) getwd (path)',
'#  endif',
'#endif',
].join('\n')
print(compile(text,'C',true))
}


async function main() {
// testCompiler(); return

  if (dbFile) dbaux=JSON.parse(fs.readFileSync(dbFile,'utf8'));
  if (configFile) {
    var text = fs.readFileSync(configFile,'utf8');
    if (/\.json/.test(configFile)) {
      config=Object.assign(config,JSON.parse(text));
      if (config.macros) {
        macros=Object.assign(macros,config.macros);
        delete config.macros;
      }
    } else if (/\.h/.test(configFile)) {
      var c = new pre.Compiler({
        constants:{
        }
      })
      var result = c.compile(text);
      for(var i in c.defines) {
        var def = c.defines[i];
        console.log(i,def)
        if (def.name && def.value) {
          config[def.name]=(!isNaN(Number(def.value))?Number(def.value):def.value);
        } else if (def.args) {
          macros[i]='('+def.args.join(',')+')  '+def.content[0];
          if (def.content.length>1 && def.args[0]!='...') macros[i]+=(def.args.join(',')+def.content[1]);
        }
      }
    }
  }
  
  setup("C")
  
  print(config)
  
  if (config.DATAWIDTH!=16 && config.DATAWIDTH!=32) fail('Datawidth not supprted (16/32)');
  if (['unix','emb','rtl','stm32'].indexOf(config.SYSARCH)==-1) fail('Host architecture not supprted (16/32)');
  
  // if (config.SYSARCH=='unix') config.READLINE=1;
  var words  = wordFile=='default'?db.C.words:fs.readFileSync(wordFile,'utf8').split('\n').filter(s => s),
      lexer;
  switch (config.LEXER) {
    case constants.LSTLEXER:
      lexer  = lst.generate(words);
      print('Linear search table lexer:');
      print('words',words.length,'charSum',lexer.stats.charSum,'table size',lexer.table.length,
            'wordtable (opt)',lexer.stats.charSum+words.length*4);
      print('minChoices',lexer.stats.minChoices,'maxChoices',lexer.stats.maxChoices,
            'slicesN',lexer.stats.slicesN,'avgChoices',lexer.stats.avgChoices/lexer.stats.slicesN);
      break;
    case constants.PHLEXER:
      var dict = {}
      for(var i in words) {
        dict[words[i]]=Number(i)+1;
      }
      var tables = perfecthash.create(dict);
      lexer       = {}
      lexer.table = tables;
      lexer.map   = [];
      lexer.words = words;
      for(var i in words) {
        lexer.map[Number(i)+1]=words[perfecthash.lookup(tables[0],tables[1],words[i])-1];
      }
      lexer.stats = {
        table1:tables[0].length,
        table2:tables[1].length,
        charSum:words.map(s => s.length).reduce((a,b) => (a+b)),
      }
      print('Perfect hash table lexer:');
      print('words',words.length,'charSum',lexer.stats.charSum,'table sizes',
             lexer.stats.table1,lexer.stats.table2,'wordtable',lexer.stats.charSum+words.length*4);
      break;
  } 
  // console.log(lexer)
  open(outputDir+'/'+'words.map');
  emitWORDS(lexer.table,lexer.map);
  close();

  // Internal words
  var wordsAux = [
    'call',
    'fcall',
    'branch',
    'branchz',
    'indirect',
    'muladd',
    'nop',
    'lit',
    '2lit'
  ]
  var index = lexer.map.length;
  for(var i in wordsAux) {
    words.push(wordsAux[i]);
    lexer.map.push(wordsAux[i])
  }
  
  
  switch (config.LEXER) {
    case constants.LSTLEXER:
      open(outputDir+'/'+'vmwords.c');
      emitLST_C('Core',lexer.table,lexer.map);
      close();
      open(outputDir+'/'+'vmwords.h');
      emitLST_H('Core',lexer.table,lexer.map);
      close();
      break;
    case constants.PHLEXER:
      open(outputDir+'/'+'vmwords.c');
      emitPH_C('Core',lexer.table,lexer.map);
      close();
      open(outputDir+'/'+'vmwords.h');
      emitPH_H('Core',lexer.table,lexer.map);
      close();
      break;
      
  }
  open(outputDir+'/'+'vmcode.h');
  emitCODE_H('Core',lexer.table,lexer.map);
  close();
  open(outputDir+'/'+'vmimpl.h');
  emitCODEIMPL_H('Core',lexer.table,lexer.map);
  close();
  open(outputDir+'/'+'vmloop.c');
  emitLOOP_C('Core',lexer.table,lexer.map);
  close();
  open(outputDir+'/'+'vmwords.js');
  emitLST_JS('Core',lexer.table,lexer.map);
  close();
  open(outputDir+'/'+'config.h');
  emitCONFIG_H();
  close();
  open(outputDir+'/'+'makefile');
  emitMK('C');
  close();
  
  
  function printTree(node,indent) {
    function spaces(n) { return Array(n).fill(' ').join('') }
    for(var p in node) {
      print(spaces(indent)+p+
            (node[p] instanceof Array?' ['+node[p].length+']':''));
      if (typeof node[p] == 'object' && !( node[p] instanceof Array))  printTree(node[p],indent+2);      
    }
  }
  print('========= DB =========');
  printTree(db,0);
  
  if (db[lang].sys[config.SYSARCH] && db[lang].sys[config.SYSARCH].source["main.templ.c"]) {
    open(outputDir+'/'+'main.templ.c');
    output(compile(db[lang].sys[config.SYSARCH].source["main.templ.c"],'C'));
    close();  
  } else if (db[lang].sys[config.SYSARCH] && db[lang].sys[config.SYSARCH].source["main.c"]) {
    open(outputDir+'/'+'main.c');
    output(compile(db[lang].sys[config.SYSARCH].source["main.c"],'C'));
    close();  
  }
  if (db.C) {
    for(var p in db.C.header) {
  		if (/^lookup/.test(p)) {
  			var ext = config.LEXER==constants.LSTLEXER?/^lookupl/:/^lookupa/;
  			if (!ext.test(p)) {
  				continue;
  			}
  		}
      open(outputDir+'/'+p);
      //if (p.indexOf('stack.h')!=-1)
      output(compile(db.C.header[p],'C',true));
      //else
      // output(db.C.header[p]);
      close();
    }
    for(var p in db.C.source) {
  		if (/^lookup/.test(p)) {
  			var ext = config.LEXER==constants.LSTLEXER?/^lookupl/:/^lookupa/;
  			if (!ext.test(p)) {
  				continue;
  			}
  		}
      open(outputDir+'/'+p);
      output(compile(db.C.source[p],'C'));
      close();
    }
    if (config.DSP==1 && db.C.dsp) {
      for(var p in db.C.dsp.source) {
        open(outputDir+'/'+p);
        output(compile(db.C.dsp.source[p],'C'));
        close();        
      }
      for(var p in db.C.dsp.header) {
        open(outputDir+'/'+p);
        output(compile(db.C.dsp.header[p],'C'));
        close();        
      }
    }
    if (config.READLINE==1 && db.C.sys && db.C.sys.readline) {
      for(var p in db.C.sys.readline.source) {
        open(outputDir+'/'+p);
        output(compile(db.C.sys.readline.source[p],'C',true));
        close();        
      }
      for(var p in db.C.sys.readline.header) {
        open(outputDir+'/'+p);
        output(compile(db.C.sys.readline.header[p],'C',true));
        close();        
      }
    }
  }
  // words.push('test');
  // test();
}

if (wordFile) main().catch((err) => { console.log(err); process.exit(-1) }); else usage();

