#include <DueFlashStorage.h>
DueFlashStorage dueFlashStorage;

#define BANK_COUNT 9
#define START_ADDR 4

struct bank {
  char title[25];
  bool loop[4][8];
  bool ctl[3];
};

const int bank_size = sizeof(bank);
const int total_size = bank_size * BANK_COUNT;

bank banks[BANK_COUNT];

void initData() {
  for ( int i=0 ; i < BANK_COUNT ; i++ ) {
    memset(&banks[i],0,sizeof(bank));
    strcpy(banks[i].title, "No name");
  }
}

void save(int bank) {
  byte b2[sizeof(bank)]; // create byte array to store the struct
  int addr = bank * sizeof(bank);
  
  Serial.print("save to bank ");
  Serial.println(bank);
  
  memcpy(b2, &banks[bank], sizeof(bank));
  dueFlashStorage.write(START_ADDR+addr, b2, sizeof(bank)); 
}

void load(int bank) {
  int addr = bank * sizeof(bank);

  Serial.print("load from bank ");
  Serial.println(bank);
  
  byte *b2 = dueFlashStorage.readAddress(START_ADDR+addr); 
  memcpy(&banks[bank], b2, sizeof(bank));
}

void saveAll() {
  byte b2[total_size]; // create byte array to store the struct
  
  Serial.println("save to bank all");
  
  memcpy(b2, banks, total_size);
  dueFlashStorage.write(START_ADDR, b2, total_size); 
}

void loadAll() {
 
  Serial.println("load from bank all");
  
  byte *b2 = dueFlashStorage.readAddress(START_ADDR); 
  memcpy(banks, b2, total_size);
}

void initFlash() {
  uint8_t codeRunningForTheFirstTime = dueFlashStorage.read(0); // flash bytes will be 255 at first run
  
  if (codeRunningForTheFirstTime) {
    Serial.println("-- Format flash storage");
      initData();

//    for ( int i=0 ; i < BANK_COUNT ; i++ ) {
//      save(i);
//    }
//  
      saveAll();
    
    // write 0 to address 0 to indicate that it is not the first time running anymore
    dueFlashStorage.write(0, 0); 
  } else {
    Serial.println("-- Loading Saved Data");

//    for ( int i=0 ; i < BANK_COUNT ; i++ ) {
//      load(i);
//    }
    loadAll();
  }
  
}

void setup() {
  Serial.begin(115200);
  delay(500);
  
  Serial.println("");
  Serial.println("------------- BOOT -----------------");
  
  initFlash();

  Serial.println("INITIAL...");
  for ( int i=0 ; i<BANK_COUNT; i++) {
    Serial.print("bank ");
    Serial.print(i);
    Serial.print(" title: ");
    Serial.println(banks[1].title);
  }
//  
//  Serial.println("UPDADING...");
//  for ( int i=0 ; i<BANK_COUNT; i++) {
//    sprintf(banks[i].title, "TEST: %d",i);
//   
//    
//  }
//  saveAll();
//  
//   Serial.println("RESULT...");
//
//  for ( int i=0 ; i<BANK_COUNT; i++) {
//    Serial.print("bank ");
//    Serial.print(i);
//    Serial.print(" title: ");
//    Serial.println(banks[1].title);
//  }
}

void loop() {
  // put your main code here, to run repeatedly:
  while(1){};
}
