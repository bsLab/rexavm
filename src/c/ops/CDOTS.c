STACKCHECK(1);a=POP();b=a/2+a%2;
STACKCHECK(b);for(c=1;c<=b;c++) { ub2 cc=(ub2)POP(); CONSOLEFMT("%c",(cc>>8)&0xFF); CONSOLEFMT("%c",cc&0xFF);}
if (a%2==0) CONSOLEFMT("%c",' ');
pc++;DISPATCH(); /* .s */