var fs = require('fs'),
    path = require('path');
    
var SRC = '../../src';
print=console.log

var C = {
  path : SRC+'/c',
  header : [
    'compile.h',
    'dict.h',
    'diss.h',
    'lookupa.h',
    'lookupl.h',
    'stack.h',
    'standard.h',
    'vm.h',
    'vmio.h',
    'vmloop.h',
    'vmsys.h'
  ],
  source : [
    'compile.c',
    'dict.c',
    'diss.c',
    'lookupa.c',
    'lookupl.c',
    'stack.c',
    'vm.c',
    'vmio.c',
    'vmsys.c'  
  ],
  words : 'config/wordlist.txt',
  ops : 'ops',
  sys : 'sys',
  dsp : 'dsp',
  include: [
    'sys/time.h'
  ]
}

function createC (file) {
  var S={}, H = {}, O={}, Y={}, D={source:{},header:{}}
  print('C target:')
  for(var i in C.header) {
    var name = C.header[i];
    print('  Header '+name)
    H[name]=fs.readFileSync(C.path+'/'+name,'utf8');
  }
  C.header=H;
  for(var i in C.source) {
    var name = C.source[i];
    print('  Source '+name)
    S[name]=fs.readFileSync(C.path+'/'+name,'utf8');
  }
  C.source=S;
  var ops = C.ops;
  C.ops=fs.readdirSync(C.path+'/'+ops);
  for(var i in C.ops) {
    var name = C.ops[i];
    if (/^[A-Z0-9]+\.c$/.test(name)) {
      print('  Macro '+name)
      O[name.replace(/\.c$/,'')]=fs.readFileSync(C.path+'/'+ops+'/'+name,'utf8').replace(/\n$/,'');
    }
  }
  C.ops=O;
  // Macros!
  var sys = C.sys;
  C.sys=fs.readdirSync(C.path+'/'+sys);
  for(var i in C.sys) {
    var name = C.sys[i];
    if (/\.c$/.test(name)) {
      print('  '+name)
      Y[name.replace(/\.c$/,'')]=fs.readFileSync(C.path+'/'+sys+'/'+name,'utf8').replace(/\n$/,'');
    } else {
      // sysarch directory
      var arch = name,
          dir = fs.readdirSync(C.path+'/'+sys+'/'+arch);
      Y[arch]={macros:{},constants:{},source:{},header:{}}
      for(var i in dir) {
        var path = C.path+'/'+sys+'/'+arch+'/'+dir[i];
        if (/^[A-Z0-9]+\.c$/.test(dir[i])) {
          print('  Macro '+arch+'/'+dir[i])
          Y[arch].macros[dir[i].replace(/\.c$/,'')]=fs.readFileSync(path,'utf8').replace(/\n$/,'');
        }
        else if (/^[A-Z0-9]+\.h$/.test(dir[i])) {
          print('  Constants '+arch+'/'+dir[i])
          Y[arch].constants[dir[i].replace(/\.h$/,'')]=fs.readFileSync(path,'utf8').replace(/\n$/,'');
        }
        else if (/\.c$/.test(dir[i])) {
          print('  Source '+arch+'/'+dir[i])
          Y[arch].source[dir[i]]=fs.readFileSync(path,'utf8').replace(/\n$/,'');
        }
        else if (/\.h$/.test(dir[i])) {
          print('  Header '+arch+'/'+dir[i])
          Y[arch].header[dir[i]]=fs.readFileSync(path,'utf8').replace(/\n$/,'');
        }
      }
    }
  }
  C.sys=Y;
  if (C.words) {
    print('  Words '+C.words)
    C.words = fs.readFileSync(SRC+'/'+C.words,'utf8').split('\n').filter(s=>s);
  }
  var dsp = C.dsp;
  C.dsp=fs.readdirSync(C.path+'/'+dsp);
  for(var i in C.dsp) {
    var name = C.dsp[i];
    if (/\.c$/.test(name)) {
      print('  Source '+name)
      D.source[name]=fs.readFileSync(C.path+'/'+dsp+'/'+name,'utf8').replace(/\n$/,'');
    }
    if (/\.h$/.test(name)) {
      print('  Header '+name)
      D.header[name]=fs.readFileSync(C.path+'/'+dsp+'/'+name,'utf8').replace(/\n$/,'');
    }
  }
  C.dsp=D;
  var db = {
    C:C
  }
  fs.writeFileSync(file,JSON.stringify(db),'utf8');
}

createC('rexavm.json')

