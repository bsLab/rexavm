# Words

## Data Operations (core)

All data operations are stack-based addressing the data stack (DS) to read operands and to write results.

| Operation | Operands | Description |
|:--|:---|:--|
| `+`  `-` `*` `/` `mod` `min` `max` | `( a b -- c )` | Two-operand integer arithmetics (like a/b) | 
| `abs` `not`^1^ | `( a -- b )` | Unary artithmetic operations |
| `and`  `or` `xor` | `( a b -- c )` | Two-operand logical bit-wise operations (like a and b) | 
| `<` `>` `<=`^1^ `>=`^1^ `=` | `( a b -- c )` | Two-operand relational operations (like a&lt;b) | 
| `dup` | `( a -- a a )` | Duplicates top stack element (DS) |
| `-dup`^1^ | `( a -- a a a )` | Duplicates top stack element twice times (DS) | 
| `?dup`^1^ | `( a -- a )` | Duplicates top stack element only if not zero, `dup if dup then` (DS) | 
| `drop` `fdrop`^1^ | `( a -- )` | Removes top stack element (DS/FS) | 
| `nip`^1^ | `( a b -- b )` | Removes second stack element, swap-drop combination (DS) | 
| `over` | `( a b -- a b a )` | Duplicates second top stack element (DS) | 
| `pick` `fpick`^1^ | `( n -- a )` | Picks (and duplicates) n-th element (DS/FS) | 
| `rot` | `( a b c -- b c a )` | Rotates three stack elements (DS) | 
| `-rot`^1^ | `( a b c -- c a b )` | Rotates three stack elements twice times (DS) | 
| `swap` | `( a b -- b a )` | Swaps two top stack elements (DS) | 
| `tuck` | `( a b -- b a b )` | Swap-over combination (DS) | 

In addition to single word operations, there is an optional set of double word operations, used by prefixing each operation with a `2` prefix. 

^1^Optional

## Data Conversion

| Operation | Operands | Description |
|:--|:---|:--|
| `2ext`  | `( d -- d1 d2 )` | Extends top stack element from single to double word |
| `2red`  | `( d1 d2 -- d  )` | Reduces top stack element(s) from double to single word |


## Stack Transfer

| Operation | Operands | Description |
|:--|:---|:--|
| `>r` | `( D: a -- R: -- a )` | Transfers top element from data (DS) to return stack (RS)^1^ |
| `r>` | `( R: a -- D: -- a )` | Transfers top element from return (RS) to data stack (DS)^1^ |
| `r@` `r` | `( R: a -- a D: -- a )` | Copies top element from return (RS) to data stack (DS)^1^ |
| `>f` | `( D: a -- F: -- a )` | Transfers top element from data (DS) to loop stack (FS) |
| `f>` | `( F: a -- D: -- a )` | Transfers top element from loop (FS) to data stack (DS) |
| `f@` `f` | `( F: a -- a D: -- a )` | Copies top element from loop (FS) to data stack (DS) |

In addition to single word operations, there is an optional set of double word operations, used by prefixing each operation with a `2` prefix (transferring always two stack cells without further processing). 

^1^Can be disabled for security reasons

## Control Flow

| Operation | Operands | Description |
|:--|:---|:--|
| `if` *A* `end` *N* | `( a -- )` | Branches to *A* if *a* is not zero, else branches to *N* |  
| `if` *A* `else` *B* `end` *N* | `( a -- )` | Branches to *A* if *a* is not zero, else braches to *B* |
| *a* *b* `do` *A* `loop` *N* | ( D: a b -- F: -- b i ) | Executes *A* as long as *i* &lt; *b*, starting with *i*=*a*, increments *i* by one after each iteration but before limit check | 
| *a* *b* `do` *A* *d* `+loop` *N* | ( D: a b -- F: -- b i ) | Executes *A* as long as *i* &lt; *b*, starting with *i*=*a*, increments *i* by *d* after each iteration but before limit check | 
| `begin` *A* *b* `until` *N* | - | Repeat *A* until *b* is not zero |
| `begin` *A* `again` *N* | - | Repeat *A*  (using `leave` optionally) |
| `leave` | - | Leave the current loop context |

## Signal Processing (ext)

