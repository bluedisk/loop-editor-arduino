#include <memorysaver.h>
#include <UTFT.h>
#include <UTouch.h>
#include <QueueList.h>

#include <DueFlashStorage.h>
#include <efc.h>
#include <flash_efc.h>

#include "storage.h"
#include "img.h"
#include "debounce.h"
#include "view.h"

// main defines
#include "main_config.h"

///////////////////////
// Display Module

UTFT    lcd(TFT01_32WD, LCD_RS, LCD_WD, LCD_CS, LCD_REST);
UTouch  touch( 6, 5, 4, 3, 2);

ImageView view_loop1 = ImageView( 15, 23, 41, 21, img_loop[0], img_loop_bg);
ImageView view_loop2 = ImageView( 62, 23, 41, 21, img_loop[1], img_loop_bg);
ImageView view_loop3 = ImageView(109, 23, 41, 21, img_loop[2], img_loop_bg);
ImageView view_loop4 = ImageView(156, 23, 41, 21, img_loop[3], img_loop_bg);
ImageView view_loop5 = ImageView(203, 23, 41, 21, img_loop[4], img_loop_bg);
ImageView view_loop6 = ImageView(250, 23, 41, 21, img_loop[5], img_loop_bg);
ImageView view_loop7 = ImageView(297, 23, 41, 21, img_loop[6], img_loop_bg);
ImageView view_loop8 = ImageView(344, 23, 41, 21, img_loop[7], img_loop_bg);

ImageView view_bank = ImageView( 15,  82, 41, 16, img_bank);
ImageView view_lock = ImageView( 15, 197, 41, 21, img_lock);
ImageView view_usb  = ImageView( 63, 197, 41, 21, img_usb);
ImageView view_amp  = ImageView(206, 203, 60, 10, img_ampctl, img_ampctl_dim);
ImageView view_buff = ImageView(282, 203, 52, 10, img_buffer);
ImageView view_gate = ImageView(349, 203, 36, 10, img_gate);

TextView view_chan  = TextView(60,  82,  36, 27, "00", DotMatrix_M);
TextView view_title = TextView(15, 120, 370, 40, "-----------", segment18_XXL);

#define TOTAL_VIEWS  16

View *views[] = {
 &view_loop1, &view_loop2, &view_loop3, &view_loop4,
 &view_loop5, &view_loop6, &view_loop7, &view_loop8,
 &view_bank, &view_lock, &view_usb,
 &view_amp, &view_buff, &view_gate,
 &view_chan, &view_title,
};

const int view_pins[] = {
  KEY_LOOP1, KEY_LOOP2, KEY_LOOP3, KEY_LOOP4,
  KEY_LOOP5, KEY_LOOP6, KEY_LOOP7, KEY_LOOP8,
  KEY_BKUP, KEY_EDIT, 0,
  KEY_AMP, 0, 0,
  0, KEY_NAME
};

///////////////////////
// Button Module

// 입력 핀들을 초기화하고 키 버퍼를 준비
ButtonBuffer key_buf(KEY_PINS, KEY_PIN_COUNT);
ButtonBuffer btn_buf(BTN_PINS, BTN_PIN_COUNT);


///////////////////////
// Program Main

bool usb_connected = false;

// 모든 모듈을 초기화 한다
void initialize() {

  View::setLCD(&lcd);

  // 플래시 저장공간 초기화 (필요 시 포맷) & 저장된 데이터 읽어오기
  Storage.init();
  initLoopHW();

  lcd.InitLCD(LANDSCAPE_REVERT);
  lcd.clrScr();
  lcd.setBackColor(0, 0, 0);

  touch.InitTouch(LANDSCAPE_REVERT);
  touch.setPrecision(PREC_MEDIUM);
}

// 데이터에 따라서 각뷰의 상태 업데이트
void updateViews() {

  for ( int i=0 ; i < TOTAL_LOOP ; i++ )
    views[i]->visible(Storage.getCurrentLoop(i));

  // status indicators
  view_usb.visible(usb_connected);

  // Lock indicator
  view_lock.visible(Storage.isLocked());

  view_amp.visible(Storage.getCurrentCtl(CTL_IDX_AMP));
  view_buff.visible(Storage.getCurrentCtl(CTL_IDX_BUFF));
  view_gate.visible(Storage.getCurrentCtl(CTL_IDX_GATE));

  //
  view_chan.setText(Storage.getCurrentBankName());

  //
  //char bank_title_str[12];
  //strncpy(bank_title_str, Storage.getCurrentBank().title, 11);
  view_title.setText(Storage.getCurrentBankTitle());
}

