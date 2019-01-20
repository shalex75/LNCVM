#include <EEPROMex.h>
#include "cv_init.h"
#include "cv_sys.h"
#include "CV_USER.h"
#include "cv_m.h"

#include <avr/eeprom.h>

#define DEF_LN_ART        5003 // 1..9999 
#define DEF_LN_FIRMWARE_ID 1
#define DEF_LN_FIRMWARE_VER 4

LNCVManager cvm(DEF_LN_ART, DEF_LN_FIRMWARE_ID, DEF_LN_FIRMWARE_VER, LNCV_SYS_COUNT, LNCV_COUNT, CV_SYS_INIT, CV_USER_INIT );

int freeRam () 
{
extern int __heap_start, *__brkval; 
int v; 
return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
uint16_t notifyCVRead(uint16_t LNCVAddr){
  uint16_t val = 0;
  
  if  (LNCVAddr == LNC_CV_EXT_READ) val = 777;
  Serial.print("Read CV:");
  Serial.print(LNCVAddr);
  Serial.print(" v:");
  Serial.print(val);
  
  return val;
}

int32_t notifyCVWrite(uint16_t LNCVAddr, uint16_t val){
  Serial.print("Write CV:");
  Serial.print(LNCVAddr);
  Serial.print(" v:");
  Serial.print(val);
}


void PrintFreeMem(){
  Serial.print("Free m:");
  Serial.println(freeRam());
}
void setup() {
  
  randomSeed(analogRead(0));
  Serial.begin(115000);
  cvm.eeprom_init();
  cvm.print_cv_list();
  PrintFreeMem();
}
void ParseCV(){
byte index = 0;
String s = "c";
char inChar = 0;;
 //Serial.println("Parse!"); // Now do something with the string (but not using ==)
 while (((byte)inChar != 13) && (index < 100)) // Don't read unless
  if (Serial.available())                                               // there you know there is data
  {
      if(index < 100) // One less than the size of the array
      {
          inChar = Serial.read(); // Read a character; 
          s = s + inChar;
          index++;
      }
  }
  Serial.print("str: "); // Now do something with the string (but not using ==)
  Serial.print(s); // Now do something with the string (but not using ==)
  Serial.print(" ret_code: "); // Now do something with the string (but not using ==)
  int32_t res = cvm.set_cv_from_str(s);
  Serial.println(res);
  
}
void loop() {
  // putyour main code here, to run repeatedly:
  if (Serial.available()) {
    switch (Serial.read()) {
      case 'm':
        PrintFreeMem();
        break;
      case 'l':
        cvm.print_cv_list();
        break;
      case 'd':
        cvm.set_val_by_num(LNCV_IDX_RESET_TO_DEF, 0);
        break;
      case 't':
        cvm.set_val_by_num(LNC_CV_EXT_READ, 999);
        break;
      case 'r':
        cvm.set_val_by_num(LNCV_IDX_RESET_DEVICE, 0);
        break;
      case 'c':
        ParseCV();
      default:
        break;
    }
  }
}