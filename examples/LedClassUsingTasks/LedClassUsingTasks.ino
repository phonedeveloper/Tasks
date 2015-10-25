#include <Callback.h>
#include <Tasks.h>

/*
 * An LED that can be blinked, turned on, or turned off.
 * This can be made into a library...
 * 
 * Tasks is used to handle the blinking by providing
 * a mechanism to change LED states without delay().
 */
class Led : public Callable {
  public:
    Led(int pin) : pin(pin) {
      pinMode(pin, OUTPUT);
    };
    void loop() {
      task.dispatch();
    };
    void blink() {
      if (!shouldBlink) {
        shouldBlink = true;
        callback(NULL); // start the blinking callback chain
      }
    };
    void on() {
      shouldBlink = false;
      state = HIGH;
      digitalWrite(pin, state);
    };
    void off() {
      shouldBlink = false;
      state = LOW;
      digitalWrite(pin, state);
    };
    void setBlinkRate(int delay) {
      this->delay = delay;
    };
  protected:
    void callback(void* pointer) {
      if (shouldBlink) {
        state = !state;
        digitalWrite(pin, state);
        task.schedule(this, delay);
      }
    };
  private:
    Tasks task;
    boolean shouldBlink = false;
    int pin;
    int state = LOW;
    int delay = 250;
};

//
// If the class above is made into a library,
// what remains goes into a sketch.
//

Led led(13);  // set pin 13 as an LED

void setup() {
  Serial.begin(9600);
}

void loop() {
  led.loop();  // handles LED state changes while blinking
  // look at serial port for LED commands
  while (Serial.available()) {
    switch (Serial.read()) {
      case '+':
        led.on();
        break;
      case '-':
        led.off();
        break;
      case 'b':
        led.blink();
        break;
    }
  }
}


