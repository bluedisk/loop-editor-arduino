#include <UTFT.h>
#include "view.h"

UTFT *View::lcd = NULL;

View::View(int x, int y, int width, int height, unsigned short *img) {
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->img = img;
}

void View::draw() {
  SLCD->drawBitmap(x, y, width, height, img);
}

void View::clear() {
  SLCD->setColor(VGA_BLACK);
  SLCD->fillRect(x,y,x+width-1, y+height-1);
}
