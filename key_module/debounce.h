#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include <pins_arduino.h>
#include <QueueList.h>

#define DEBOUNCE_COUNT 20


class DebounceButton {
  private:
    int pin;
    int counter = 0;
    int current_state = LOW;
    long time;

  public:
    DebounceButton(int pin);
    int getPin(); 
    
    int getState();
    bool checkState();
};

#define MAX_BUTTON_COUNT 20

class ButtonBuffer {

  private:
    QueueList <int> queue;
    
    DebounceButton *buttons[MAX_BUTTON_COUNT];
    int count;
    
 
  public:
     ButtonBuffer(int pins[], int count);
     
     bool hasNext();
     void waitNext();
     bool updateButton();
  
     int getNext(bool blocked=false);
     
     void clean();
};

#endif // DEBOUNCE_H
