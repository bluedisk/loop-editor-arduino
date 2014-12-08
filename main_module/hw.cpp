#include <pins_arduino.h>
#include "main_config.h"
#include "storage.h"

extern bool tuner_enabled;

// Init Loop Hardware
void initLoopHW() {
  for ( int i=0 ; i < OUTPUT_PIN_COUNT ; i++ )
    pinMode(OUTPUT_PINS[i], OUTPUT);

  changeLoopHW();
}

// set all loop's hw status
void changeLoopHW() {
  
  // Mute ON
  digitalWrite(PIN_OUT_TLP, HIGH);
  delay(20);

  if ( tuner_enabled ) {
    digitalWrite(PIN_OUT_TUNER, tuner_enabled);
    
     for ( int i=0 ; i < TOTAL_LOOP ; i++ )
    digitalWrite(OUTPUT_PINS_LOOP[i], LOW);
  
    // change channel
    for ( int i=0 ; i < TOTAL_CHANNEL ; i++ )
      digitalWrite(OUTPUT_PINS_CHANNEL[i], LOW );
  
    // change controls
    digitalWrite(PIN_OUT_AMP, LOW);
    
  } else { 
    
    // change loop
    for ( int i=0 ; i < TOTAL_LOOP ; i++ )
      digitalWrite(OUTPUT_PINS_LOOP[i], Storage.getCurrentLoop(i));
  
    // change channel
    for ( int i=0 ; i < TOTAL_CHANNEL ; i++ )
      digitalWrite(OUTPUT_PINS_CHANNEL[i], ( i == Storage.getChannel() ) );
  
    // change controls
    digitalWrite(PIN_OUT_AMP, Storage.getCurrentCtl(CTL_IDX_AMP));
    digitalWrite(PIN_OUT_TUNER, tuner_enabled);
    
  }
  
  // Mute OFF
  delay(30);
  digitalWrite(PIN_OUT_TLP, LOW);
}
