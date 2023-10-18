#include <Keypad.h>
#include <RotaryEncoder.h>

#define NUMBUTTONS 8
#define NUMROWS 5
#define NUMCOLS 2

#define PIN_IN1 20
#define PIN_IN2 21
#define PIN_IN3 22
#define PIN_IN4 23

// Button matrix configuration
byte buttons[NUMROWS][NUMCOLS] = {
  {0,1},
  {2,3},
  {4,5},
  {6,7},
  {8,9},
};


byte rowPins[NUMROWS] = {4,5,6,7}; 
byte colPins[NUMCOLS] = {2,3}; 

// Keypad and rotary encoder instantiation
Keypad buttbx = Keypad(makeKeymap(buttons), rowPins, colPins, NUMROWS, NUMCOLS);

RotaryEncoder encoder1(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);
RotaryEncoder encoder2(PIN_IN3, PIN_IN4, RotaryEncoder::LatchMode::TWO03);

void setup() {
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_IN1), getTick1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_IN2), getTick1, CHANGE);

  attachInterrupt(digitalPinToInterrupt(PIN_IN3), getTick2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_IN4), getTick2, CHANGE);
}

void loop() { 
  checkAllButtons();
  checkAllEncoders();
  // Read digital buttons and set joystick buttons
  Joystick.button(1, !digitalRead(8));
  Joystick.button(2, !digitalRead(9));
  Joystick.button(3, !digitalRead(10));
  Joystick.button(4, !digitalRead(11));

  // Delay to limit the loop speed
  delay(50);
}

void checkAllButtons() {
    if (buttbx.getKeys())
    {
       for (int i=0; i<LIST_MAX; i++)   
        {
           if ( buttbx.key[i].stateChanged )   
            {
            switch (buttbx.key[i].kstate) { 
                    case PRESSED:
                    case HOLD:
                              Joystick.button(buttbx.key[i].kchar + 5, 1);
                              break;
                    case RELEASED:
                    case IDLE:
                              Joystick.button(buttbx.key[i].kchar + 5, 0);
                              break;
            }
           }   
         }
     }
}

void checkAllEncoders(){
  static int pos = 0;  
  static int pos2 = 0;

   int newPos = encoder1.getPosition();
   int newPos2 = encoder2.getPosition();

   if (pos != newPos) {
    if (newPos < pos){
      Joystick.button(29, 1);
      delay(50);
      Joystick.button(29, 0);
    } 
    else if (newPos > pos){
      Joystick.button(30, 1);
      delay(50);
      Joystick.button(30, 0);
    } 
    pos = newPos;
   }   
   
   if (pos2 != newPos2) {
    if (newPos2 < pos2){
      Joystick.button(31, 1);
      delay(50);
      Joystick.button(31, 0);
    }
    else if (newPos2 > pos2){
      Joystick.button(32, 1);
      delay(50);
      Joystick.button(32, 0);
    }
    pos2 = newPos2;    
   } 
}

void getTick1(){
  encoder1.tick();    
}

void getTick2(){
  encoder2.tick();    
}
