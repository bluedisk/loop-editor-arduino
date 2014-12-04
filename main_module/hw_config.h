#ifndef HW_CONFIG_H
#define HW_CONFIG_H

// LCD pins assign
#define LCD_RS      38
#define LCD_WD      39
#define LCD_CS      40
#define LCD_REST    41

#define T_CLK   13
#define T_CS    12
#define T_IN    11
#define T_OUT   10
#define T_IRQ   9

// output pins assign
#define PIN_OUT_LOOP1       CANRX
#define PIN_OUT_LOOP2       DAC1
#define PIN_OUT_LOOP3       DAC0
#define PIN_OUT_LOOP4       A11
#define PIN_OUT_LOOP5       A10
#define PIN_OUT_LOOP6       A9
#define PIN_OUT_LOOP7       A8
#define PIN_OUT_LOOP8       A7

#define PIN_OUT_CHAN_A        5
#define PIN_OUT_CHAN_B        6
#define PIN_OUT_CHAN_C        8
#define PIN_OUT_CHAN_D        7

#define PIN_OUT_TLP         17
#define PIN_OUT_AMP         A6
#define PIN_OUT_TUNER       CANTX

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

#define KEY_TUNER     4
#define KEY_CHAN_D    A2
#define KEY_CHAN_C    A3
#define KEY_CHAN_B    A5
#define KEY_CHAN_A    A4
#define KEY_BKUP      A1
#define KEY_BKDN      A0
#define KEY_LOOP1     3
#define KEY_LOOP2     2
#define KEY_LOOP3     14
#define KEY_LOOP4     15
#define KEY_LOOP5     21
#define KEY_LOOP6     20
#define KEY_LOOP7     19
#define KEY_LOOP8     18
#define KEY_AMP       16

#endif // HW_CONFIG_H
