
#include "config.h"
#include "standard.h"
#include "compile.h"
#include "dict.h"
#include "stack.h"
#include "vm.h"
#include "vmloop.h"
#include "vmio.h"
#include "vmwords.h"


uw1 csbottom=0;


// use CONSOLEFMT, CONSOLELINE, or PRINT macros for output 

static int execute() {
  sb2 pos;
  PRINT("csbottom [%d] cstop [%d]\n",csbottom,cstop);
  pos=pctop=(sb2)csbottom;
  pos=compile(&CS[csbottom],pos,(uw1*)&pctop);
  if (vmerror!=EOK) {
    PRINT("Compilation failed with error %d at character position %d\n",vmerror,-pos);
    return -1;
  }
  PRINT("Code size: %d Byte(s)\n",pctop);    
  pctop=0;
  vmstate=0;
  while (vmstate!=VMEND && !vmerror) pctop=vmloop(CS,(uw1)pctop,1000);
  PRINT("END pc=%x state=%x ds=%d rs=%d fs=%d err=%d\n",pctop,vmstate,dstop,rstop,fstop,vmerror);
  return vmerror;
}

static ub4 myMilli() {
  return (ub4)millis ();
}
static ub4 myReset() {
  cstop=0;
  dstop=0;
  fstop=0;
  rstop=0;
}
#define RAMTOP 0x20005000


static ub4 myStatus() {
  register char * stack_ptr asm ("sp");
  PRINT("REXAVM %s ready @%d Hz\nCS=%d DS=%d FS=%d RS=%d CSTACK=%d\n",
        VMVERSION,F_CPU,CODESIZE,STACKSIZE,FSTACKSIZE,RSTACKSIZE,
        RAMTOP-(unsigned long)stack_ptr);
}

extern "C" {
  #include "fpsin.h"
  #include "fplog10.h"
  #include "fprelu.h"
  #include "fpsigmoid.h"
  #include "fpvec.h"
  void fpInit() {
#if DSP==1
  // DSP Math
  FPSININIT;
  FPLOG10INIT;
  FPSIGMOIDINIT;
  FPRELUINIT;
  FPVECINIT;
#endif
  }
}

// the setup function runs once when you press reset or power the board
void setup() {
  register char * stack_ptr asm ("sp");
  Serial.begin(115200);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(PC13, OUTPUT);
  digitalWrite(PC13, HIGH);  // turn the LED on (HIGH is the voltage level)
  cstop=csbottom=0;
  iosInit();
  // ( -- M M )
  fiosAdd("milli",IOSCALLBACK(myMilli),0,0,4);
  // ( --  )
  fiosAdd("reset",IOSCALLBACK(myReset),0,0,0);
  // ( --  )
  fiosAdd("status",IOSCALLBACK(myStatus),0,0,0);

  fpInit();
}

int counter=0;

extern "C" {
  void serialPrint(char *buf){
    Serial.print(buf);
  }
  void serialPrintNL(char *buf){
    Serial.println(buf);
  }
}
// the loop function runs over and over again forever
int state=0;
void loop() {
  if(Serial.available()){
    char c = Serial.read();
    CS[cstop] = c;
    if (CS[cstop]=='\n') {
      CS[cstop]=0;
      execute();
      cstop=csbottom; // TODO incremental compiling and processing
    } else {
      // multi-line code with \n=>\r replacement?
      if (CS[cstop]=='\r') CS[cstop]='\n';
      cstop++;
    }
    if (state) {
      digitalWrite(PC13, HIGH);   // turn the LED on (HIGH is the voltage level)
      state=0;
    } else {
      digitalWrite(PC13, LOW);    // turn the LED off by making the voltage LOW
      state=1;
    }
  }
}
