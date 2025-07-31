#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pins
const int VRX_PIN = A0;
const int VRY_PIN = A1;
const int BUTTON_PIN = 8;

#define SS_PIN 10
#define RST_PIN 9

const int trigPin = 6;
const int echoPin = 7;
const int wakeDistance = 30;

const int LEFT_THRESHOLD = 200;
const int RIGHT_THRESHOLD = 850;
const int UP_THRESHOLD = 850;
const int DOWN_THRESHOLD = 200;

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 20, 4);

bool displayAwake = false;
unsigned long displayTimeout = 0;
const unsigned long displayDuration = 10000;

int mode = 0;
int lastMode = -1;

String lastCommand = "";

// Arrow characters
byte arrowUp[8] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00100,
  B00000
};

byte arrowDown[8] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100,
  B00000
};

byte arrowLeft[8] = {
  B00100,
  B01000,
  B11111,
  B01000,
  B00100,
  B00010,
  B00001,
  B00000
};

byte arrowRight[8] = {
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B01000,
  B10000,
  B00000
};

void setup() {
  Serial.begin(9600);

  pinMode(VRX_PIN, INPUT);
  pinMode(VRY_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  SPI.begin();
  mfrc522.PCD_Init();

  lcd.init();
  lcd.backlight();

  // Load arrow characters
  lcd.createChar(0, arrowUp);
  lcd.createChar(1, arrowDown);
  lcd.createChar(2, arrowLeft);
  lcd.createChar(3, arrowRight);

  lcd.setCursor((20 - 15) / 2, 0);
  lcd.print(" WELCOME BOSS");
  lcd.setCursor((20 - 15) / 2, 2);
  lcd.print("   FAHIM SADIK   ");
  delay(500);
  lcd.clear();
}

void loop() {
  if (checkPresence()) {
    if (!displayAwake) {
      displayAwake = true;
      lcd.backlight();
      lcd.clear();
      lastMode = -1;
    }
    displayTimeout = millis() + displayDuration;
  }

  if (displayAwake) {
    checkRFID();
    handleJoystick();

    if (millis() > displayTimeout) {
      displayAwake = false;
      lcd.clear();
      lcd.noBacklight();
      lastMode = -1;
      lastCommand = "";
    }
  }

  delay(50);
}

bool checkPresence() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  long distance = duration * 0.034 / 2;
  return (distance > 0 && distance <= wakeDistance);
}

void handleJoystick() {
  int xValue = analogRead(VRX_PIN);
  int yValue = analogRead(VRY_PIN);
  static unsigned long lastButtonPress = 0;
  static bool buttonPressedLast = false;

  bool buttonPressed = (digitalRead(BUTTON_PIN) == LOW);
  if (buttonPressed && !buttonPressedLast && millis() - lastButtonPress > 500) {
    mode = (mode + 1) % 3;
    lastMode = -1;
    lastButtonPress = millis();
  }
  buttonPressedLast = buttonPressed;

  if (mode != lastMode) {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Mode ");
    lcd.print(mode);
    lcd.print(" selected        ");
    delay(800);
    lcd.clear();
    displayModeInfo(mode);
    lastCommand = "";
    lastMode = mode;
  }

  String command = "";
  switch (mode) {
    case 0:
      if (xValue > UP_THRESHOLD) command = "x1";          
      else if (xValue < DOWN_THRESHOLD) command = "x2";   
      else if (yValue > RIGHT_THRESHOLD) command = "y1";  
      else if (yValue < LEFT_THRESHOLD) command = "y2";   
      break;

    case 1:
      if (xValue > UP_THRESHOLD) command = "x3";          
      else if (xValue < DOWN_THRESHOLD) command = "x4";   
      else if (yValue > RIGHT_THRESHOLD) command = "y3";  
      else if (yValue < LEFT_THRESHOLD) command = "y4";   
      break;

    case 2:
      if (xValue > UP_THRESHOLD) command = "x5";
      else if (xValue < DOWN_THRESHOLD) command = "x6";
      else if (yValue > RIGHT_THRESHOLD) command = "y5";
      else if (yValue < LEFT_THRESHOLD) command = "y6";
      break;
  }

  if (command != "" && command != lastCommand) {
    Serial.println(command);
    delay(300);
  }
}

void displayModeInfo(int m) {
  lcd.clear();
  switch (m) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("Mode 0: Volume Ctrl");

      lcd.setCursor(0, 1);
      lcd.write(byte(0)); lcd.print(" Vol Up   ");
      lcd.write(byte(1)); lcd.print(" Vol Dn");

      lcd.setCursor(0, 2);
      lcd.write(byte(2)); lcd.print(" Vol Dn   ");
      lcd.write(byte(3)); lcd.print(" Vol Up");

      lcd.setCursor(0, 3);
      lcd.print(" SCAN RFID FOR PASS ");
      break;

    case 1:
      lcd.setCursor(0, 0);
      lcd.print("Mode 1: Media Ctrl");

      lcd.setCursor(0, 1);
      lcd.write(byte(0)); lcd.print(" Pause    ");
      lcd.write(byte(1)); lcd.print(" Play");

      lcd.setCursor(0, 2);
      lcd.write(byte(2)); lcd.print(" Prev     ");
      lcd.write(byte(3)); lcd.print(" Next");

      lcd.setCursor(0, 3);
      lcd.print(" SCAN RFID FOR PASS ");
      break;

    case 2:
      lcd.setCursor(0, 0);
      lcd.print("Mode 2: Custom Act");

      lcd.setCursor(0, 1);
      lcd.write(byte(0)); lcd.print(" x5       ");
      lcd.write(byte(1)); lcd.print(" x6");

      lcd.setCursor(0, 2);
      lcd.write(byte(2)); lcd.print(" y6       ");
      lcd.write(byte(3)); lcd.print(" y5");

      lcd.setCursor(0, 3);
      lcd.print(" SCAN RFID FOR PASS ");
      break;
  }
}

void checkRFID() {
  // Look for new card
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String card_ID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      card_ID += String(mfrc522.uid.uidByte[i], HEX);
    }

    Serial.println(card_ID);  // Send the UID over serial

    lcd.clear();

    // Match and display messages for specific cards
    if (card_ID == "b37855f7") {
      lcd.setCursor((20 - 15) / 2, 0);
      lcd.print("  Welcome Boss!  ");
      lcd.setCursor((20 - 15) / 2, 2);
      lcd.print("  Unlocking now  ");
    } else if (card_ID == "5387223f101") {
      lcd.setCursor((20 - 15) / 2, 0);
      lcd.print("  Welcome Guest  ");
      lcd.setCursor((20 - 15) / 2, 2);
      lcd.print("  Access granted ");
    } 
    else if (card_ID == "53eef73e101") {
      lcd.setCursor((20 - 15) / 2, 0);
      lcd.print("  Welcome Guest  ");
      lcd.setCursor((20 - 15) / 2, 2);
      lcd.print("  Access granted ");
    }else {
      lcd.setCursor((20 - 15) / 2, 0);
      lcd.print(" Unauthorized ID ");
      lcd.setCursor((20 - 15) / 2, 2);
      lcd.print(" Access Denied!  ");
    }

    delay(3000);  // Show the message for 2 seconds
    lcd.clear();
    displayModeInfo(mode);  // Return to current mode screen
    delay(1000);  // Delay to avoid multiple reads
  }
}

