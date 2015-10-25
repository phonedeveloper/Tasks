#include "Callback.h"
#include "Tasks.h"

Tasks led;

void setup() {
  pinMode(13, OUTPUT);
  // Start the chain of tasks that turn the LED on and off.
  blink(LOW);  
}

void loop() {
  // Check to see if time to call a scheduled task.
  led.dispatch(); 
}

void blink(int state) {
  digitalWrite(13, state);
  // Call blink() again in 250 ms with opposite LED state.
  led.schedule(blink, 250, !state); 
}

