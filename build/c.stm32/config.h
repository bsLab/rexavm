#define CONSOLE(msg)         serialPrint(msg)
#define CONSOLELINE(msg)     serialPrintNL(msg)
#define CONSOLEFMT(...)      { char buf[100]; sprintf(buf,__VA_ARGS__); serialPrint(buf) }
#define CONSOLENL()          serialPrintNL("")
#define PRINT(...)           { char buf[100]; sprintf(buf,__VA_ARGS__); serialPrint(buf) }

