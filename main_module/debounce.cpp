#include "debounce.h"

DebounceButton::DebounceButton(const int pin) {
  if ( pin != 99 ) 
    pinMode(pin,INPUT_PULLUP);
    
  this->pin = pin;
}

int DebounceButton::getPin() {
  return pin;
}

int DebounceButton::getState() {
  return current_state;
}

bool DebounceButton::checkState() {
  if ( pin == 99 ) return false;
  
  bool updated=false;
  
  if(millis() != time)
  {
    int reading = digitalRead(pin);

    if(reading == current_state && counter > 0)
    {
      counter--;
    }
    if(reading != current_state)
    {
       counter++;
    }
    // If the Input has shown the same value for long enough let's switch it
    if(counter >= DEBOUNCE_COUNT)
    {      
//      Serial.print("chagned ");
//      Serial.println(pin);

      counter = 0;
      current_state = reading;
      updated=true;
    }
    time = millis();
  }
  return updated;
}

//////////////////////////////



ButtonBuffer::ButtonBuffer(const int pins[], int count) {
    
    this->count = count;
    
    int i;
    for ( i=0 ; i < count ; i++ ) {
        buttons[i] = new DebounceButton(pins[i]);
    }
}

bool ButtonBuffer::hasNext() {
  return !queue.isEmpty();
}

void ButtonBuffer::waitNext() {
  while(queue.isEmpty()) updateButton();
}

bool ButtonBuffer::updateButton() {
  for ( int i=0 ; i < count; i++ ) {
    if ( buttons[i]->checkState() && buttons[i]->getState() == LOW ) 
        queue.push(i);
  } 
 
 return !queue.isEmpty(); 
}

void ButtonBuffer::emulateButton(int pin) {
  for ( int i=0 ; i < count; i++ ) {
    if ( buttons[i]->getPin() == pin ) 
        queue.push(i);
  } 
}

int ButtonBuffer::getNext(bool blocked) {
  updateButton();
  
  if ( queue.isEmpty() )
  {
    if ( !blocked ) 
      return -1;
    
    waitNext();
  }
  
  return queue.pop();
}

void ButtonBuffer::clean() {
  while(!queue.isEmpty() ) queue.pop();
}
