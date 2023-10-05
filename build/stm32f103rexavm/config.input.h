#define LEXER       1
#define DICTLINEAR  0
#define CODESIZE    6000
#define STACKSIZE   256
#define RSTACKSIZE  128
#define FSTACKSIZE  128
#define LUTSIZE     64

#define FPDEBUG     0

#define CONSOLE(msg)         serialPrint(msg)
#define CONSOLELINE(msg)     serialPrintNL(msg)
#define CONSOLEFMT(...)      { char buf[100]; sprintf(buf,__VA_ARGS__); serialPrint(buf); }
#define CONSOLELINEFMT(...)  { char buf[100]; sprintf(buf,__VA_ARGS__); serialPrintNL(buf); }
#define CONSOLENL()          serialPrintNL("")
#define PRINT(...)           { char buf[100]; sprintf(buf,__VA_ARGS__); serialPrint(buf); }