// 전체 화면 그리기
void updateScreen() {
  for ( int i=0 ; i < TOTAL_VIEWS ; i++ )
    views[i]->update();
}

// 범위 체크용 유틸함수
bool is_in_range(int value, int start, int count) {
  return (value >= start && value < (start+count) );
}

// 일반 버튼 눌렸을 때 처리
void fetchGeneralButton() {
    int idx = btn_buf.getNext();

    while ( idx != -1 ) {
      Serial.print("gen. keypad ");
      Serial.println(idx);

      if ( is_in_range(idx, BTN_IDX_CHAN_START, BTN_CNT_CHAN ) ) {

        // channel button
        const int ch_idx = (idx-BTN_IDX_CHAN_START);
        Storage.setChannel(ch_idx);

      } else if (is_in_range(idx, BTN_IDX_LOOP_START, BTN_CNT_LOOP ) ) {

        // loop button
        const int loop_idx = (idx-BTN_IDX_LOOP_START);
        Storage.toggleLoop(loop_idx);

      } else {
        switch (idx) {
          case BTN_IDX_TUNER:
            Storage.toggleControl(CTL_IDX_TUNE);
            break;

          case BTN_IDX_AMP:
            Storage.toggleControl(CTL_IDX_AMP);
            break;

          case BTN_IDX_BKUP:
            Storage.incBank();
            break;

          case BTN_IDX_BKDN:
            Storage.decBank();
            break;
        }
      }

      idx = btn_buf.getNext();
    } // while

    // update ui & loop hw
    changeLoopHW();
    updateViews();
    
    Storage.dumpCurrent();
}

// UI 키패드 버튼 눌렸을 때 처리
void fetchKeypadButton() {
  int idx = key_buf.getNext();

  while ( idx != -1 ) {
    Serial.print("ui keypad ");
    Serial.println(idx);
    
    switch(idx) {
      case KEY_IDX_EDIT:
        Serial.println("EDIT ON!");
        Storage.editMode(true);
        break;
        
      case KEY_IDX_STORE:
        Storage.saveAll();
        break;
        
      case KEY_IDX_EXIT:
        Storage.editMode(false);
        break;
//

      case KEY_IDX_UP:
        break;
      
      case KEY_IDX_DOWN:
        break;
      
      case KEY_IDX_RIGHT:
        break;
      
      case KEY_IDX_LEFT:
        break;
        
//
      case KEY_IDX_NAME:
        break;
        
      case KEY_IDX_DELETE:
        break;
      
      case KEY_IDX_ENTER:
        break;
      
    }
    
    // next
    idx = key_buf.getNext();
  }
  
  updateViews();
  Storage.dumpCurrent();
}

void onTouch(int x, int y) {
  for ( int i=0 ; i < TOTAL_VIEWS ; i++ ) {
    if ( views[i]->isTouched(x, y) ) {
      const int pin = view_pins[i];
      key_buf.emulateButton(pin);
      btn_buf.emulateButton(pin);
    }
  }
}

///////////////////////
// Arduino Structure

void setup() {

  Serial.begin(115200);
  Serial.println(" -- start -- ");
  Serial.println(A0);

  initialize();
  updateViews();
  updateScreen();

}

String line;
int line_count = 0;

int last_touch_x=0;
int last_touch_y=0;

int touch_x;
int touch_y;

void loop() {

  if (touch.dataAvailable()) {
      touch.read();
      touch_x=touch.getX();
      touch_y=touch.getY();

      if ( touch_x == -1 && touch_y == -1 )
        onTouch(last_touch_x, last_touch_y);


      last_touch_x = touch_x;
      last_touch_y = touch_y;
  }

  // key pad button check
  key_buf.updateButton();
  btn_buf.updateButton();

  // check next is not required in logic. only for save cpu time.
  if ( key_buf.hasNext() ) fetchKeypadButton();
  if ( btn_buf.hasNext() ) fetchGeneralButton();

  updateScreen();
}

void serialEvent() {

  if (Serial.available() >0) {
    char c = Serial.read();  //gets one byte from serial buffer
    if ( c == '\n') line_count++;
    line += c; //makes the string readString
  }

  while ( line_count > 0 ) {
    int pos = line.indexOf('\n');
    String cmd = line.substring(0,pos);

    int pin = cmd.toInt();

    Serial.print("input ");
    Serial.println(pin);

    key_buf.emulateButton(pin);
    btn_buf.emulateButton(pin);

    line = line.substring(pos+1);
    line_count--;
  }
}
