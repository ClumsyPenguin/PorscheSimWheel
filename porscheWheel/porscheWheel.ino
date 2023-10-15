#include <Keypad.h>
#include <RotaryEncoder.h>

// Constants
#define ENABLE_PULLUPS
#define NUMBUTTONS 28 // Changed from 24 to 28 for 4 additional buttons
#define NUMROWS 5
#define NUMCOLS 2

#define PIN_IN1 15
#define PIN_IN2 16
#define PIN_IN3 17
#define PIN_IN4 18

// Button matrix configuration
byte buttons[NUMROWS][NUMCOLS] = {
  {0,1},
  {2,3},
  {4,5},
  {6,7},
  {8,9},
};

// Keypad and rotary encoder instantiation
Keypad buttbx = Keypad(makeKeymap(buttons), rowPins, colPins, NUMROWS, NUMCOLS);
RotaryEncoder encoder1(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);
RotaryEncoder encoder2(PIN_IN3, PIN_IN4, RotaryEncoder::LatchMode::TWO03); // Second encoder

void setup() {
  Serial.begin(9600);
  rotary_init();
}

void loop() { 
  checkAllButtons();
  checkEncoder(encoder1); // Encoder 1
  checkEncoder(encoder2); // Encoder 2
}

void checkAllButtons() {
  if (buttbx.getKeys()) {
    for (int i=0; i<LIST_MAX; i++) {
      if (buttbx.key[i].stateChanged) {
        switch (buttbx.key[i].kstate) {  
          case PRESSED:
          case HOLD:
            Serial.println("Button pressed");
            break;
          case RELEASED:
          case IDLE:
            break;
        }
      }   
    }
  }
}

void checkEncoder(RotaryEncoder& encoder) {
  static int pos = 0;
  encoder.tick();
  int newPos = encoder.getPosition();
  if (pos != newPos) {
    Serial.print("Encoder position: ");
    Serial.print(newPos);
    Serial.print(" Direction: ");
    Serial.println((int)(encoder.getDirection()));
    pos = newPos;
  }
}
