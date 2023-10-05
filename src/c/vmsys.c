/*
VM system call API

Programming and interaction with VM only via vmsys wrapper API
*/
#include "config.h"
#include "standard.h"
#include "vmsys.h"

#ifdef SYSCALL
u32 vmsysmagic = VMMAGICNUM;

char* (*consoleCurrentLine)(void)=NULL;
void (*consolePrint)(char *msg)=NULL;
void (*consoleUpdateLine)(void)=NULL;
void (*consoleSend)(int v)=NULL;
void (*consoleNL)(void)=NULL;


u32 vmsys(u16 cmd,u32 arg1,u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
  char buf[40];
  switch (cmd) {
    case VMSYSINIT:
/*
      vmsysmagic = VMMAGICNUM;
      pctop=0;
      cstop=0;
      dstop=0;
      rstop=0;
      fstop=0;
*/
      // Copy the static data initializations
      VMSYSINIT
      break;
    case VMSYSGETENV :   
      switch (arg1) {
        case VMMAGIC :  return (u32)VMMAGICNUM;
        case VMVERS  :  return (u32)VMVERSION;
        case VMCS    :  return (u32)&CS;
        case VMPCTOP:   return (u32)&pctop; 
        case VMCSTOP:   return (u32)&cstop; 
        case VMDSTOP:   return (u32)&dstop; 
        case VMRSTOP:   return (u32)&rstop; 
        case VMFSTOP:   return (u32)&fstop; 
        case VMBASE:    return (u32)&vmbase; 
        case VMERROR:   return (u32)&vmerror;
        case VMEVENT:   return (u32)&vmevent; 
      }
      break;
    case VMSYSSETENV :  
      switch (arg1) {
        case VMCONSOLEPRINT:  consolePrint=(void (*)(char *msg))arg2; break;
        case VMCONSOLELINE:   consoleCurrentLine=(char* (*)())arg2; break;
        case VMCONSOLEUPDATE: consoleUpdateLine=(void (*)())arg2; break;
        case VMCONSOLESEND:   consoleSend=(void (*)())arg2; break;
        case VMCONSOLENL:     consoleNL=(void (*)())arg2; break;
      }
      return 0;
      break;
    case VMSYSCOMPILE : return compile(CS,(sb2) /* position */ arg1, (ub2 *) arg2 /* *pc */);
    case VMSYSRUN     : return vmrun((ub2) arg1 /* steps */);
    case VMSYSLOOP    : return vmloop(CS, (ub2) arg1 /* pc */, (ub2) arg2 /*steps*/);
    case VMSYSRESET   : vmreset(); return 0; 
    case VMSYSIOSINIT : iosInit(); return 0;
    case VMSYSFIOSADD : return (u32) fiosAdd((char *)arg1 /* char *name */,
                                             IOSCALLBACKPTR(arg2),
                                             (ub1)arg3,
                                             (ub1)arg4,
                                             (ub1)arg5);
    case VMSYSDIOSADD : return (u32) diosAdd((char *)arg1 /* char *name */,
                                             IOSDATA(arg2),
                                             (ub2)arg3,
                                             (ub1)arg4);
    default: return 0;
  }
  return 0;
}
#endif

// Wrapper for system millisecond clock returning current (ub4) millisecods
ub4 vmMilli(void){
#ifdef MILLI
MILLI
#else
  return 0;
#endif
}

