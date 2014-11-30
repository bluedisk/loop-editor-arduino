#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include <pins_arduino.h>
#include <QueueList.h>

#define DEBOUNCE_COUNT 20


// debounce 처리가 된 버튼 눌림 체크 클래스
class DebounceButton {
  private:
    int pin;
    int counter = 0;
    int current_state = LOW;
    long time;

  public:
    DebounceButton(const int pin);
    int getPin();

    int getState();
    bool checkState();
};


#define MAX_BUTTON_LIMIT 30

// 눌린 버튼들을 체크 & 버퍼에 넣어서 순차 적으로 처리 할 수 있게 해주는 버퍼 클래스
class ButtonBuffer {

  private:
    QueueList <int> queue;

    DebounceButton *buttons[MAX_BUTTON_LIMIT];
    int count;


  public:
     ButtonBuffer(const int pins[], const int count);

     bool hasNext();
     void waitNext();
     bool updateButton();

     int getNext(bool blocked=false);

     void emulateButton(int pin);
     void clean();
};

#endif // DEBOUNCE_H
