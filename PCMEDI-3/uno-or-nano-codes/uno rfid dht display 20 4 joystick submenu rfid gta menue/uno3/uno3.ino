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
const int wakeDistance = 300;

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
// Add these global variables near the top
bool submenuActive = false;
int submenuIndex = 0;
const int submenuCount = 7;  // Adjust to your number of submenu items

String submenuItems[submenuCount] = {
  "S@dik25j2002",
  "S@dik25j2002sickboy",
  "S@dik25j2002sickboymadafaka",
  "fahim25j2002",
  "F@him25j2002",
  "F@him25j2002sickboy",
  "F@him25j2002sickboymadafaka"
};

bool gtaCheatsActive = false;
int gtaCheatIndex = 0;

const int gtaCheatsCount = 6;
String gtaCheats[gtaCheatsCount] = {
  "Max Health & Armor",
  "Give Weapons",
  "Lower Wanted Level",
  "Raise Wanted Level",
  "Super Jump",
  "Spawn Buzzard"
};

String gtaCheatsCodes[gtaCheatsCount] = {
  "TURTLE",    // Max Health & Armor
  "TOOLUP",    // Give Weapons
  "LAWYERUP",  // Lower Wanted Level
  "FUGITIVE",  // Raise Wanted Level
  "HOPTOIT",   // Super Jump
  "BUZZOFF"    // Spawn Buzzard
};

bool gtaSubmenuActive = false;
int gtaSubmenuIndex = 0;



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
  delay(2000);
  lcd.clear();
}

void loop() {
  checkRFID();
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

// Modify handleJoystick to handle submenu navigation
void handleJoystick() {
  int xValue = analogRead(VRX_PIN);
  int yValue = analogRead(VRY_PIN);
  static unsigned long lastButtonPress = 0;
  static bool buttonPressedLast = false;

  bool buttonPressed = (digitalRead(BUTTON_PIN) == LOW);

  // If submenu is active, joystick controls menu navigation
  if (submenuActive) {
    // Up navigation
    if (xValue > UP_THRESHOLD) {
      if (submenuIndex > 0) submenuIndex--;
      else submenuIndex = submenuCount - 1;
      displaySubmenuItem();
      delay(300);
    }
    // Down navigation
    else if (xValue < DOWN_THRESHOLD) {
      submenuIndex = (submenuIndex + 1) % submenuCount;
      displaySubmenuItem();
      delay(300);
    }
    // Left to exit submenu
    else if (yValue < DOWN_THRESHOLD) {  // Left
      submenuActive = false;
      lcd.clear();
      displayModeInfo(mode);
      delay(300);
    }
    // Right or button press to select/send text
    else if (yValue > UP_THRESHOLD || buttonPressed) {
      Serial.print("T:");
      Serial.println(submenuItems[submenuIndex]);  // Send text via serial
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Send");
      // Print submenu text on next lines (handle max 20 chars per line)
      String text = submenuItems[submenuIndex];
      int len = text.length();
      for (int row = 1; row < 4; row++) {
        lcd.setCursor(0, row);
        if (len > 20) {
          lcd.print(text.substring((row - 1) * 20, row * 20));
        } else {
          lcd.print(text);
          break;
        }
      }
      delay(500);
      displaySubmenuItem();  // Return to submenu display
      delay(150);
    }
  } else if (gtaSubmenuActive) {
    delay(150);
    // GTA Cheats submenu navigation
    if (xValue > UP_THRESHOLD) {
      if (gtaSubmenuIndex > 0) gtaSubmenuIndex--;
      else gtaSubmenuIndex = gtaCheatsCount - 1;

      displayGTACheatItem();
      delay(150);
    } else if (xValue < DOWN_THRESHOLD) {
      gtaSubmenuIndex = (gtaSubmenuIndex + 1) % gtaCheatsCount;
      displayGTACheatItem();
      delay(150);
    } else if (yValue < DOWN_THRESHOLD) {
      // Left to exit GTA submenu
      gtaSubmenuActive = false;
      mode = 0;  // go back to mode 0 (main menu or default)
      lcd.clear();
      displayModeInfo(mode);  // update display
      delay(300);
    } else if (yValue > UP_THRESHOLD || buttonPressed) {
      Serial.print("T:");
      Serial.println(gtaCheatsCodes[gtaSubmenuIndex]);
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Sent Cheat:");
      lcd.setCursor(0, 2);
      lcd.print(gtaCheats[gtaSubmenuIndex]);
      delay(1000);
      displayGTACheatItem();
      delay(300);
    }
  } else {
    // Normal mode handling
    if (buttonPressed && !buttonPressedLast && millis() - lastButtonPress > 500) {
      mode = (mode + 1) % 4;
      lastMode = -1;
      lastButtonPress = millis();
    }
    buttonPressedLast = buttonPressed;

    if (mode != lastMode) {
      lcd.clear();
      if (mode == 3) {
        gtaSubmenuActive = true;
        gtaSubmenuIndex = 0;
        displayGTACheatItem();
      } else {
        gtaSubmenuActive = false;
        lcd.setCursor(0, 1);
        lcd.print("Mode - ");
        lcd.print(mode);
        lcd.print(" selected        ");
        delay(800);
        lcd.clear();
        displayModeInfo(mode);
      }
      lastCommand = "";
      lastMode = mode;
    }
    if (!gtaSubmenuActive) {
      String command = "";
      switch (mode) {
        case 0:
          if (xValue > UP_THRESHOLD) command = "J:x1";
          else if (xValue < DOWN_THRESHOLD) command = "J:x2";
          else if (yValue > RIGHT_THRESHOLD) command = "J:y1";
          else if (yValue < LEFT_THRESHOLD) command = "J:y2";
          break;

        case 1:
          if (xValue > UP_THRESHOLD) command = "J:x3";
          else if (xValue < DOWN_THRESHOLD) command = "J:x4";
          else if (yValue > RIGHT_THRESHOLD) command = "J:y3";
          else if (yValue < LEFT_THRESHOLD) command = "J:y4";
          break;

        case 2:
          if (xValue > UP_THRESHOLD) command = "J:x5";
          else if (xValue < DOWN_THRESHOLD) command = "J:x6";
          else if (yValue > RIGHT_THRESHOLD) command = "J:y5";
          else if (yValue < LEFT_THRESHOLD) command = "J:y6";
          break;
      }

      if (command != "" && command != lastCommand) {
        Serial.println(command);
        delay(300);
      }
    }
  }
}

void displayModeInfo(int m) {
  lcd.clear();
  switch (m) {
    case 0:

      lcd.setCursor(0, 0);
      lcd.print("  Mode 0: Volume Ctrl");

      lcd.setCursor(0, 1);
      lcd.write(byte(0));
      lcd.print(" Vol Up   ");
      lcd.write(byte(1));
      lcd.print(" Vol Dn");

      lcd.setCursor(0, 2);
      lcd.write(byte(2));
      lcd.print(" Vol Dn   ");
      lcd.write(byte(3));
      lcd.print(" Vol Up");

      lcd.setCursor(0, 3);
      lcd.print("Press Button For menu");
      break;

    case 1:
      lcd.setCursor(0, 0);
      lcd.print(" Mode 1: Media Ctrl");

      lcd.setCursor(0, 1);
      lcd.write(byte(0));
      lcd.print(" Pause    ");
      lcd.write(byte(1));
      lcd.print(" Play");

      lcd.setCursor(0, 2);
      lcd.write(byte(2));
      lcd.print(" Prev     ");
      lcd.write(byte(3));
      lcd.print(" Next");

      lcd.setCursor(0, 3);
      lcd.print("Press Button For menu");
      break;

    case 2:
      lcd.setCursor(0, 0);
      lcd.print(" Mode 2: Custom Act");

      lcd.setCursor(0, 1);
      lcd.write(byte(0));
      lcd.print(" x5       ");
      lcd.write(byte(1));
      lcd.print(" x6");

      lcd.setCursor(0, 2);
      lcd.write(byte(2));
      lcd.print(" y6       ");
      lcd.write(byte(3));
      lcd.print(" y5");

      lcd.setCursor(0, 3);
      lcd.print("Press Button For menu");
      break;
  }
}

void displayGTACheatItem() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GTA Cheats:");
  lcd.setCursor(0, 1);
  lcd.print(gtaCheats[gtaSubmenuIndex]);
}
// Add this helper function to display current submenu item fully
void displaySubmenuItem() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Menu Item:");
  lcd.setCursor(0, 1);
  String text = submenuItems[submenuIndex];
  int len = text.length();
  if (len <= 20) {
    lcd.print(text);
    // Clear the rest of lines
    for (int i = 2; i < 4; i++) {
      lcd.setCursor(0, i);
      lcd.print("                    ");  // 20 spaces to clear line
    }
  } else {
    // If longer than 20 chars, split into 3 lines max
    for (int i = 0; i < 3; i++) {
      lcd.setCursor(0, i + 1);
      int start = i * 20;
      if (start >= len) break;
      int end = min(start + 20, len);
      lcd.print(text.substring(start, end));
    }
  }
}

