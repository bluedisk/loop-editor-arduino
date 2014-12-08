#include <memorysaver.h>
#include <UTFT.h>
#include <UTouch.h>
#include <QueueList.h>

// ADK
#include "variant.h"
#include <stdio.h>
#include <adk.h>

// Accessory descriptor. It's how Arduino identifies itself to Android.
char applicationName[] = "Monolab_Loop_Editor"; // the app on your phone
  char accessoryName[] = "Programmable loop"; // your Arduino board
char companyName[] = "Monolab";

// Make up anything you want for these
char versionNumber[] = "1.0";
char serialNumber[] = "1";
char url[] = "https://dl.dropboxusercontent.com/u/31577897/monolab/app-release.apk";

USBHost Usb;
ADK adk(&Usb, companyName, applicationName, accessoryName, versionNumber, url, serialNumber);

// Storage
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
UTouch  touch( T_CLK, T_CS, T_IN, T_OUT, T_IRQ);

ImageView view_loop1 = ImageView(344, 23, 41, 21, img_loop[0], img_loop_bg);
ImageView view_loop2 = ImageView(297, 23, 41, 21, img_loop[1], img_loop_bg);
ImageView view_loop3 = ImageView(250, 23, 41, 21, img_loop[2], img_loop_bg);
ImageView view_loop4 = ImageView(203, 23, 41, 21, img_loop[3], img_loop_bg);
ImageView view_loop5 = ImageView(156, 23, 41, 21, img_loop[4], img_loop_bg);
ImageView view_loop6 = ImageView(109, 23, 41, 21, img_loop[5], img_loop_bg);
ImageView view_loop7 = ImageView( 62, 23, 41, 21, img_loop[6], img_loop_bg);
ImageView view_loop8 = ImageView( 15, 23, 41, 21, img_loop[7], img_loop_bg);

ImageView view_bank = ImageView( 15,  82, 41, 16, img_bank);
ImageView view_lock = ImageView( 15, 197, 41, 21, img_lock);
ImageView view_usb  = ImageView( 63, 197, 41, 21, img_usb);
ImageView view_amp  = ImageView(206, 203, 60, 10, img_ampctl, img_ampctl_dim);
ImageView view_buff = ImageView(282, 203, 52, 10, img_buffer);
ImageView view_gate = ImageView(349, 203, 36, 10, img_gate);

TextView view_chan  = TextView(60,  82,  36, 27, "00", DotMatrix_M);
EditView view_title = EditView(15, 120, 370, 40, "-----------", Ubuntu);

TextView view_tuner  = TextView(75,  105,  300, 40, "TUNER MODE", Ubuntu);

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
bool tuner_enabled = false;

String nameEditText;

// 모든 모듈을 초기화 한다
void initialize() {

  // init usb
  cpu_irq_enable();
  
  // init view
  View::setLCD(&lcd);

  // 플래시 저장공간 초기화 (필요 시 포맷) & 저장된 데이터 읽어오기
  Storage.init();
  initLoopHW();

  lcd.InitLCD(LANDSCAPE);

  touch.InitTouch(LANDSCAPE);
  touch.setPrecision(PREC_MEDIUM);
}


void showLogo() {
  lcd.fillScr(0xEF7E);
  lcd.drawBitmap(90, 45, 219, 150, logo_only);
 
  delay(2000); 
  
  lcd.clrScr();
  lcd.setBackColor(0, 0, 0);
}


#define DUMP_BUFFER_SIZE    150
char dump_buffer[DUMP_BUFFER_SIZE];

void dumpStatus() {
  String dump = "[";
  
  dump += Storage.dumpCurrent();
  dump += ":";
  dump += BOOL_TO_STRING(tuner_enabled);
  dump += "]";  
  
  dump.toCharArray(dump_buffer, DUMP_BUFFER_SIZE);
  
  Serial.println(dump_buffer);
  if ( usb_connected ) adk.write(strlen(dump_buffer), (uint8_t *)dump_buffer);
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
  view_chan.setText(Storage.getCurrentName());

  //
  if ( view_title.blinkPos() == -1 )
    view_title.setText(Storage.getCurrentTitle());
  else
    view_title.setText(nameEditText);
}

