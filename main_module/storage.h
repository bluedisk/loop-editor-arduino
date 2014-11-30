#ifndef STORAGE_H
#define STORAGE_H

#include <DueFlashStorage.h>
#include <efc.h>
#include <flash_efc.h>

#include "main_config.h"

// 플래시 저장 방법 관련 선언
#define TITLE_MAX_LEN   32  // = [31 characters] + NULL
#define CHECK_ADDR      0
#define START_ADDR      4

#define TOTAL_CONTROL   4
#define CTL_IDX_AMP     0
#define CTL_IDX_BUFF    1
#define CTL_IDX_GATE    2
#define CTL_IDX_TUNE    3

// 뱅크 데이터 구조
struct bank {
  char title[TITLE_MAX_LEN];              // 32
  bool loop[TOTAL_CHANNEL][TOTAL_LOOP];   // + 32(4*8) = 64
  bool ctl[TOTAL_CONTROL];                // + 4 = 68
};

#define BANK_BYTESIZE    68               // must be multiplier of 4
#define TOTAL_BYTESIZE   68*TOTAL_BANK    // must be multiplier of 4


class StorageClass {

  private:
    bank banks[TOTAL_BANK];

    bank edit_bank;
    bool editmode;

    int current_bank;
    int current_ch;

    // name buffer
    char bank_addr_str[4];

  public:
    // 기본정보 가져오기용 유틸 메크로
    inline bank& getCurrentBank() { 
      if ( editmode ) 
        return edit_bank;
      else 
        return banks[current_bank]; 
    }
    
    inline bool& getCurrentLoop(const int loop_idx) { return getCurrentBank().loop[current_ch][loop_idx]; }
    inline bool& getCurrentCtl(const int ctl_idx)  { return getCurrentBank().ctl[ctl_idx]; }

    // 현재 설정 변경 용 유틸 함수들
    inline bool incBank() { if ( current_bank >= (TOTAL_BANK-1) ) return false; current_bank++; editMode(false); return true; }
    inline bool decBank() { if ( current_bank <= 0 ) return false; current_bank--; editMode(false); return true; }

    inline void setChannel(const int idx) { current_ch = idx; }
    inline void toggleLoop(const int idx) { if ( isLocked() ) return; getCurrentLoop(idx) = !getCurrentLoop(idx); }
    inline void toggleControl(const int idx) { if ( isLocked() ) return; getCurrentCtl(idx) = !getCurrentCtl(idx); }

    inline const char* getCurrentBankName() {
      bank_addr_str[0] = '1'+current_bank;
      bank_addr_str[1] = 'A'+current_ch;
      
      if ( !editmode ) {
        bank_addr_str[2] = '\0';
      } else {
        bank_addr_str[2] = '*';
        bank_addr_str[3] = '\0';
      }

      return bank_addr_str;
    }

    inline const char* getCurrentBankTitle() {
      return getCurrentBank().title;
    }
    
    inline bool isEditmode() {
      return this->editmode;
    }
    
    inline bool isLocked() {
      return !(this->editmode);
    }
    
    inline void editMode(int mode) {
      if ( this->editmode == mode ) return;
      
      if ( !this->editmode ) {
        // case of "lock => edit "
        edit_bank = getCurrentBank();
      }
      
      this->editmode = mode;
    }


    StorageClass();
    
    // 저장 관련 함수들 프로토 타입선언
    void formatFlash();
//    void save(const int bank);
//    void load(const int bank);
    void saveAll();
    void loadAll();
    void init();
    
    void dumpCurrent();

};

extern StorageClass Storage;

#endif // STORAGE_H
