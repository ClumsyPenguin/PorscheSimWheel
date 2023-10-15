#include <Keypad.h>
#include <RotaryEncoder.h>
#include <Joystick.h>

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

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_JOYSTICK, 32, 0, // Button Count, Hat Switch Count
  false, false, false, false, false, false, // X, Y, Z, Rx, Ry, and Rz Axis
  false, false, false,                     // rudder, throttle, and accelerator 
  false, false, false);                    // brake, steering, and virtual joystick

void setup() {
  Serial.begin(9600);
  rotary_init();
  
  // Set the Joystick to be a game controller.
  Joystick.begin();
}

void loop() { 
  checkAllButtons();
  checkEncoder(encoder1, 30, 31); // Encoder 1
  checkEncoder(encoder2, 32, 33); // Encoder 2
}

void checkAllButtons() {
  if (buttbx.getKeys()) {
    for (int i=0; i<LIST_MAX; i++) {
      if (buttbx.key[i].stateChanged) {
        bool isPressed = (buttbx.key[i].kstate == PRESSED) || (buttbx.key[i].kstate == HOLD);
        Joystick.setButton(buttbx.key[i].kchar, isPressed); // Set the button state in Joystick
      }   
    }
  }
}

void checkEncoder(RotaryEncoder& encoder, int buttonCW, int buttonCCW) {
  static int pos = 0;
  encoder.tick();
  int newPos = encoder.getPosition();
  if (pos != newPos) {
    if (newPos > pos) {
      Joystick.setButton(buttonCW, 1);
      delay(50); // Small delay to ensure the button press is detected by the game
      Joystick.setButton(buttonCW, 0);
    } else {
      Joystick.setButton(buttonCCW, 1);
      delay(50); // Small delay to ensure the button press is detected by the game
      Joystick.setButton(buttonCCW, 0);
    }
    pos = newPos;
  }
}