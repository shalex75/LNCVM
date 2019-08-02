#ifndef LN_CV_BM
#define LN_CV_BM

#include <Arduino.h>
#include <cv_init.h>
#include <cv_sys.h>

#define DEBUG_CVM
#undef DEBUG_CVM

#define KeyType uint16_t 


#if defined (__cplusplus)
	extern "C" {
#endif

extern int32_t notifyCVReadExternal( uint16_t CVAddress) __attribute__ ((weak));
extern int32_t notifyCVWriteExternal(uint16_t CVAddress, uint16_t val) __attribute__ ((weak));

extern void notifyAfterCVRead( uint16_t CVAddress, uint16_t val) __attribute__ ((weak));
extern void notifyAfterCVWrite(uint16_t CVAddress, uint16_t val) __attribute__ ((weak));

#if defined (__cplusplus)
}

#endif


class LNCVBManager{
private:
  uint16_t idx_reset_to_def;
  uint16_t idx_reset_device;
  
  uint16_t ln_art;
  uint16_t ln_firmware_id;
  uint16_t ln_version_id;
  
  uint16_t *lncv_val;
  uint16_t *lncv_num;
  uint16_t *lncv_idx;
	
  CVDesc * ln_sys;
  CVDesc * ln_user;
  CVDesc * ln_user1;
  CVDesc * ln_user2;
  CVDesc * ln_user3;
  
  CVDesc * ln_cvs;
  
  uint16_t count_user_cv;
  uint16_t count_user_cv1;
  uint16_t count_user_cv2;
  uint16_t count_user_cv3;
  uint16_t count_sys_cv;
  
  uint16_t count;
  
  KeyType calc_key();

  void soft_reset(){
#ifdef DEBUG_CVM
    Serial.println("reset!");
    delay(200);
#endif
    
    asm volatile ("  jmp 0");  
  }; 
  
public:  
  void reset_default();
  
  void eeprom_init();
  
  ~LNCVBManager();

  LNCVBManager(uint16_t ln_art_in, uint16_t ln_firmware_id_in, uint16_t ln_version_id_in,
	      uint16_t count_sys_cv_in, const CVDesc ln_sys_in[],
		  uint16_t count_user_cv_in, const CVDesc ln_user_in[],
		  uint16_t count_user_cv_in1, const CVDesc ln_user_in1[],
		  uint16_t count_user_cv_in2, const CVDesc ln_user_in2[],
		  uint16_t count_user_cv_in3, const CVDesc ln_user_in3[]
		  );
  
  uint16_t get_count() { return count; };
  uint16_t get_EEPROM_size() { return sizeof(KeyType) + count * sizeof(uint16_t); };

  int32_t  set_val_by_idx(uint16_t idx, uint16_t val);
  int32_t  set_val_by_num(uint16_t num, uint16_t val);
    
  
  int32_t  get_idx_by_num(uint16_t lncv_num_in);
  int32_t  get_num_by_idx(uint16_t idx); 

  
  uint32_t  get_val_by_num(uint16_t lncv_num);
  uint32_t  get_val_by_idx(uint16_t idx);
  
  int32_t  set_cv_from_str(String s);
  
  int32_t  cv(uint16_t lncv_num) { return get_val_by_num(lncv_num);};

  CV_TYPE   get_cv_type_by_idx(uint16_t idx);

  uint16_t  get_def_val_by_idx(uint16_t idx);
  
  uint16_t  get_string_by_idx(uint16_t idx, char res[]);
  uint16_t  get_string_by_num(uint16_t lncv_num, char res[]);
  uint16_t  get_sorted_idx(uint16_t pos);
  
  void 		print_cv_list();

  void      print_lncv_num();
};



#endif
