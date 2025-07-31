#include "HID-Project.h"
// Define Arduino pin numbers for buttons and LEDs
#define ROTARY_A 14
#define ROTARY_B 16
#define ROTARY_C 10

boolean A, a, B, b, C, c;
boolean D, d, E, e, F, f,g, G;

char line[80];
int n, leds;
unsigned long t;
int m, ledss;
unsigned long tt;
// int mm, ledsss;
// unsigned long ttt;

const int joystick_x_pin = A0;	
const int joystick_y_pin = A1;


int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis


void setup() {
//  Serial.begin(38400);
//  Serial.write("Starting...\n");
//  Serial.end();
   pinMode(ROTARY_A, INPUT_PULLUP);
  pinMode(ROTARY_B, INPUT_PULLUP);
  pinMode(ROTARY_C, INPUT_PULLUP);
  // pinMode(ROTARY_D, INPUT_PULLUP);
  // pinMode(ROTARY_E, INPUT_PULLUP);
  // pinMode(ROTARY_F, INPUT_PULLUP);
   

  a = b = c = false;
  d = e = f = g = false;
  t = 0;
  tt = 0;
  n = 0;
  m=0;

  Consumer.begin();
  BootKeyboard.begin();
//  System.begin(); // For System functions
  Gamepad.begin(); // For gamepad functions
  Mouse.begin(); // For mouse functions
//  AbsoluteMouse.begin(); // For the Absolute Mouse

}

void loop() {

  
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
