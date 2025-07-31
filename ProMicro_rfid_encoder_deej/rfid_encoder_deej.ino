#include "HID-Project.h"
#include <ezButton.h>
#include <SPI.h>
#include <MFRC522.h>  // RFID library

#define SS_PIN 10     // RX slave select
#define RST_PIN 9
#define NUM_SLIDERS 5
const int gled = 7;  // Optional green LED
const int rled = 4;  // Optional red LED
const int analogInputs[NUM_SLIDERS] = {A0, A1, A2, A3, A4};
int analogSliderValues[NUM_SLIDERS];

// Rotary Encoder Pins
const int pinA = 2; // CLK
const int pinB = 3; // DT
const int buttonPin = 4; // SW
int lastStateCLK;
int currentStateCLK;
int lastButtonState = HIGH;
int currentButtonState;

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
String card_ID = "";
String rfid = "21581167122";       // UID (unique Id Code Of Your Rfid Tag)
const char* message = "2572002";

void setup() {
  Serial.begin(9600);  // Initialize serial communications with the PC
  BootKeyboard.begin();
  SPI.begin();         // Init SPI bus
  mfrc522.PCD_Init();  // Init MFRC522 card
  pinMode(gled, OUTPUT);
  pinMode(rled, OUTPUT);
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  lastStateCLK = digitalRead(pinA);
}

void loop() {
  // Look for new card
  if (!mfrc522.PICC_IsNewCardPresent()) {
    updateSliderValues();
    sendSliderValues();
    checkRotaryEncoder();
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    updateSliderValues();
    sendSliderValues();
    checkRotaryEncoder();
    return;  // If read card serial(0) returns 1, the uid struct contains the ID of the read card.
  }
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    card_ID += mfrc522.uid.uidByte[i];
  }
  // Serial.println(card_ID);
  if (card_ID == rfid) {
    digitalWrite(gled, HIGH);
    BootKeyboard.write(KEY_RETURN);
    delay(1000);
    const char* message = "2572002";
    while (*message) {
      BootKeyboard.write(*message); // Send each character
      delay(50); // Delay between characters (adjust as needed)
      message++;
    }
    BootKeyboard.write(KEY_RETURN); // Press "Enter"
    delay(500); // Debounce delay

    digitalWrite(gled, LOW);
    delay(200);
    digitalWrite(gled, HIGH);
    delay(200);
    digitalWrite(gled, LOW);
  } else {
    digitalWrite(rled, HIGH);
    delay(200);
    digitalWrite(rled, LOW);
    delay(200);
    digitalWrite(rled, HIGH);
    delay(200);
    digitalWrite(rled, LOW);
  }
  delay(1000);
  card_ID = "";
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    analogSliderValues[i] = analogRead(analogInputs[i]);
  }
}

void sendSliderValues() {
  String builtString = "";
  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)analogSliderValues[i]);
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
    // Determine the rotation direction
    if (digitalRead(pinB) != currentStateCLK) {
      // Encoder rotated clockwise
      Consumer.write(MEDIA_VOLUME_UP);
    } else {
      // Encoder rotated counterclockwise
      Consumer.write(MEDIA_VOLUME_DOWN);
    }
  }
  lastStateCLK = currentStateCLK;

  // Check for button press to mute/unmute
  currentButtonState = digitalRead(buttonPin);
  if (currentButtonState != lastButtonState) {
    if (currentButtonState == LOW) {
      // Button pressed, mute/unmute
      Consumer.write(MEDIA_VOLUME_MUTE);
    }
    lastButtonState = currentButtonState;
  }
}
