#include "storage.h"

DueFlashStorage dueFlashStorage;


StorageClass Storage;

// globals
bank Storage::banks[TOTAL_BANK];

int Storage::current_bank = 0;
int Storage::current_ch = 0;

void Storage::formatFlash() {
  for ( int i=0 ; i < TOTAL_BANK ; i++ ) {
    memset(&banks[i],0,sizeof(bank));
    strcpy(banks[i].title, "No name");
  }

  Storage.saveAll();

  // CHECK_ADDR에 0을 써서 데이터 초기화가 완료 되었음을 표시
  dueFlashStorage.write(CHECK_ADDR, 0);
}

void Storage::save(int bank) {
  byte b2[sizeof(bank)]; // 데이터를 바이트로 변환하기 위한 배열
  int addr = bank * sizeof(bank);

  //Serial.print("save to bank ");
  //Serial.println(bank);

  memcpy(b2, &banks[bank], sizeof(bank));
  dueFlashStorage.write(START_ADDR+addr, b2, sizeof(bank));
}

void Storage::load(int bank) {
  int addr = bank * sizeof(bank);

  //Serial.print("load from bank ");
  //Serial.println(bank);

  byte *b2 = dueFlashStorage.readAddress(START_ADDR+addr);
  memcpy(&banks[bank], b2, sizeof(bank));
}

void Storage::saveAll() {
  byte b2[TOTAL_BYTESIZE]; // 데이터를 바이트로 변환하기 위한 배열

  //Serial.println("save to bank all");

  memcpy(b2, banks, TOTAL_BYTESIZE);
  dueFlashStorage.write(START_ADDR, b2, TOTAL_BYTESIZE);
}

void Storage::loadAll() {

  //Serial.println("load from bank all");

  byte *b2 = dueFlashStorage.readAddress(START_ADDR);
  memcpy(banks, b2, TOTAL_BYTESIZE);
}

void Storage::init() {
  uint8_t codeRunningForTheFirstTime = dueFlashStorage.read(CHECK_ADDR); // 플래시는 처음 실행시 255가 들어 있음.

  if (codeRunningForTheFirstTime) {
    Storage.formatFlash();
  } else {
    Storage.loadAll();
  }

}
