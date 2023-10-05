#define DEBUG(...)           
#define PRINT(...)           { char buf[200]; sprintf(buf,__VA_ARGS__); serialPrint(buf); }
#define CONSOLE(msg)         { char buf[200]; sprintf(buf,"%s",msg); serialPrint(buf); }
#define CONSOLEFMT(...)      { char buf[200]; sprintf(buf,__VA_ARGS__); serialPrint(buf); }
#define CONSOLELINE(msg)     { char buf[200]; sprintf(buf,"%s",msg); serialPrintNL(buf); }
#define CONSOLELINEFMT(...)  { char buf[200]; sprintf(buf,__VA_ARGS__); serialPrintNL(buf); }
#define CONSOLENL()          { serialPrintNL(""); }
#define OUT(v)               { char buf[50]; sprintf(buf,"$%04x",(ub2)v); serialPrint(buf); } 
#define SEND(dst,v)          { char buf[50]; sprintf(buf,"$%02x:%04x",dst,(ub2)v); serialPrint(buf); }
#define SENDN(dst,n,addr)    {char buf[50]; int i;sprintf(buf,"$%02x:%02x:",dst,(ub2)n);serialPrint(buf);\
                                            for(i=0;i<n;i++) { sprintf(buf,"%04x",((ub2*)&CS[addr])[i]); serialPrint(buf);}}
