#ifndef LN_CV_INIT
#define LN_CV_INIT

#define  LNCV_TXT_MAX_LENGTH 50

typedef enum {  
	CV_RW = 0, 
	CV_RO = 1, 
	CV_RW_EXT = 2, 
	CV_RO_EXT = 3
} CV_TYPE;

struct CVDesc {
  uint16_t  lncv_num;
  uint16_t  def_value;
  CV_TYPE   cv_type;
  char      txt[LNCV_TXT_MAX_LENGTH];
};

#endif