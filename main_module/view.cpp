#include <UTFT.h>
#include "view.h"

UTFT *View::lcd = NULL;

View::View(int x, int y, int width, int height) {
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  
  invalidate = true;
  is_visible = true;
}

void View::visible(bool visible) {
  if ( this->is_visible == visible ) return;
  this->is_visible=visible;
  invalidate=true;
}

void View::update() {
  if ( !invalidate ) return;
  invalidate=false;
  
  if ( is_visible ) 
    draw();
  else 
    clear();
}

void View::clear() {
  SLCD->setColor(SLCD->getBackColor());
  SLCD->fillRect(x,y,x+width-1, y+height-1);
}

void View::draw() {
  SLCD->setColor(VGA_WHITE);
  SLCD->fillRect(x,y,x+width-1, y+height-1);
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
  invalidate = true;
}

void TextView::draw() {
  
  SLCD->setColor(255, 255, 255);
  SLCD->setFont(font);
  SLCD->print(text, x, y);
}
