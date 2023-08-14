array input  250 ( [50,5] )

( One CNN convolution )
( [50,5,1] k=[5,5] pad=2 stride=2  filters=3 )
array kernel1 { 1 2 3 4 5
                6 7 8 9 0
                1 2 3 4 5
                6 7 8 9 0
                1 2 3 4 5 }
array kernel2 { 1 2 3 4 5
                6 7 8 9 0
                1 2 3 4 5
                6 7 8 9 0
                1 2 3 4 5 }
array kernel3 { 1 2 3 4 5
                6 7 8 9 0
                1 2 3 4 5
                6 7 8 9 0
                1 2 3 4 5 }
( [25,3,3] )
array convout 75

( Max-Pooling )
( [12,1,3] k=[2,2] stride=2 filters=3 )
array poolou1 12
array poolou2 12
array poolou3 12

( FC/Softmax )
( [12,1,3] => [2] )
array wghta1 { 
1 2 3 4 5 6 7 8 9 0 1 2 
}
array wghta2 { 
1 2 3 4 5 6 7 8 9 0 1 2
}
array wghta3 { 
1 2 3 4 5 6 7 8 9 0 1 2
}

array wghtb1 { 
1 2 3 4 5 6 7 8 9 0 1 2
}
array wghtb2 { 
1 2 3 4 5 6 7 8 9 0 1 2
}
array wghtb3 { 
1 2 3 4 5 6 7 8 9 0 1 2
}

array bias2 { -766 -703 }
array output 2

array accu 12

array scalea1 {
1 1 1 1 1 1 1 1 1 1 1 1 1 1
}
array scalea2 {
1 1 1 1 1 1 1 1 1 1 1 1 1 1
}
array scalea3 {
1 1 1 1 1 1 1 1 1 1 1 1 1 1
}

array scaleb1 {
1 1 1 1 1 1 1 1 1 1 1 1 1 1
}
array scaleb2 {
1 1 1 1 1 1 1 1 1 1 1 1 1 1
}
array scaleb3 {
1 1 1 1 1 1 1 1 1 1 1 1 1 1
}

: cnnfw
  ( conv & pool )
  input kernel1 convout 1 50 5 2 2 vecconv
  convout convout $ sigmoid 0 vecmap
  convout 256 2 + poolou1 1 25 -2 2 0 vecconv
  input kernel2 convout 1 50 5 2 2 vecconv
  convout convout $ sigmoid 0 vecmap  
  convout 256 2 + poolou2 1 25 -2 2 0 vecconv
  input kernel3 convout 1 50 5 2 2 vecconv
  convout convout $ sigmoid 0 vecmap  
  convout 256 2 + poolou3 1 25 -2 2 0 vecconv
  
  ( neuron 1 input )
  poolou1 wghta1 accu scalea1 vecmul
  accu 0 12 8 vecreduce
  poolou2 wghta2 accu scalea2 vecmul
  accu 0 12 8 vecreduce
  poolou3 wghta3 accu scalea3 vecmul
  accu 0 12 8 vecreduce
  
  ( neuron 1 output )
  2+ 2+ 445l 2/ 2red sigmoid 
  output 0 cell+ !
  
  ( neuron 2 input )
  poolou1 wghtb1 accu scaleb1 vecmul
  accu 0 12 8 vecreduce
  poolou2 wghtb2 accu scaleb2 vecmul
  accu 0 12 8 vecreduce 
  poolou3 wghtb3 accu scaleb3 vecmul
  accu 0 12 8 vecreduce 
  
  ( neuron 2 output )
  2+ 2+ 312l 2/ 2red sigmoid 
  output 1 cell+ !
;


2var counter
0 counter !
." Test " cr
milli >f >f
1000 0 do
  100 0 do
    cnnfw
    counter 2@ 1l 2+ counter 2!
  loop
loop
milli f> f> 2-
counter 2@ 2. ." loops " cr
1000l 2* 2dup 2. ." us  " 2dup 1000l 2/ 2. ." ms tot. " cr
2dup counter 2@ 1000l 2/ 2/ 2. ." ns/cnnfw " cr
2drop

end

