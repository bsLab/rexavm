var fs = require('fs')

var H = fs.readFileSync('vmimpl.h','utf8').split('\n')

var ops = {}

var code,name
for(var i = 0;i<H.length;i++) {
  if (/#define/.test(H[i])) {
    if (name) ops[name]=code.join('\n');
    var tokens=H[i].match(/#define[ ]+([^ $]+)/)
    name=tokens[1].replace('DO_','');
    code=[H[i].replace(tokens[0],'').replace('\\','').replace(/^[ ]+/,'').replace(/{/,'')]
    console.log(name)
  } else if (/\\$/.test(H[i])) {
    code.push(H[i].replace('\\','').replace(/^[ ]+/,''))
  } else if (name) {
    if (H[i]) code.push(H[i].replace(/^[ ]+/,'').replace(/}[;]*/,''))
    ops[name]=code.filter(function (l) { return l }).join('\n');
    name=code=null;
  }
}

for(var p in ops) {
  fs.writeFileSync(p+'.c',ops[p]);
}
