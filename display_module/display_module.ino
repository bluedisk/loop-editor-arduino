#include <UTFT.h>

#include "img.h"
#include "view.h"

UTFT lcd(TFT01_32WD,38,39,40,41);

View view_loops[] = {
  View( 15, 23, 41, 21, img_loop[0]),
  View( 62, 23, 41, 21, img_loop[1]),
  View(109, 23, 41, 21, img_loop[2]),
  View(156, 23, 41, 21, img_loop[3]),
  View(203, 23, 41, 21, img_loop[4]),
  View(250, 23, 41, 21, img_loop[5]),
  View(297, 23, 41, 21, img_loop[6]),
  View(344, 23, 41, 21, img_loop[7]),
};

View view_status[] = {
  View( 15,  82, 41, 16, img_bank),
  View( 15, 197, 41, 21, img_lock),
  View( 63, 197, 41, 21, img_usb),
  View(206, 203, 60, 10, img_ampctl),
  View(282, 203, 52, 10, img_buffer),
  View(349, 203, 36, 10, img_gate),

};

#define VIEW_IDX_BANK  0
#define VIEW_IDX_LOCK  1
#define VIEW_IDX_USB   2
#define VIEW_IDX_AMP   3
#define VIEW_IDX_BUFF  4
#define VIEW_IDX_GATE  5

//
void drawLoops(bool data[]) {
  for ( int i=0 ; i < 8 ; i++ )
    if ( data[i] )
      view_loops[i].draw();
    else
      view_loops[i].clear();
}

void setup() {
  // put your setup code here, to run once:
  View::setLCD(&lcd); 

  lcd.InitLCD(LANDSCAPE_REVERT);
  lcd.clrScr();
  lcd.setBackColor(0, 0, 0);

  bool loopData[] = { 1, 1, 1, 1, 1, 1, 1, 1 };
  drawLoops(loopData);

  for ( int i=0 ; i < 6 ; i++ )
    view_status[i].draw();

  lcd.setColor(255, 255, 255);

  lcd.setFont(DotMatrix_M);
  lcd.print("3C",60,82);

  lcd.setFont(segment18_XXL);
  lcd.print("Plexi Disto",15,120);

}

void loop() {
}
