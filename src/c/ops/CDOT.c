STACKCHECK(1); CONSOLEFMT(vmbase==0?"%d ":"%x ",vmbase==0?(sb2)POP():(ub2)POP()); pc++;
DISPATCH(); /* . */