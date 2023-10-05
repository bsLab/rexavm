ub1 *str=(ub1 *)&code[pc+1];a=strlen((char*)str);b=a/2+a%2;
for(c=b-1;c>=0;c--) PUSH((str[c*2]<<8)+str[c*2+1]); PUSH(a);
pc+=(a+2);DISPATCH(); /* s" */
