#ifndef LN_CV_M
#define LN_CV_M

#define DEBUG_CVM
#undef DEBUG_CVM

#define KeyType long 


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


class LNCVManager{
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
  
  uint16_t count_user_cv;
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
  
  ~LNCVManager();
  
  LNCVManager(uint16_t ln_art_in, uint16_t ln_firmware_id_in, uint16_t ln_version_id_in,
	      uint16_t count_sys_cv_in, uint16_t count_user_cv_in, const CVDesc ln_sys_in[], const CVDesc ln_user_in[]);

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

  int32_t  LNCVManager::set_cv_from_str(String s){

//	String digits = "0123456789";
	uint16_t cv_num;
	uint16_t cv_val;
	
#ifdef DEBUG_CVM
     Serial.print("cv str len:");
     Serial.print(s.length());
#endif

	if (s.length() < 5) return -6;

#ifdef DEBUG_CVM
     Serial.print(" s:");
     Serial.println(s);
#endif

	
	if (!(s.substring(0,2)=="cv")) return -7;
	
	int16_t vs = s.indexOf("=");
	if (vs >=3 ){
			cv_val = s.substring(vs + 1).toInt();
			cv_num = s.substring(2, vs).toInt();
#ifdef DEBUG_CVM
			Serial.print("vs: ");
			Serial.print(vs);
			Serial.print(" n:");
			Serial.print(s.substring(2, vs));
			Serial.print(" v:");
			Serial.print(s.substring(vs+1));
			
			Serial.print(" cv_val: ");
			Serial.print(cv_val);
			Serial.print(" cv_num: ");
			Serial.println(cv_num);
#endif
			return set_val_by_num(cv_num, cv_val);
	}
	else
		return -8;
  }
  void      LNCVManager::print_lncv_num() {
#ifdef DEBUG_CVM
     Serial.print("lncv_num c:");
     Serial.println(count);
     for (int i = 0; i < count; i++){
       Serial.print("i: ");
       Serial.print(i);
       Serial.print("v: ");
       Serial.println(lncv_num[i]);
     }
#endif     
  };
  
void LNCVManager::print_cv_list(){
  char buf[LNCV_TXT_MAX_LENGTH];
  Serial.print("Art: ");
  Serial.print(ln_art);
  Serial.print(" fwt: ");
  Serial.print(ln_firmware_id);
  Serial.print(" ver: ");
  Serial.print(ln_version_id);
  Serial.print(" CVs: ");
  Serial.print(get_count());
  Serial.print(" sz: ");
  Serial.println(get_EEPROM_size());
  for (int i =0; i < get_count(); i++){
    Serial.print("cv");
    Serial.print(get_num_by_idx(get_sorted_idx(i)));
    Serial.print("=");
    Serial.print(get_val_by_idx(get_sorted_idx(i)));
    Serial.print(";d:");
    Serial.print(get_def_val_by_idx(get_sorted_idx(i)));
    Serial.print(";t:");
    switch (get_cv_type_by_idx(get_sorted_idx(i))){
      case CV_RW:
        Serial.print("rw");
        break;
      case CV_RO:
        Serial.print("ro");
        break;
      case CV_RW_EXT:
        Serial.print("rw_ext");
        break;
      case CV_RO_EXT:
        Serial.print("ro_ext");
        break;
      default:
        Serial.print("unk");
        break;
    }
    Serial.print(";");
    get_string_by_idx(get_sorted_idx(i), buf);
    Serial.println(buf);
    
  }
};

  CV_TYPE   LNCVManager::get_cv_type_by_idx(uint16_t idx){
    uint8_t  val;
    if (idx <  count_sys_cv) 
      val = pgm_read_byte_near(&ln_sys[idx].cv_type);
    else
      val = pgm_read_byte_near(&ln_user[idx - count_sys_cv].cv_type);
    return (CV_TYPE)val;
  };

  uint16_t LNCVManager::get_sorted_idx(uint16_t pos){
    return lncv_idx[pos];
  }
  KeyType LNCVManager::calc_key(){
    KeyType  key = ln_art + ln_firmware_id * 10000 + ln_version_id * 1000000;
    return key;
  };
  
  uint16_t  LNCVManager::get_def_val_by_idx(uint16_t idx){
    uint16_t  val;
    if (idx <  count_sys_cv) 
      val = pgm_read_word_near(&ln_sys[idx].def_value);
    else
      val = pgm_read_word_near(&ln_user[idx - count_sys_cv].def_value);
    return val;
  }
  
  void LNCVManager::reset_default(){
    for(int i = 0; i < count; i++)
      lncv_val[i] = get_def_val_by_idx(i);
    uint16_t addr = sizeof(KeyType);
    eeprom_write_block((void*)lncv_val, (void*)(addr), count *sizeof(lncv_val[0]) );                
#ifdef DEBUG_CVM
    Serial.println("CVDEF!");
#endif    
  };  
  
