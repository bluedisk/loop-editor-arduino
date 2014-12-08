#include <UTFT.h>
#include "view.h"

UTFT *View::lcd = NULL;

View::View(int x, int y, int width, int height) {
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;

  invalidated = true;
  is_visible = true;
}

void View::visible(bool visible) {
  if ( this->is_visible == visible ) return;
  this->is_visible=visible;
  invalidate();
}

void View::invalidate() {
  invalidated=true;
}

void View::update() {
  postUpdate();
  
  if ( !invalidated ) return;
  invalidated=false;

  if ( is_visible )
    draw();
  else
    clear();
}

void View::postUpdate() {
  // do nothing
}

void View::clear() {
  SLCD->setColor(SLCD->getBackColor());
  SLCD->fillRect(x,y,x+width, y+height);
}

void View::draw() {
  SLCD->setColor(VGA_WHITE);
  SLCD->fillRect(x,y,x+width, y+height);
}

// touch

bool View::isTouched(int x, int y) {
  return (x >= this->x) && (x < (this->x + this->width)) && (y >= this->y) && (y < (this->y + this->height));
}

///////////////////////////
//
// ImageView
//

ImageView::ImageView(int x, int y, int width, int height, unsigned short *img, unsigned short *bg) : View(x, y, width, height) {
  this->img = img;
  this->bg = bg;
}

void ImageView::draw() {
    SLCD->drawBitmap(x, y, width, height, img);
}

void ImageView::clear() {
    if ( bg != NULL )
      SLCD->drawBitmap(x, y, width, height, bg);
    else
      View::clear();
}

///////////////////////////
//
// TextView
//

TextView::TextView(int x, int y, int width, int height, String text, unsigned char *font) : View(x, y, width, height) {
  this->text = text;
  this->font = font;
}

void TextView::setText(String text) {
  if ( this->text.compareTo(text) == 0 ) return;
  this->text = text;
  invalidate();
}

String& TextView::getText() {
  return this->text;
}

void TextView::draw() {

  SLCD->setColor(255, 255, 255);
  SLCD->setFont(font);
  SLCD->print(text, x, y);
}

/////////////////////////////
//
// EditView
//


EditView::EditView(int x, int y, int width, int height,String text, unsigned char *font) : TextView(x, y, width, height, text, font) {
  this->editpos = -1;
}

void EditView::blinkPos(int pos) {
  this->editpos = pos;
  this->blink_counter = 0;
  invalidate();
}

void EditView::blinkNext() {
  if ( this->editpos == -1 ) return;  
  
  if ( this->editpos > EDITVIEW_MAXLENGTH ) return;
  
  this->editpos++;
  invalidate();
}

void EditView::blinkPrev() {
  if ( this->editpos <= 0 ) return;

  this->editpos--;
  invalidate();
}


void EditView::setText(String text) {
  if ( this->text.compareTo(text) == 0 ) return;
  this->text = text.substring(0, EDITVIEW_MAXLENGTH);
  invalidate();
}

void EditView::draw() {
  TextView::draw();

  if ( editpos != -1 && blink_counter < EDITVIEW_BLINK_UNTIL ) {
    const int font_width = (int)this->font[0];
    const int font_height = (int)this->font[1];
    const int ex = x + (font_width * editpos);
    
    SLCD->setColor(0,0,0);
    SLCD->fillRect(ex, y, ex+font_width, y+font_height);
  } 
}

void EditView::postUpdate() {
  if ( editpos == -1 ) return;
  
  blink_counter++;

  if ( blink_counter == EDITVIEW_BLINK_UNTIL ) {
    invalidate();
  } else if ( blink_counter > EDITVIEW_SHOW_UNTIL ) {
    blink_counter = 0;
    invalidate();
  }
}