| Operation | Operands | Description |
|:--|:---|:--|
| `sin` | `( x -- y )` | Integer discrete sine function (x/y scale: 1000) | 
| `log` | `( x -- y )` | Integer discrete logarithmic(base 10) function (scale: x 10, y 1000) | 
| `sigmoid` | `( x -- y )` | Integer discrete sigmoid function (x/y scale: 1000) | 
| `relu` | `( x -- y )` | Integer discrete rectifying linear unut function (x/y scale: 1000) | 
| `hull` | `( vecaddr vecoff veclen k --  )` |  In-place signal hull computation using a low-pass frequency filter | 
| `lowp` | `( vecaddr vecoff veclen k --  )` |  In-place signal low-pass frequency filter | 
| `highp` | `( vecaddr vecoff veclen k --  )` |  In-place signal high-pass frequency filter |


## Vector Operations (ext)

| Operation | Operands | Description |
|:--|:--|:--|
| `array <ident> <#cells>` | - | Allocates a data array in the code segment |
| `array <ident> { v1 v2 .. }` | - | Allocates an initialized data array in the code segment |
| `vecload` | `( srcvec srcoff dstvec -- )` | Loads a data array into another array buffer. The source can be any external data provided by the IOS or internal embedded data. The source array (plus optional offset) must have at least the size of the destination array | 
| `vecscale` | `( srcvec dstvec scalevec -- )`| Scales the source data array with scaling factors from the scale array and stores the result in the destination array. Negative scalign values reduce, positive values expand the source data values.^1^^2^ | 
| `vecadd` | `( op1vec op2vec dstvec scalevec -- )` | Adds two vectors element-wise with an optional result scaling (value 0 disables scaling). Both input and the destination vectors must have the same size.^1^^2^ | 
| `vecmul` | `( op1vec op2vec dstvec scalevec -- )` | Multiplies two vectors element-wise with an optional result scaling (value 0 disables scaling).Both input and the destination vectors must have the same size.^1^^2^ | 
| `vecfold` | `( invec wgtvec outvec scalevec -- )` | Performs a folding operation *ivec* &times; *wgtvec*. The weights vector *wgtvec* must have the size &Verbar;*invec*&Verbar;\*&Verbar;*outvec*&Verbar;.^1^^2^ |
| `vecconv` | `( inmat wgtmat outmat scale inwidth wgtwidth stride pad -- )` | Performs a 2D matrix convolution operation with *wgtmat* kernel matrix applied to *inmat*. The width of the input and kernel matrix must be provided, the height is computed automatically from the parameters as well as the output matrix. A matrix is just a flat array. The scaling is applied to all kernel computations. | 
| `vecmap` | `( srcvec dstvec func scalevec -- )` | Maps all elements from the source array onto the destination array using an external (IOS) or internal (user-defined word) function, e.g., the sigmoid function.^1^^2^ |
| `vecreduce` | `( vec off len op -- index val / val val )` | Applies an operation to a vector and returns a scalar (double word or index,value pair). Supported operations are minimum (op=1), maximum (op=2), mean (op=4), and sum (op=8) |
| `vecreshape` | `( vec dst k -- )` | Shrinks (k<0) or expands (k>0) a vector. |
^1^Negative scaling values *k* provide a division *v*/|*k*|, positive scaling values a multiplication *k*\**v*, zero value disables scaling. Note that a scaling vector of same size must be provided as the destination vector, except for the zero value that is a scalar value.
^2^Instead of a vector a negative k value (division only) can be provided to apply the same scaling to all vector elements. Internally, 2N arithmetics is used and a final down-scaling can prevent overflows. 

---
```pseudo
@array va { 1 2 3 4 }
@array vb { 5 6 7 8 }
@array vc 4
va vb vc 0 @vecadd
```
---
*Example of vector operations*


## Variables and Arrays

Arrays and variables are created by the `array` and `var` commands. The required data space is allocated inside or at the end of the current code segment region. Initialized arrays are allocated in-place, non-initialized arrays at the end. 

Variables as well as (internal) arrays are accessed by the `@` and `!` operators, summarized in the following table.

| Operation | Description |
|:--|:--|
| `var <ident>` | Allocates a single word variable. |
| `2var <ident>` | Allocates a double word variable. |
| `array <ident> <#cells>` | Allocates a data array in the code segment. |
| `array <ident> { v1 v2 .. }` | Allocates an initialized data array in the code segment. |
| `<ident> @` | Fetches the variable value and pushes the value on the data stack. |
| `<value> <ident> !` | Pops a value from the data stack and stores it in the variable. |
| `<ident> <index> cell+ @` | Fetches the array element with the given index and pushes the value on the data stack. |
| `<value> <ident> <index> cell+ !` | Pops a value from the data stack and stores it in the array at the specified index. |


