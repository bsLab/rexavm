char *str=(char *)&code[pc+1];CONSOLE(str);
pc+=(strlen(str)+2);DISPATCH(); /* ." */