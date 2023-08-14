# Compiler

Incremental byetcode compilation that is processed in-place! Assumption: Current text code position is always higher than current bytecode program counter. 
 
## Bytecode Format

The bytecode format supports single byte, short and long words.

```pseudo
[CC|6]               Byte
[CC|6][8]            SHort Word
[CC|6][8][8][8]      Long  Word

CC  
==============================
00 Short 14 Bit constant value
01 Long 30 Bit constant value
1X Instruction
```

The upper two bits of the first byte defines the word sub-class (instruction word or constant literal values).

## Instruction Operand Encoding

- Abosulte branch addresses (CS)
- IOS calls with IOS LUT index
- Absolute local data addresses (CS)
- IOS data access by IOS LUT index

```ascii
  ub1    ub2          ub1    ub2
+----+---------+----+----+----+----+----+----+----
|CALL| CS ADDR | .. |  : |  SIZE+3 | .. | ;  |
+-+--+---------+----+-+--+----+----+----+----+---- 
  |                   |              ^         ^
  |                   |              |         |
  |                   +--------------|---------+
  +----------------------------------+
  
  ub1   sb1 (<0)
+-----+------+
|FCALL| -i-1 | i: IOS LUT Index
+-----+------+ 

  LIT (≥0)        LIT (<0)
+------------+   +-----------+
| VAR CS ADR |   | DIOS -i-1 | i: IOS LUT Index
+------------+   +-----------+
```

More encodings are shown in the following section with transformation rules.


## Transformation Rules

### Literals

```pseudo

<D>
--------------------------
[LIT|6:D(13 downto 8)]
[D(7 downto 0)]  ( MSB,LSB )

<DD>l
--------------------------
[TWOLIT|6:D(29 downto 24)]
[D(23 downto 16)]
[D(15 downto 8)]
[D(7 downto 0)]  ( MSB,LSB )
```

### Word Definition

```pseudo
: name
  
;
-------------------------
[DEF]
[16:(ub2)(size+3)]
a: ..
b: [DEFRET] ( size=b-a+1 )
```

## Conditional Branch


```pseudo
if .. 
a: endif
--------------------------
[IF][16:(ub2)a]..[ENDIF]

if .. 
a: else ..
endif
--------------------------
[IF][16:(ub2)a]..[ELSE]..[ENDIF]
```

## Loops

```pseudo
do 
a: .. 
   leave
   ..
b: loop/+loop
--------------------------
[DO][16:(ub2)b]
..
[LEAVE] ( indirect call to b+3 )
..
[CPLU/LOOP][16:(ub2)a]


```


## API

### Return

*vmerror*
: If not equal 0, an error occurd, and -pos contains the last character position.

*vmcslocked*
: If positive, a word was compiled and stored in the global dictionary. *vmcslocked* holds the CS end address.



### Non-incremental Loop

```js
code = text
pc   = init
pos  = start
pos=compile(code,pos,&pc);
```


### Incremental Loop

```js
code = text
pc   = init
pos  = start
steps = chunk
while (pos>=0 && !vmerror) pos=compile(code,pos,&pc,steps);
```

## Author and Version

```
Stefan Bosse
1.1.2 (4.4.2023)
```