STACKCHECK(3); addr=POP(); a=POP(); b=POP(); SENDN(addr,a,b);
pc++;DISPATCH(); /* send */