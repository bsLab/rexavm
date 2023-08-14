2var counter 
0l counter 2! 
milli
1000 30 * 0 do 
  100 0 do 
    i j + drop 
    counter 2@ 1l 2+ 
    counter 2! 
  loop 
loop 
milli 
2swap 2- 2dup 2dup 
2. ."  ms  " cr 
counter 2@ 2dup 
2. ."  loops  " 
2swap 2/ 1000l 2* 2dup 
1000l 2/ 2. ." KLPS " cr
11l 2* 2dup 
2. ."  IPS  " 1000000l 2/ 
2. ."  MIPS  " cr 
end
