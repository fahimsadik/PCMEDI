#include "HID-Project.h"
#include <ezButton.h>

const int modeButtonPin = 15; // Mode button connected to digital pin 2
const int rotarySwitchPin = A0; // Rotary switch connected to analog pin A0
int currentMode = 0; // Variable to track current mode


boolean A, a, B, b, C, c;
boolean D, d, E, e, F, f;

char line[80];
int n, leds;
unsigned long t;

const int joystick_x_pin = A0;	
const int joystick_y_pin = A1;


int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis

void setup() {
 Serial.begin(9600);

  pinMode(ROTARY_A, INPUT_PULLUP);
  pinMode(ROTARY_B, INPUT_PULLUP);
  pinMode(ROTARY_C, INPUT_PULLUP);
  pinMode(ROTARY_D, INPUT_PULLUP);
  pinMode(ROTARY_E, INPUT_PULLUP);
  pinMode(ROTARY_F, INPUT_PULLUP);
  pinMode(ROTARY_G, INPUT_PULLUP);

// pinMode(switchPin, INPUT_PULLUP);
  pinMode(modeButtonPin, INPUT_PULLUP); // Enable internal pull-up resistor for mode button


  a = b = c = false;
  d = e = f = false;
  t = 0;


  Consumer.begin();
  BootKeyboard.begin();

}

void loop() {

  if (digitalRead(modeButtonPin) == LOW) {
    currentMode++; // Increment mode
    if (currentMode > 2) { // If we've exceeded the number of modes, wrap back to 0
      currentMode = 0;
    }
    while(digitalRead(modeButtonPin) == LOW); // Wait for button release
    delay(100); // Debounce delay
  }


  xValue = analogRead(A0);
  yValue = analogRead(A1);




// first:
// if (int l==1){
switch (currentMode) {
    case 0:

  // lcd.setCursor(0, 0);  lcd.print(" Joy 1  ");
  // lcd.setCursor(0, 1);  lcd.print("DESKTOP ");
      
  Serial.print("Mode 0, Rotary Value: ");
  if (yValue > 900) {
    Keyboard.press(KEY_LEFT_ALT);
    delay(50);
    Keyboard.press(KEY_TAB);
  delay(500); // A little pause for dramatic effect
  Keyboard.releaseAll(); // Release both keys
  } 
 else if(yValue<100){
   Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
Keyboard.press(KEY_TAB);
 
delay(500); // A little pause for dramatic effect
Keyboard.releaseAll(); // Release both keys
  }
  if (xValue > 950) {
    Keyboard.press(KEY_UP_ARROW);
  } else if (xValue < 30) {
    Keyboard.press(KEY_DOWN_ARROW);
  } else {
    Keyboard.release(KEY_UP_ARROW);
    Keyboard.release(KEY_DOWN_ARROW);
  }
      
      break;
    case 1:
  // lcd.setCursor(0, 0);  lcd.print(" Joy 2  ");
  // lcd.setCursor(0, 1);  lcd.print("WEBSITE ");
  //     // Mode 1 behavior
      if (yValue > 900) {
    Keyboard.press(KEY_LEFT_CTRL);
Keyboard.press(KEY_TAB);
delay(500); // A little pause for dramatic effect
Keyboard.releaseAll(); // Release both keys
  } 
 else if(yValue<100){
   Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
Keyboard.press(KEY_TAB);
 
delay(500); // A little pause for dramatic effect
Keyboard.releaseAll(); // Release both keys
  }
  if (xValue > 950) {
    Keyboard.press(KEY_UP_ARROW);
  } 
  else if (xValue < 30) {
    Keyboard.press(KEY_DOWN_ARROW);
  } else {
    Keyboard.release(KEY_UP_ARROW);
    Keyboard.release(KEY_DOWN_ARROW);
  }
      
      break;
  // lcd.setCursor(0, 0);  lcd.print(" Joy 3  ");
  // lcd.setCursor(0, 1);  lcd.print("ELSE   ");
      // Mode 2 behavior
      Serial.print("Mode 2, Rotary Value: ");
      
      break;
    default:
      // Default behavior if an unknown mode is somehow selected
      
      break;
  
}
 

  if (A && !a) {
    if (B) {
      n++;
      Consumer.write(MEDIA_VOLUME_UP);
    }
    else {
      n--;
      Consumer.write(MEDIA_VOLUME_DOWN);
    }
 
  a = A;
  
  if (C != c) {
    if (C) {
      Consumer.write(MEDIA_VOLUME_MUTE);

    }
    delay(10);
  }
  c = C;
    
  if (millis() > t + 10) {
    t = millis();
    leds = 0;
    leds = BootKeyboard.getLeds();
   
  }

}
}


