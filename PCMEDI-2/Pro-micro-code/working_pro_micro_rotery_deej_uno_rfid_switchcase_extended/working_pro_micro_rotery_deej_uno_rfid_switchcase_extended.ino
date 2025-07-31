#include "HID-Project.h"
#include <ezButton.h>

const int NUM_SLIDERS = 4;
const int analogInputs[NUM_SLIDERS] = { A0, A1, A2, A3};
int analogSliderValues[NUM_SLIDERS];

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

    if (key == "b37855f7") {
      handleRFID(key);
    } 
    if (key == "53eef73e101") {
      handleRFID1(key);
    } 
    
    else {

      if (key == "x1" || key == "x2" ||key == "x3" || key == "x4"||
      key == "x5" || key == "x6") {
        receiveAndExecuteCommandx(key);
      } else if (key == "y1" || key == "y2"||key == "y3" || key == "y4"||
      key == "y5" || key == "y6") {
        receiveAndExecuteCommandy(key);
      }
    }
   
  }

  updateSliderValues();
  sendSliderValues();
  checkRotaryEncoder();
delay(10);
}

void handleRFID(String card_ID) {
  if (card_ID == "b37855f7") {  // Replace with your RFID UID
    BootKeyboard.write(KEY_RETURN);
    delay(1000);
    const char* message = "the message";
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

void handleRFID1(String card_ID) {
  if (card_ID == "53eef73e101") {  // Replace with your RFID UID
    BootKeyboard.write(KEY_RETURN);
    delay(1000);
    const char* message = "S@dik25j2002sickboy";
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
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)analogSliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
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
  } else if (key == "x3") {
    Consumer.write(MEDIA_VOLUME_UP);
  } else if (key == "x4") {
    Consumer.write(MEDIA_VOLUME_DOWN);
  } else if (key == "x5") {
    Consumer.write(MEDIA_VOLUME_UP);
  } else if (key == "x6") {
    Consumer.write(MEDIA_VOLUME_DOWN);
  }
  

}
void receiveAndExecuteCommandy(String key) {
  if (key == "y1") {
    Consumer.write(MEDIA_VOLUME_UP);
  } else if (key == "y2") {
    Consumer.write(MEDIA_VOLUME_DOWN);
  } else if (key == "y3") {
    Consumer.write(MEDIA_VOLUME_UP);
  } else if (key == "y4") {
    Consumer.write(MEDIA_VOLUME_DOWN);
  } else if (key == "y5") {
    Consumer.write(MEDIA_VOLUME_UP);
  } else if (key == "y6") {
    Consumer.write(MEDIA_VOLUME_DOWN);
  }
}