#include "HID-Project.h"
#include <ezButton.h>

const int NUM_SLIDERS = 4;
const int analogInputs[NUM_SLIDERS] = { A0, A1, A2, A3};
int analogSliderValues[NUM_SLIDERS];

int lastStateCLK = HIGH;
int currentStateCLK = HIGH;
// Rotary encoder state tracking variables
int lastEncoded = 0;
int encoderValue = 0;

// Button state and debounce
#define DEBOUNCE_DELAY 2
unsigned long lastButtonDebounceTime = 0;
int lastButtonState = HIGH;
int currentButtonState = HIGH;



// Rotary Encoder Pins
const int pinA = 2;       // CLK
const int pinB = 3;       // DT
const int buttonPin = 4;  // SW


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

  //checkRotaryEncoder();

  if (Serial1.available() > 0) {
    String key = Serial1.readStringUntil('\n');
    key.trim();
    Serial.println(key);

  if (key.startsWith("R:")) {
    String uid = key.substring(2);
    if (uid == "b37855f7") {
      handleRFID(uid);
     
    } else if (uid == "5387223f101") {
      String uid = key.substring(2);
      handleRFID1(uid);
    }
  }else if (key.startsWith("J:")) {
    String swipe = key.substring(2);

      if (swipe == "x1" || swipe == "x2" ||swipe == "x3" || swipe == "x4"||
      swipe == "x5" || swipe == "x6") {
        receiveAndExecuteCommandx(swipe);
      } else if (swipe == "y1" || swipe == "y2"||swipe == "y3" || swipe == "y4"||
      swipe == "y5" || swipe == "y6") {
        receiveAndExecuteCommandy(swipe);
      }
    }else if (key.startsWith("T:")) {
    String text = key.substring(2);
    Keyboard.print(text);  // Send text as keystrokes
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
  if (card_ID == "5387223f101") {  // Replace with your RFID UID
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
  
  //Serial.println(builtString);
}

void checkRotaryEncoder() {
  // --- Rotary Encoder Gray Code Decoding ---
  int MSB = digitalRead(pinA);  // Most significant bit
  int LSB = digitalRead(pinB); // Least significant bit

  int encoded = (MSB << 1) | LSB;
  int sum = (lastEncoded << 2) | encoded;

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    encoderValue++;
    Consumer.write(MEDIA_VOLUME_UP);
  }
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    encoderValue--;
    Consumer.write(MEDIA_VOLUME_DOWN);
  }

  lastEncoded = encoded;

  // --- Button Debounce Logic ---
  currentButtonState = digitalRead(buttonPin);
  if ((currentButtonState != lastButtonState) && (millis() - lastButtonDebounceTime > DEBOUNCE_DELAY)) {
    lastButtonDebounceTime = millis();
    if (currentButtonState == LOW) {
      Consumer.write(MEDIA_VOLUME_MUTE);
    }
  }
  lastButtonState = currentButtonState;
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