  void LNCVManager::eeprom_init(){
    KeyType key;
    KeyType master_key = calc_key();
#ifdef DEBUG_CVM
    Serial.print("mkey:");
    Serial.println(master_key);
#endif
    uint16_t addr = 0;

    eeprom_read_block((void*)&key, (const void*)addr, sizeof(KeyType));
    if (key != master_key){
#ifdef DEBUG_CVM
      Serial.println("INIT!");
#endif      
      addr = 0;
      eeprom_write_block((void*)&master_key, (void*)addr, sizeof(master_key));                
      
      reset_default();
    } else {
      addr = sizeof(KeyType);
      eeprom_read_block((void*)lncv_val, (void*)(addr), count *sizeof(lncv_val[0]) );                      
    };
  };
  
  LNCVManager::~LNCVManager(){
    delete[] lncv_idx;
    delete[] lncv_num;
    delete[] lncv_val;
  };
  
  LNCVManager::LNCVManager(uint16_t ln_art_in, uint16_t ln_firmware_id_in, uint16_t ln_version_id_in, 
			   uint16_t count_sys_cv_in, uint16_t count_user_cv_in, 
			   const CVDesc ln_sys_in[], const CVDesc ln_user_in[]){
    ln_art = ln_art_in;
    ln_firmware_id = ln_firmware_id_in;
    ln_version_id = ln_version_id_in;

    idx_reset_to_def = 0;
    idx_reset_device = 0;
 
    ln_sys = (CVDesc *)ln_sys_in;
    ln_user = (CVDesc *)ln_user_in;
  
    count_sys_cv = count_sys_cv_in;
    count_user_cv  = count_user_cv_in;
    count = count_sys_cv + count_user_cv;

    lncv_val = new uint16_t[count];
    lncv_num = new uint16_t[count];
    lncv_idx = new uint16_t[count];

    for(int i = 0; i < count_sys_cv; i++)
        lncv_num[i] = pgm_read_word_near(&ln_sys[i].lncv_num);

    for(int i = 0; i < count_user_cv; i++) 
        lncv_num[i + count_sys_cv] = pgm_read_word_near(&ln_user[i].lncv_num);

//    uint8_t  *lncv_ro;

    for(int i = 0; i < count; i++) lncv_idx[i] = i;
    
    uint16_t t;
    
//sorting nums    
    for(int i = 0; i < count-1; i++) 
      for(int j = i + 1; j < count; j++) 
        if (lncv_num[lncv_idx[i]] > lncv_num[lncv_idx[j]]) {
          t = lncv_idx[i];
          lncv_idx[i] = lncv_idx[j];      
          lncv_idx[j] = t;
        }
  };


  
    
  int32_t  LNCVManager::set_val_by_idx(uint16_t idx, uint16_t val){
#ifdef DEBUG_CVM
    Serial.print("set_val_by_idx: ");
    Serial.print("idx: ");
    Serial.print(idx);
    Serial.print(" v: ");
    Serial.println(val);
#endif    

    if ((idx >= count)) return -1;

#ifdef DEBUG_CVM
    Serial.print("set_val_by_idx: idx:");
    Serial.print(idx);
    Serial.print(" v: ");
    Serial.println(val);
#endif
    switch (lncv_num[idx]){
      case LNCV_IDX_RESET_TO_DEF:
#ifdef DEBUG_CVM
		Serial.println("reset to def!");
#endif      
		reset_default();
        soft_reset();
        break;
      case LNCV_IDX_RESET_DEVICE:
#ifdef DEBUG_CVM
		Serial.println("reset device!");
#endif      
        soft_reset();
        break;
      default:
        break;      
    }
    uint16_t addr;
	int32_t  res;
	
    CV_TYPE cv_type = get_cv_type_by_idx(idx);
#ifdef DEBUG_CVM
    Serial.print(" cv_type: ");
    Serial.print(cv_type);
#endif
    switch (cv_type) {
	case CV_RW:
#ifdef DEBUG_CVM
		Serial.print(" RW ");
#endif
	
		lncv_val[idx] = val;
    	addr = sizeof(KeyType) + idx * sizeof(lncv_val[0]);
    	eeprom_write_block((void*)&lncv_val[idx], (void*)(addr), sizeof(lncv_val[0]) );
    	res = lncv_val[idx];
		break;
	case CV_RO:
#ifdef DEBUG_CVM
		Serial.print(" RO ");
#endif		
		res = -2;
		break;
	case CV_RO_EXT:
#ifdef DEBUG_CVM
		Serial.print(" RO_EXT ");
#endif		
		res = -3;
		break;
	case CV_RW_EXT:
#ifdef DEBUG_CVM
		Serial.print(" RW_EXT ");
#endif		
		if (notifyCVWriteExternal) res = notifyCVWriteExternal(lncv_num[idx], val);
		break;
	default:
		break;
    };
#ifdef DEBUG_CVM
	Serial.print(" res: ");
	Serial.println(res);
#endif
	if (res >= 0)
		if (notifyAfterCVWrite) notifyAfterCVWrite(lncv_num[idx], val);
	return res;
		
  };
  