// Modify checkRFID() so that scanning "53eef73e101" activates submenu
void checkRFID() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String card_ID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      card_ID += String(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.print("R:");
    Serial.println(card_ID);

    lcd.clear();

    if (card_ID == "b37855f7") {
      lcd.setCursor(0, 0);
      lcd.print("  CARD  Authorized  ");
      lcd.setCursor(0, 1);
      lcd.print("  Welcome Boss!  ");
      lcd.setCursor(0, 2);
      lcd.print("   FAHIM SADIK   ");
      lcd.setCursor((20 - 15) / 2, 3);
      lcd.print("  Unlocking now  ");
      submenuActive = false;
    } else if (card_ID == "5387223f101") {
      lcd.setCursor((20 - 15) / 2, 0);
      lcd.print("  CARD  Authorized  ");
      lcd.setCursor((20 - 15) / 2, 1);
      lcd.print("  Welcome Guest  ");
      lcd.setCursor((20 - 15) / 2, 2);
      lcd.print("  Access granted ");
      submenuActive = false;
    } else if (card_ID == "53eef73e101") {
      // Activate submenu mode here
      submenuActive = true;
      submenuIndex = 0;
      displaySubmenuItem();
      delay(1000);
      return;  // Early return, do not clear display after delay
    } else {
      lcd.setCursor((20 - 15) / 2, 0);
      lcd.print(" Unauthorized ID ");
      lcd.setCursor((20 - 15) / 2, 2);
      lcd.print(" Access Denied!  ");
      submenuActive = false;
    }

    delay(1000);
    if (!submenuActive) {
      lcd.clear();
      displayModeInfo(mode);
      delay(1000);
    }
  }
}