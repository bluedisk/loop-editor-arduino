#ifndef MAIN_CONFIG_H
#define MAIN_CONFIG_H

#include "hw_config.h"

///////////////////////
// GENERAL

#define TOTAL_LOOP      8
#define TOTAL_CHANNEL   4
#define TOTAL_BANK      9
#define TOTAL_CONTROL   3

///////////////////////
// Display Module



///////////////////////
// Button Module

#define KEY_PIN_COUNT       11  // fix the array size to check repeatly for safety
#define BTN_PIN_COUNT       17   // fix array size to check repeatly for safety

#define BTN_IDX_CHAN_START  4               // channel button start index position
#define BTN_CNT_CHAN        TOTAL_CHANNEL   // total channel count
#define BTN_IDX_LOOP_START  8               // loop button start index position
#define BTN_CNT_LOOP        TOTAL_LOOP      // total loop count

#define BTN_IDX_TUNER       0
#define BTN_IDX_AMP         1
#define BTN_IDX_BKUP        2
#define BTN_IDX_BKDN        3

// button pins (defined in key_define.h

const int BTN_PINS[BTN_PIN_COUNT] = {
  KEY_TUNER, KEY_AMP, KEY_BKUP, KEY_BKDN,
  KEY_CHAN_A, KEY_CHAN_B, KEY_CHAN_C, KEY_CHAN_D,
  KEY_LOOP1, KEY_LOOP2, KEY_LOOP3, KEY_LOOP4,
  KEY_LOOP5, KEY_LOOP6, KEY_LOOP7, KEY_LOOP8
};

#define KEY_IDX_EDIT       0
#define KEY_IDX_NAME       1
#define KEY_IDX_STORE      2
#define KEY_IDX_UP         3
#define KEY_IDX_DOWN       4
#define KEY_IDX_LEFT       5
#define KEY_IDX_RIGHT      6
#define KEY_IDX_DELETE     7
#define KEY_IDX_ENTER      8
#define KEY_IDX_EXIT       9
#define KEY_IDX_NOP        10

const int KEY_PINS[KEY_PIN_COUNT] = {
  KEY_EDIT, KEY_NAME, KEY_STORE, KEY_UP,
  KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_DELETE,
  KEY_ENTER, KEY_EXIT, KEY_NOP
};

///////////////////////
// HW

#define OUTPUT_PIN_COUNT     15

const int OUTPUT_PINS[OUTPUT_PIN_COUNT] = {
    PIN_OUT_LOOP1, PIN_OUT_LOOP2, PIN_OUT_LOOP3, PIN_OUT_LOOP4,
    PIN_OUT_LOOP5, PIN_OUT_LOOP6, PIN_OUT_LOOP7, PIN_OUT_LOOP8,

    PIN_OUT_CHAN_A, PIN_OUT_CHAN_B, PIN_OUT_CHAN_C, PIN_OUT_CHAN_D,

    PIN_OUT_TLP, PIN_OUT_AMP, PIN_OUT_TUNER
};

const int OUTPUT_PINS_LOOP[] = {
    PIN_OUT_LOOP1, PIN_OUT_LOOP2, PIN_OUT_LOOP3, PIN_OUT_LOOP4,
    PIN_OUT_LOOP5, PIN_OUT_LOOP6, PIN_OUT_LOOP7, PIN_OUT_LOOP8,
};

const int OUTPUT_PINS_CHANNEL[] = {
    PIN_OUT_CHAN_A, PIN_OUT_CHAN_B, PIN_OUT_CHAN_C, PIN_OUT_CHAN_D
};

// prototype

void initLoopHW();
void changeLoopHW();

#endif // MAIN_CONFIG_H
