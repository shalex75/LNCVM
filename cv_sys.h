#ifndef LN_CV_SYS
#define LN_CV_SYS

#define LNCV_IDX_FIRMWARE_TYPE_ID 1003
#define DEF_FIRMWARE_TYPE_ID 1 // 1..999
#define LNCV_IDX_FIRMWARE_VER 1002
#define DEF_FIRMWARE_VER     3 // 1..999

#define LNCV_IDX_PARTY_ID 1000
#define DEF_PARTY_ID     0 // 1..65535

#define LNCV_IDX_MODULE_ID 1001
#define DEF_MODULE_ID     0 // 1..65535

//#define LNCV_IDX_LN_ART 999
//#define DEF_LN_ART  

#define LNCV_IDX_LN_ADDR 0
#define DEF_LNCV_ADDRESS 10

#define LNCV_IDX_RESET_TO_DEF 9
#define DEF_LNCV_RESET_TO_DEF 0

#define LNCV_IDX_RESET_DEVICE 8
#define DEF_LNCV_RESET_DEVICE 0

#define LNCV_SYS_COUNT 7

const CVDesc CV_SYS_INIT[LNCV_SYS_COUNT] PROGMEM ={
  {LNCV_IDX_LN_ADDR,          DEF_LNCV_ADDRESS,       "Default address of Loconet module"},
  {LNCV_IDX_RESET_TO_DEF,     DEF_LNCV_RESET_TO_DEF,  "Reset CVs to default (write any value)"},
  {LNCV_IDX_RESET_DEVICE,     DEF_LNCV_RESET_DEVICE,  "Reset device (write any value)"},
  {LNCV_IDX_PARTY_ID,         DEF_PARTY_ID,           "Party ID"},                           
  {LNCV_IDX_MODULE_ID,        DEF_MODULE_ID,          "Module ID (one party can have many nodules)"},
  {LNCV_IDX_FIRMWARE_TYPE_ID, DEF_FIRMWARE_TYPE_ID,   "Firmware type ID (type of decoder)"},
  {LNCV_IDX_FIRMWARE_VER,     DEF_FIRMWARE_VER,       "Firmware version"}
//  {LNCV_IDX_LN_ART,           DEF_LN_ART,             "LN Art"}

};

#endif
