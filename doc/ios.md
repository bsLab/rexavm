# IOS

The Input-Output System (IOS) is used to access host platform functions and data directly from the VM programming level. The IOS provides a dynamic bridge controlled by the host application comparable with the widely used foreign function interface (FFI).

## C-API


For the binding of functions and data two different API functions are used, as shown below. The callback function (*fios*) must return the appropriate data format that is specified by the *retsize* argument: Signed byte (1), signed word (2), or signed double word (4), i.e., `sb1`, `sb2`, or `sb4`, respectively. If *retsize* is equal zero, no value is returned (`void`). The number of supported arguments (*args*) is currently in the range from zero to four (but depends on the word concrete `FCALL` implementation). All arguments are taken from the data stack, and the return value is stored on the stack. The data type encoding for input arguments is the same as for the return argument. A returned double word (`sb4`) is split into two `ub2` values, supporting the return of two independent values, too.


```pseudo
ub1 @fiosAdd(char *name, 
            void (*callback)(),
            ub1 args, 
            ub1 argsize, 
            ub1 retsize);
ub1 @diosAdd(char *name, 
            void *data, 
            ub2 cells,
            ub1 size);
```

Examples are shown below:

```pseudo
// ( -- a b )
sb4 tuple2() {
  sb2 a=-2000,b=-100;
  return (sb4)(((ub2)a<<16)+(ub2)b);
}
fiosAdd("tuple2",IOSCALLBACK(tuple2),0,0,4);

( -- d1 d2 )
sb4 value2() {
  sb4 d=-123456;
  return d;
}
fiosAdd("value2",IOSCALLBACK(value2),0,0,4);

// ( a b -- c )
sb2 add2(sb2 a, sb2 b) {
  return a+b
}
fiosAdd("add2",IOSCALLBACK(add2),2,2,4);
```
