#include "storage.h"

DueFlashStorage dueFlashStorage;

// global
StorageClass Storage;

///////////////////////////////
// StorageClass implements

StorageClass::StorageClass() {
//  Serial.println("Storage constructer");

  this->current_bank = 0;
  this->current_ch = 0;
  
  for ( int i=0 ; i < TOTAL_BANK ; i++ ) {
    for ( int ch=0 ; ch < TOTAL_CHANNEL ; ch++ ) {
      memset(&(banks[i][ch]),0,sizeof(bank));
      strcpy(banks[i][ch].title, "No Name");
    }
  }
  
  editMode(false);
}

void StorageClass::formatFlash() {

  Storage.saveAll();

  // CHECK_ADDR에 0을 써서 데이터 초기화가 완료 되었음을 표시
  dueFlashStorage.write(CHECK_ADDR, 0);
}
//
//void StorageClass::save(int bank) {
//  byte b2[sizeof(bank)]; // 데이터를 바이트로 변환하기 위한 배열
//  int addr = bank * sizeof(bank);
//
//  //Serial.print("save to bank ");
//  //Serial.println(bank);
//
//  memcpy(b2, &banks[bank], sizeof(bank));
//  dueFlashStorage.write(START_ADDR+addr, b2, sizeof(bank));
//}
//
//void StorageClass::load(int bank) {
//  int addr = bank * sizeof(bank);
//
//  //Serial.print("load from bank ");
//  //Serial.println(bank);
//
//  byte *b2 = dueFlashStorage.readAddress(START_ADDR+addr);
//  memcpy(&banks[bank], b2, sizeof(bank));
//}

void StorageClass::saveAll() {
  byte b2[TOTAL_BYTESIZE]; // 데이터를 바이트로 변환하기 위한 배열

  //Serial.println("save to bank all");
  if ( editmode ) {
    editMode(false);
    getCurrentBank() = edit_bank;
  }

  memcpy(b2, banks, TOTAL_BYTESIZE);
  dueFlashStorage.write(START_ADDR, b2, TOTAL_BYTESIZE);
}

void StorageClass::loadAll() {

  //Serial.println("load from bank all");
  editMode(false);
  
  byte *b2 = dueFlashStorage.readAddress(START_ADDR);
  memcpy(banks, b2, TOTAL_BYTESIZE);
  
}

void StorageClass::init() {
  uint8_t codeRunningForTheFirstTime = dueFlashStorage.read(CHECK_ADDR); // 플래시는 처음 실행시 255가 들어 있음.

  if (codeRunningForTheFirstTime) {
    Storage.formatFlash();
  } else {
    Storage.loadAll();
  }

}

String StorageClass::dumpCurrent() {
  String dump;
  
  dump += getCurrentName();
  dump += ":";
  dump += getCurrentTitle();
  
  bank& current_bank = getCurrentBank();
  
  dump += ":";
  for ( int lp=0 ; lp < TOTAL_LOOP ; lp++ ) 
    dump += BOOL_TO_STRING(current_bank.loop[lp]);
  
  dump += ":"; 
  for ( int ctl=0 ; ctl < TOTAL_CONTROL ; ctl++ ) 
    dump += BOOL_TO_STRING(current_bank.ctl[ctl]);
  
  return dump;
}
