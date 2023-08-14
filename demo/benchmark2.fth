2var counter 
0l counter 2! 
var temp
0 temp !
milli
:* foo
  i j + temp !
  counter 2@ 1l 2+ 
  counter 2! 
;
1000 30 * 0 do 
  1000 0 do 
    foo 
  loop
loop 
milli 
2swap 2- 2dup 2dup 
2. ."  ms  " cr 
counter 2@ 2dup 
2. ."  loops  " 
2swap 2/ 1000l 2* 2dup 
1000l 2/ 2. ." KLPS " cr
14l 2* 2dup 
2. ."  WPS  " 1000000l 2/ 
2. ."  MWPS  " cr 
end
