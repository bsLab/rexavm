
#include "config.h"
#include "standard.h"
#include "compile.h"
#include "dict.h"
#include "stack.h"
#include "vm.h"
#include "vmloop.h"
#include "vmio.h"
#include "vmwords.h"

#include "fpsin.h"
#include "fplog10.h"
#include "fpsigmoid.h"
#include "fpvec.h"

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
  dstop=0;
  fstop=0;
  rstop=0;
}


// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(PC14, OUTPUT);
  digitalWrite(PC13, HIGH);  // turn the LED on (HIGH is the voltage level)
  cstop=csbottom=0;
  iosInit();
  // ( -- M M )
  fiosAdd("milli",IOSCALLBACK(myMilli),0,0,4);
  // ( --  )
  fiosAdd("reset",IOSCALLBACK(myReset),0,0,0);
  PRINT("REXAVM ready @%d Hz\n",F_CPU);
}

int counter=0;

void serialPrint(char *buf){
  Serial.print(buf);
}
void serialPrintNL(char *buf){
  Serial.println(buf);
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
