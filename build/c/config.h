#ifndef __CONFIG_H
#define __CONFIG_H

// Support double word operations (32 bit) (2xxx ops)
#define TWOWORDS    1

#define CODESIZE    32768
#define STACKSIZE   1024
#define RSTACKSIZE  1024
#define FSTACKSIZE  1024
// Global dictionary size for word definitions
#define DICTSIZE    256
// Temporary LUT dictionary (compiler) for local variables and arrays
#define LUTSIZE     128
// Maximal control block nesting (IF,DO,..)
#define ENVDEPTH 8

// C Function and Data IO Table sizes  (FORTH FCALL and READ/WRITE API)
#define IOSIZE      64

#define VMRUNSLICE  50

// #define MULTITHREADING
#ifdef MULTITHREADING
  #define MAXTHREADS  4
#endif

// #define MULTITASKING
#ifdef MULTITASKING
  #define MAXTASKS  16
  #define FRAMES    16
#endif

// #define PROFILING
#ifdef  PROFILING
  extern unsigned int profileops;
  #define PROFILER          profileops++; // profileops++;
#else
  #define PROFILER
#endif

#define COMMENTS  1

// Linear Search Table lexer
#define LSTLEXER  1

// Perfect Hash lexer
// #define PHLEXER

// With word string list table
#define WORDSTRINGS 1

#ifdef PHLEXER
  #define WORDSTRINGS 1
#endif

// System-dependent macros
#define DEBUG(...)                // printf(__VA_ARGS__)
#define PRINT(...)                printf(__VA_ARGS__)
#define CONSOLE(msg)              printf("%s",msg)
#define CONSOLEFMT(fmt,msg)       { printf(fmt,msg); }
#define CONSOLELINE(msg)          printf("%s\n",msg)
#define CONSOLELINEFMT(fmt,msg)   printf(fmt,msg); printf("\n")
#define CONSOLENL()               printf("\n")
#define OUT(v)                    printf("$%04x",(ub2)v)
#define SEND(dst,v)               printf("$%02x:%04x",dst,(ub2)v)
#define SENDN(dst,n,addr)         printf("$%02x:%02x:",dst,(ub2)n)

// Provide a millisecond clock
#include <sys/time.h>
#define MILLI   ub4 vmMilli() {\
  long long milli;\
  struct timeval tv;\
  gettimeofday(&tv,NULL);\
  milli=(((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);\
  return (ub4)milli;\
}


// VM Loop Architecture
#define COMPUTEDGOTO      1
#define SWITCHCASE        2
#define SWITCHCASEGOTO    3
#define FUNCTABLE         4
#define VMLOOP SWITCHCASE

#endif
