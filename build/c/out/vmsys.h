#ifndef __VM_SYS_H
#define __VM_SYS_H

#ifdef SYSCALL
#define VMMAGICNUM   0xEFAC32BA
#ifndef VMSYSINIT
#define VMSYSINIT   pctop=0;\
      cstop=0;\
      dstop=0;\
      rstop=0;\
      fstop=0;
#endif
enum VMSYSCMDS {
  VMSYSINIT,
  VMSYSMAGIC,
  VMSYSGETENV,
  VMSYSSETENV,
  VMSYSCOMPILE,
  VMSYSRUN,
  VMSYSLOOP,
  VMSYSRESET,
  VMSYSIOSINIT,
  VMSYSFIOSADD,
  VMSYSDIOSADD,
  VMSYSHELLO
};
enum VMARGS {
  VMMAGIC,
  VMVERS,
  VMCS,
  VMCSTOP,
  VMPCTOP,
  VMDSTOP,
  VMRSTOP,
  VMFSTOP,
  VMBASE,
  VMERROR,
  VMEVENT,
  
  VMCONSOLEPRINT,
  VMCONSOLELINE,
  VMCONSOLEUPDATE,
  VMCONSOLESEND,
  VMCONSOLENL
};

extern char* (*consoleCurrentLine)(void);
extern void (*consolePrint)(char *msg);
extern void (*consoleUpdateLine)(void);
extern void (*consoleSend)(int v);
extern void (*consoleNL)(void);

extern u32 vmsysmagic;
extern u32 vmsys(u16 cmd, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5);
#endif


extern ub4 vmMilli(void);

#endif



