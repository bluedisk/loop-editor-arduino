#ifndef HW_CONFIG_H
#define HW_CONFIG_H

// LCD pins assign
#define LCD_RS      38
#define LCD_WD      39
#define LCD_CS      40
#define LCD_REST    41


// output pins assign
#define PIN_OUT_LOOP1       8
#define PIN_OUT_LOOP2       7
#define PIN_OUT_LOOP3       14
#define PIN_OUT_LOOP4       15
#define PIN_OUT_LOOP5       16
#define PIN_OUT_LOOP6       17
#define PIN_OUT_LOOP7       18
#define PIN_OUT_LOOP8       19

#define PIN_OUT_CHAN_A        9
#define PIN_OUT_CHAN_B        10
#define PIN_OUT_CHAN_C        11
#define PIN_OUT_CHAN_D        12

#define PIN_OUT_TLP         20
#define PIN_OUT_AMP         21
#define PIN_OUT_TUNER       13

// Key pins assign
#define KEY_EDIT    42
#define KEY_NAME    43
#define KEY_STORE   44
#define KEY_UP      45
#define KEY_DOWN    46
#define KEY_LEFT    47
#define KEY_RIGHT   48
#define KEY_DELETE  49
#define KEY_ENTER   50
#define KEY_EXIT    51

#define KEY_TUNER   A0
#define KEY_CHAN_D    A1
#define KEY_CHAN_C    A2
#define KEY_CHAN_B    A3
#define KEY_CHAN_A    A4
#define KEY_BKUP    A5
#define KEY_BKDN    A6
#define KEY_LOOP1   A7
#define KEY_LOOP2   A8
#define KEY_LOOP3   A9
#define KEY_LOOP4   A10
#define KEY_LOOP5   A11
#define KEY_LOOP6   DAC0
#define KEY_LOOP7   DAC1
#define KEY_LOOP8   CANRX
#define KEY_AMP     CANTX

#endif // HW_CONFIG_H