void invalidateAll() {
  for ( int i=0 ; i < TOTAL_VIEWS ; i++ )
    views[i]->invalidate();
}

// 전체 화면 그리기
void updateScreen() {
  if ( tuner_enabled ) return;

  for ( int i=0 ; i < TOTAL_VIEWS ; i++ )
    views[i]->update();
}

// 범위 체크용 유틸함수
bool is_in_range(int value, int start, int count) {
  return (value >= start && value < (start+count) );
}

void tunerMode(boolean mode) {
  tuner_enabled = mode;
  
  if ( tuner_enabled ) {
    lcd.clrScr();    
    view_tuner.invalidate();
    view_tuner.update();
  } else {
    lcd.clrScr();    
    invalidateAll();
    updateScreen();
  }
}

//
//// 일반 버튼 눌렸을 때 처리
void fetchGeneralButton() {
  int idx = btn_buf.getNext();
  bool updateHW = false;
    
  if ( tuner_enabled ) {
    while ( idx != -1 ) {
      if ( idx == BTN_IDX_TUNER ) {
        tunerMode(false);
        changeLoopHW();
        dumpStatus();
        return;
      }
      
      idx = key_buf.getNext();
    }
    return;
  }

  while ( idx != -1 ) {

    if ( Storage.isLocked() && is_in_range(idx, BTN_IDX_CHAN_START, BTN_CNT_CHAN ) ) {

      // channel button
      const int ch_idx = (idx-BTN_IDX_CHAN_START);
      Storage.setChannel(ch_idx);
      
      updateHW = true;

    } else if ( Storage.isEditMode() && is_in_range(idx, BTN_IDX_LOOP_START, BTN_CNT_LOOP ) ) {

      // loop button
      const int loop_idx = (idx-BTN_IDX_LOOP_START);
      Storage.toggleLoop(loop_idx);
      
      updateHW = true;

    } else {
      switch (idx) {
        case BTN_IDX_TUNER:
          if ( Storage.isLocked() ) {
            tunerMode(true);
            updateHW = true;
          }
          break;

        case BTN_IDX_AMP:
          if ( Storage.isEditMode() ) {
            Storage.toggleControl(CTL_IDX_AMP);
            updateHW = true;
          }
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
  if ( updateHW ) changeLoopHW();
  updateViews();

  dumpStatus();
}

const String CHAR_LIST = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
const int CHAR_LIST_LEN = 62;

void incNameEditChar() {
  const int pos = view_title.blinkPos();    // 편집 중인 위치
  const char ch = nameEditText.charAt(pos); // 문자열상의 해당 문자
  int idx = CHAR_LIST.indexOf(ch);          // 해당문자의 문자표상의 위치
  
  idx++;
  if ( idx >= CHAR_LIST_LEN ) idx = 0;      // 초과시 맨앞으로
  
  nameEditText.setCharAt(pos, CHAR_LIST.charAt(idx));
}

void decNameEditChar() {  
  const int pos = view_title.blinkPos();
  const char ch = nameEditText.charAt(pos);
  int idx = CHAR_LIST.indexOf(ch);

  idx--;
  if ( idx < 0 ) idx = CHAR_LIST_LEN-1;
  
  nameEditText.setCharAt(pos, CHAR_LIST.charAt(idx));
}

// UI 키패드 버튼 눌렸을 때 처리
void fetchKeypadButton() {
  int idx = key_buf.getNext();


  if ( tuner_enabled ) {
    while ( idx != -1 ) {
      idx = key_buf.getNext();
    }
    return;
  }


  while ( idx != -1 ) {
    
    if ( idx == KEY_IDX_NOP ) {
      idx = key_buf.getNext();
      continue;
    }
    
    const int blink_pos = view_title.blinkPos();
    if ( blink_pos != -1 ) {
      
      switch(idx) {
        case KEY_IDX_UP:
          incNameEditChar();
          break;

        case KEY_IDX_DOWN:
          decNameEditChar();
          break;

        case KEY_IDX_RIGHT:
          Serial.println("next");
          view_title.blinkNext();
          break;

        case KEY_IDX_LEFT:          
          Serial.println("prev");
          view_title.blinkPrev();
          break;

        case KEY_IDX_DELETE:
          nameEditText = nameEditText.substring(0, blink_pos) + nameEditText.substring(blink_pos+1) + " ";
          break;

        case KEY_IDX_ENTER:
          view_title.blinkPos(-1);
          Storage.setCurrentTitle(nameEditText);
          break;
          
        case KEY_IDX_STORE:
          view_title.blinkPos(-1);
          Storage.setCurrentTitle(nameEditText);
          Storage.saveAll();
          break;
          
        case KEY_IDX_EXIT:
          view_title.blinkPos(-1);
          Storage.editMode(false);
          changeLoopHW();
      } // switch
      
    } else {
      switch(idx) {
        case KEY_IDX_EDIT:
          Storage.editMode(true);
          break;

        case KEY_IDX_STORE:
          Storage.saveAll();
          break;

        case KEY_IDX_EXIT:
          Storage.editMode(false);
          changeLoopHW();
          break;

        case KEY_IDX_NAME:
          if ( Storage.isEditMode() ) {
            view_title.blinkPos(0);
            nameEditText = Storage.getCurrentTitle();
            
            int padding = EDITVIEW_MAXLENGTH - nameEditText.length();
            for ( ; padding > 0 ; padding-- ) {
              nameEditText += ' ';
            }
          }
          break;
      } // switch
    } // if blankpos

    // next
    idx = key_buf.getNext();
  }

  updateViews();
  dumpStatus();
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

void processTextCmd(String& cmd) {
  
    Serial.print("cmd:");
    Serial.println(cmd);
  
    if ( cmd.startsWith("NOP") ) {
      Serial.println("NOP");
      dumpStatus();
    } else if (cmd.startsWith("NAME:") ) {
      String title = cmd.substring(5);

      Serial.print("name cmd:");
      Serial.println(title);

      Storage.editMode(true);
      Storage.setCurrentTitle(title);
      
      updateViews();
      dumpStatus();
      updateScreen();
    } else if (cmd.startsWith("KEY:") ) {
      int pin = cmd.substring(4).toInt();

      Serial.print("key cmd:");
      Serial.println(pin);
      
      key_buf.emulateButton(pin);
      btn_buf.emulateButton(pin);
    } else {
      Serial.println("error unknown cmd");
    }
    
}

///////////////////////
//
// ADK
//
//
#define ADK_RCVSIZE 128

String adk_line;
int adk_line_count=0;

uint8_t adk_buf[ADK_RCVSIZE];
uint32_t nbread = 0;

const char* hello="hello?\n";
 
void onADKReady() {
  
  adk.read(&nbread, ADK_RCVSIZE, adk_buf);
  
  if (nbread > 0)
  {
    for (uint32_t i = 0; i < nbread; ++i) {
      const char c = adk_buf[i];
      adk_line += c;
      if ( c == '\n') adk_line_count++;
    }
  } 
  
  while ( adk_line_count > 0 ) {
    int pos = adk_line.indexOf('\n');
    String cmd = adk_line.substring(0,pos);

    processTextCmd(cmd);

    adk_line = adk_line.substring(pos+1);
    adk_line_count--;
  }
}

void onADKConnect() {
  Serial.println("adk connect");
  usb_connected = true;
  updateViews();
}

void onADKDisconnect() {
  Serial.println("adk disconnect");
  usb_connected = false;
  updateViews();
}

///////////////////////
// Arduino Structure

void setup() {

  Serial.begin(115200);
  Serial.println(" -- boot! -- ");

  initialize();
  
  Serial.println(" -- inited! -- ");
  
  showLogo();

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
  
  Usb.Task();

  if (adk.isReady()) {
    if ( usb_connected ) 
      onADKReady();
    else
      onADKConnect();
  } else {
    if ( usb_connected )
      onADKDisconnect();
  } 
  
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

    processTextCmd(cmd);

    line = line.substring(pos+1);
    line_count--;
  }
}
