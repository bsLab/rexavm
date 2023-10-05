#ifndef __VM_CONFIG_H
#define __VM_CONFIG_H
#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif
#include <sys/time.h>
#define COMPUTEDGOTO         1
#define SWITCHCASE           2
#define LSTLEXER             1
#define PHLEXER              2
#define COMMENTS             1
#define CODESIZE             5000
#define COMPILEINCR          0
#define DATAWIDTH            16
#define DSP                  1
#define ENDIANESS            "LE"
#define ENVDEPTH             8
#define DEFINELOCAL          1
#define DICTLINEAR           0
#define DICTSIZE             256
#define FSTACKSIZE           128
#define IOSIZE               64
#define LEXER                1
#define LUTSIZE              64
#define MULTITASKING         0
#define MULTITHREADED        0
#define PROFILER             ""
#define READLINE             0
#define RSTACKSIZE           128
#define STACKSIZE            256
#define SYSARCH              "stm32"
#define SDK                  "generic"
#define TWOWORDS             1
#define VMLOOP               2
#define VMRUNSLICE           50
#define WORDSTRINGS          1
#define DEBUG(...)           
#define PRINT(...)           { char buf[100]; sprintf(buf,__VA_ARGS__); serialPrint(buf) }
#define CONSOLE(msg)         serialPrint(msg)
#define CONSOLEFMT(...)      { char buf[100]; sprintf(buf,__VA_ARGS__); serialPrint(buf) }
#define CONSOLELINE(msg)     serialPrintNL(msg)
#define CONSOLELINEFMT(...)  printf(__VA_ARGS__); printf("\n")
#define CONSOLENL()          serialPrintNL("")
#define OUT(v)               printf("$%04x",(ub2)v)
#define SEND(dst,v)          printf("$%02x:%04x",dst,(ub2)v)
#define SENDN(dst,n,addr)    {int i;printf("$%02x:%02x:",dst,(ub2)n);for(i=0;i<n;i++) printf("%04x",((ub2*)&CS[addr])[i]);}
typedef signed short int    sw1;
typedef unsigned short int  uw1;
typedef signed long  int    sw2;
typedef unsigned long  int  uw2;
#endif