  int32_t  LNCVManager::set_val_by_num(uint16_t num, uint16_t val){
#ifdef DEBUG_CVM
    Serial.print("set_val_by_num: n:");
    Serial.print(num);
    Serial.print(" v: ");
    Serial.println(val);
#endif    
    int32_t idx = get_idx_by_num(num);
#ifdef DEBUG_CVM
    Serial.print("idx: ");
    Serial.println(idx);
#endif
    if (idx < 0) return -1;
    return set_val_by_idx(idx,val);
  };
  
  int32_t  LNCVManager::get_idx_by_num(uint16_t lncv_num_in){
#ifdef DEBUG_CVM
    Serial.print(" num2idx: ");
    Serial.println(lncv_num_in);
#endif
    //check if it above that min CV number
    if (lncv_num[lncv_idx[0]] > lncv_num_in) return -1;
    
    //check if it more that min CV number
    if (lncv_num[lncv_idx[count-1]] < lncv_num_in) return -2;

    uint16_t a = 0;
    uint16_t b = count-1;
    
    uint16_t c = (b + a) / 2;
    uint16_t flg = 0;
    
    while (1) {
#ifdef DEBUG_CVM
        Serial.print("a: ");
        Serial.print(a);
        Serial.print(" i(a): ");
        Serial.print(lncv_idx[a]);
        Serial.print(" v(a): ");
        Serial.print(lncv_num[lncv_idx[a]]);
        
        Serial.print(" b: ");
        Serial.print(b);
        Serial.print(" i(b): ");
        Serial.print(lncv_idx[b]);
        Serial.print(" v(b): ");
        Serial.print(lncv_num[lncv_idx[b]]);
        
        Serial.print(" c: ");
        Serial.print(c);
        Serial.print(" i(c): ");
        Serial.print(lncv_idx[c]);
        Serial.print(" v(c): ");
        Serial.println(lncv_num[lncv_idx[c]]);
#endif

        if (lncv_num[lncv_idx[a]] == lncv_num_in) {
        //  Serial.println("a!");
          return lncv_idx[a];
        }
        if (lncv_num[lncv_idx[b]] == lncv_num_in) {
        //  Serial.println("b!");
          return lncv_idx[b];
        }
        if (lncv_num[lncv_idx[c]] == lncv_num_in) {
        //  Serial.println("c!");
          return lncv_idx[c];
        }
        
        if ( (b - a) <= 2) return -1;
        
        if (lncv_num[lncv_idx[c]] <= lncv_num_in) a = c; 
          else b = c;
        c = (b + a)/ 2;
#ifdef DEBUG_CVM
        Serial.print("END a: ");
        Serial.print(a);
        Serial.print(" b: ");
        Serial.print(b);
        Serial.print(" c: ");
        Serial.println(c);
#endif

    };
    
    return -1;
  };

  int32_t  LNCVManager::get_num_by_idx(uint16_t idx){
    if ((idx >= count)) return -1;
    return lncv_num[idx];

  };

  
  uint32_t  LNCVManager::get_val_by_num(uint16_t lncv_num){
    int32_t idx = get_idx_by_num(lncv_num);
    if (idx >= 0) 
		return get_val_by_idx(idx);
    else  
		return idx;
  };
  
  uint32_t  LNCVManager::get_val_by_idx(uint16_t idx){
  uint32_t	res;
  
    if (idx < count) {
	    CV_TYPE cv_type = get_cv_type_by_idx(idx);
	    switch (cv_type) {
		case CV_RW:
		case CV_RO:
			switch (lncv_num[idx]) {
				case LNCV_IDX_FIRMWARE_TYPE_ID:
					res =  ln_firmware_id;
					break;
				case LNCV_IDX_FIRMWARE_VER:
					res = ln_version_id;
					break;
				default:
					res = lncv_val[idx];
					break;
			};
			break;
		case CV_RO_EXT:
		case CV_RW_EXT:
			if (notifyCVReadExternal)
				res = notifyCVReadExternal(lncv_num[idx]);
			break;
		default:
			res = -2;
			break;
    	};
		
		if (notifyAfterCVRead) 
				notifyAfterCVRead(lncv_num[idx], res);

    }
//return lncv_val[idx];
      else res = -1;
	return res;
  }

  uint16_t LNCVManager::get_string_by_idx(uint16_t idx, char res[]){
    if ((idx >= count)) return 0;
    
    char * buf;
    char r[LNCV_TXT_MAX_LENGTH ] = "aa";
    if (idx < count_sys_cv) buf = & ln_sys[idx].txt[0];
      else buf = & ln_user[idx - count_sys_cv].txt[0] ;
    
    
    for (int i = 0; i < LNCV_TXT_MAX_LENGTH; i++)
        res[i]= pgm_read_byte_near(buf + i);
    return 1;
    
  }
  uint16_t LNCVManager::get_string_by_num(uint16_t lncv_num, char res[]){
    int32_t idx = get_idx_by_num(lncv_num);
    if (idx >= 0) return get_string_by_idx(idx, res);
      else 
    return 0;
  };
  


#endif
