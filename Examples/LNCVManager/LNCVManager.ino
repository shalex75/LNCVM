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
void printCVList(){
  char buf[LNCV_TXT_MAX_LENGTH];
  
  Serial.print("count: ");
  Serial.println(cvm.get_count());
  for (int i =0; i < cvm.get_count(); i++){
    Serial.print("CV:");
    Serial.print(cvm.get_num_by_idx(cvm.get_sorted_idx(i)));
    Serial.print(";v:");
    Serial.print(cvm.get_val_by_idx(cvm.get_sorted_idx(i)));
    Serial.print(";d:");
    Serial.print(cvm.get_def_val_by_idx(cvm.get_sorted_idx(i)));
    Serial.print(";t:");
    Serial.print(cvm.get_cv_type_by_idx(cvm.get_sorted_idx(i)));
    Serial.print(";");
    cvm.get_string_by_idx(cvm.get_sorted_idx(i), buf);
    Serial.println(buf);
    
  }
}


void PrintFreeMem(){
  Serial.print("Free m:");
  Serial.println(freeRam());
}
void setup() {
  
  randomSeed(analogRead(0));
  Serial.begin(115000);
  cvm.eeprom_init();
  printCVList();
  PrintFreeMem();
}

void loop() {
  // putyour main code here, to run repeatedly:
  if (Serial.available()) {
    switch (Serial.read()) {
      case 'm':
        PrintFreeMem();
        break;
      case 'l':
        printCVList();
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
      default:
        break;
    }
  }
}