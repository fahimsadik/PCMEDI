#include "HID-Project.h"
#include <ezButton.h>

#define NUM_SLIDERS 4
const int analogInputs[4] = { A0, A1, A2, A3 };
int analogSliderValues[4];

// Rotary Encoder Pins
const int pinA = 2;       // CLK
const int pinB = 3;       // DT
const int buttonPin = 4;  // SW
int lastStateCLK;
int currentStateCLK;
int lastButtonState = HIGH;
int currentButtonState;

void setup() {
  Serial.begin(9600);  // Initialize serial communications with the Uno
  Serial1.begin(9600);
  BootKeyboard.begin();
  Consumer.begin();
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  lastStateCLK = digitalRead(pinA);
}

void loop() {

  checkRotaryEncoder();

  if (Serial1.available() > 0) {
    String key = Serial1.readStringUntil('\n');
    key.trim();

    if (key == "d751a77a") {
      handleRFID(key);
    } else {

      if (key == "x1" || key == "x2") {
        receiveAndExecuteCommandx(key);
      } else if (key == "y1" || key == "y2") {
        receiveAndExecuteCommandy(key);
      }
    }
   
  }

  updateSliderValues();
  sendSliderValues();
  checkRotaryEncoder();
}

void handleRFID(String card_ID) {
  if (card_ID == "d751a77a") {  // Replace with your RFID UID
    BootKeyboard.write(KEY_RETURN);
    delay(1000);
    const char* message = "2572002";
    while (*message) {
      BootKeyboard.write(*message);  // Send each character
      delay(50);                     // Delay between characters (adjust as needed)
      message++;
    }
    BootKeyboard.write(KEY_RETURN);  // Press "Enter"
    delay(500);                      // Debounce delay
  } else {
    // Handle invalid RFID
  }
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    analogSliderValues[i] = analogRead(analogInputs[i]);
  }
}

void sendSliderValues() {
  String builtString = "SLIDERS:";
  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String(analogSliderValues[i]);
    if (i < NUM_SLIDERS - 1) {
      builtString += "|";
    }
  }
  Serial.println(builtString);
}

void checkRotaryEncoder() {
  currentStateCLK = digitalRead(pinA);

  // If the state has changed, a pulse occurred
  if (currentStateCLK != lastStateCLK) {
    if (digitalRead(pinB) != currentStateCLK) {
      Consumer.write(MEDIA_VOLUME_UP);
    } else {
      Consumer.write(MEDIA_VOLUME_DOWN);
    }
  }
  lastStateCLK = currentStateCLK;

  currentButtonState = digitalRead(buttonPin);
  if (currentButtonState != lastButtonState) {
    if (currentButtonState == LOW) {
      Consumer.write(MEDIA_VOLUME_MUTE);
    }
    lastButtonState = currentButtonState;
  }
}
void receiveAndExecuteCommandx(String key) {
  if (key == "x1") {
    Consumer.write(MEDIA_VOLUME_UP);
  } else if (key == "x2") {
    Consumer.write(MEDIA_VOLUME_DOWN);
  }
}
void receiveAndExecuteCommandy(String key) {
  if (key == "y1") {
    Consumer.write(MEDIA_VOLUME_UP);
  } else if (key == "y2") {
    Consumer.write(MEDIA_VOLUME_DOWN);
  }
}