STACKCHECK(1); a=POP(); vmevent.timeout=vmMilli()+a;
pc++;goto suspend; /* sleep */