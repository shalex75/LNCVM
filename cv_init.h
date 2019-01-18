#ifndef LN_CV_INIT
#define LN_CV_INIT

#define  LNCV_TXT_MAX_LENGTH 50

struct CVDesc {
  uint16_t  lncv_num;
  uint16_t  def_value;
  uint8_t  read_only;
  char txt[LNCV_TXT_MAX_LENGTH];
};

#endif