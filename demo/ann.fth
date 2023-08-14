( Layers: 14,8,2 #neurons:24 )
array input 14
array wghtsI { 194 944 19 -974 -188 456 335 84 663 627 659 663 722 655 }
array biasI { 542 903 354 -689 219 -127 -744 -65 -797 -502 -882 -744 -352 454 }
array scaleI { 2 -9 2 9 8 -2 2 1 -7 -8 0 5 -1 6 }
array actI 14
array wghtsH1 { -388 624 913 -658 -537 838 -134 984 -600 -297 890 -395 -774 398 465 189 38 -824 647 -858 -472 649 14 874 -812 289 -110 -525 -448 -831 723 812 965 237 201 396 -855 186 887 -125 697 935 -109 -915 -467 -66 492 161 -416 123 -58 -109 150 198 -826 -370 -915 -601 -954 574 459 -32 -455 911 213 -761 -873 -752 993 750 826 -93 -368 446 304 303 28 -990 -649 93 470 521 -898 -127 572 556 -841 629 27 -849 -652 137 -67 -868 -341 -662 -18 -396 -544 -34 811 -531 663 -441 414 325 311 610 153 -54 -824 -439 }
array biasH1 { -475 454 284 606 866 761 209 -225 }
array scaleH1 { -9 -2 8 2 8 -9 -9 -1 }
array actH1 8
array wghtsO { 80 -524 112 -411 -640 959 -155 -339 326 753 -193 302 -200 -117 294 -738 }
array biasO { -766 -703 }
array scaleO { 3 9 }
array output 2
( Input data is stored in input )
( Output data is stored in output )
: forward
  input wghtsI actI scaleI vecmul
  actI biasI actI 0 vecadd
  actI actI $ sigmoid 0 vecmap
  actI wghtsH1 actH1 scaleH1 vecfold
  actH1 biasH1 actH1 0 vecadd
  actH1 actH1 $ sigmoid 0 vecmap
  actH1 wghtsO output scaleO vecfold
  output biasO output 0 vecadd
  output output $ sigmoid 0 vecmap
;
( sample 0 input vecload )
2var counter
0 counter !
." Test " cr
milli >f >f
1000 30 * 0 do
  100 0 do
    forward
    counter 2@ 1l 2+ counter 2!
  loop
loop
milli f> f> 2-
counter 2@ 2. ." loops " cr
1000l 2* 2dup 2. ." us  " 2dup 1000l 2/ 2. ." ms tot. " cr
2dup counter 2@ 1000l 2/ 2/ 2. ." ns/forward " cr
." Layers: 3 Neurons: 24 " cr
." Done. " cr
end
