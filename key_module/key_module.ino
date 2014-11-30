#include <QueueList.h>
#include "debounce.h"



const int pins[] = { 12, 11, 10 };
ButtonBuffer buffer(pins, 3);

void setup() {
  Serial.begin(115200);
  
  pinMode(13, OUTPUT);
}

void loop() {
  if( buffer.updateButton() ) {
    int bt = buffer.getNext();
    Serial.print("button pressed: ");
    Serial.println(bt);

    if ( bt == 12 ) 
      digitalWrite(13, HIGH);
    else
      digitalWrite(13, LOW);
  }
}
