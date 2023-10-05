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
#define CODESIZE             32768
#define COMPILEINCR          0
#define DATAWIDTH            16
#define DSP                  1
#define ENDIANESS            "LE"
#define ENVDEPTH             8
#define DEFINELOCAL          1
#define DICTLINEAR           0
#define DICTSIZE             256
#define FSTACKSIZE           4000
#define IOSIZE               64
#define LEXER                1
#define LUTSIZE              300
#define MULTITASKING         0
#define MULTITHREADED        0
#define PROFILER             ""
#define READLINE             1
#define RSTACKSIZE           4000
#define STACKSIZE            16000
#define SYSARCH              "unix"
#define SDK                  "generic"
#define TWOWORDS             1
#define VMLOOP               2
#define VMRUNSLICE           50
#define WORDSTRINGS          1
#define FPDEBUG              0
#define DEBUG(...)           
#define PRINT(...)           printf(__VA_ARGS__)
#define CONSOLE(msg)         printf("%s",msg)
#define CONSOLEFMT(...)      printf(__VA_ARGS__)
#define CONSOLELINE(msg)     printf("%s\n",msg)
#define CONSOLELINEFMT(...)  printf(__VA_ARGS__); printf("\n")
#define CONSOLENL()          printf("\n")
#define OUT(v)               printf("$%04x",(ub2)v)
#define SEND(dst,v)          printf("$%02x:%04x",dst,(ub2)v)
#define SENDN(dst,n,addr)    {int i;printf("$%02x:%02x:",dst,(ub2)n);for(i=0;i<n;i++) printf("%04x",((ub2*)&CS[addr])[i]);}
typedef signed short int    sw1;
typedef unsigned short int  uw1;
typedef signed long  int    sw2;
typedef unsigned long  int  uw2;
#endif
