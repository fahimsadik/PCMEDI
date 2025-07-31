## PCMedi Foundation: Modular PC Controller
# Introduction
Welcome to the repository for the second generation of the PCMedi project. This version refines the concept of a custom PC controller by offering a modular firmware approach. The system is built upon a consistent hardware platform: an Arduino Pro Micro acts as the dedicated Human Interface Device (HID) connected to the PC, while an Arduino Uno serves as the main user-facing controller.

The key feature of this project is the choice between two different firmware options for the Uno, allowing you to select the functionality that best suits your needs: a streamlined, straightforward controller or an advanced version with a text-selection submenu.

# System Architecture
The project's intelligence is split between two devices, each with a specific job.

The Pro Micro (HID Brain - working_pro_micro_rotery_deej_uno_rfid_switchcase_extended.ino)
This board is the sole component connected to your PC. Its job is to listen for simple commands sent from the Uno and translate them into actions your computer understands, like typing on a keyboard or adjusting media volume. It also independently manages its own attached components: a rotary encoder for direct volume/mute control and four analog sliders configured to work with the 

deej.exe software for per-application volume mixing.

The Uno (The Main Controller)
This is the user interaction hub. It manages the LCD screen, reads the joystick, scans RFID cards, and detects user presence with an ultrasonic sensor. Based on your actions, it sends commands to the Pro Micro. You can choose one of two firmware versions for the Uno, each offering a different user experience.

# Uno Firmware Options
You can upload either of the following sketches to your Arduino Uno. Both are designed to work with the single provided Pro Micro sketch, but they offer different sets of features.

1. Basic Controller (uno.ino)
This firmware provides a clean and direct control experience. It is the simpler of the two options and works perfectly with the Pro Micro sketch out of the box.

Functionality:


Three Control Modes: Use the joystick button to cycle between Volume Control, Media Control, and Custom Actions. The LCD shows the function of each joystick direction for the current mode.


RFID Actions: When you scan a recognized RFID card, the Uno sends the card's unique ID directly to the Pro Micro. The Pro Micro then types a pre-programmed message or password associated with that specific ID.


Presence Detection: The LCD screen wakes up when you approach and turns off after a period of inactivity.

2. Advanced Controller with Submenu (uno2.ino)
This firmware builds upon the basic version by adding a powerful text-selection submenu, ideal for managing multiple passwords or text snippets with a single RFID card.

Functionality:

Includes all the features of the Basic Controller (three modes, presence detection, etc.).

Submenu Activation: Scanning a specific RFID card (53eef73e101) does not type a password directly. Instead, it activates a special 

submenu on the LCD screen.


Submenu Navigation: Once active, you can use the joystick to scroll up and down through a predefined list of text items (e.g., different passwords or commands).


Send Text: Pressing right on the joystick or clicking the button sends the currently selected text string from the list over the serial port to the Pro Micro.

‼️ Important Compatibility Note:
The submenu text-sending feature of the uno2.ino sketch will not work with the provided working_pro_micro_rotery_deej_uno_rfid_switchcase_extended.ino sketch as-is.


Reason: The Pro Micro code is programmed to listen for specific, known strings: either a card UID (like "b37855f7") or a joystick command (like "x1"). It does not have a case to handle the arbitrary text strings sent by the submenu (e.g., 

"Option One Text...").

Solution: To enable this feature, a small modification is needed in the Pro Micro's loop() function. You must add a final else condition that treats any unrecognized serial data as text to be typed.

C++

// In working_pro_micro_rotery_deej_uno_rfid_switchcase_extended.ino

if (Serial1.available() > 0) {
  String key = Serial1.readStringUntil('\n');
  key.trim();
  if (key == "b37855f7") {
    handleRFID(key);
  } else if (key == "53eef73e101") {
    handleRFID1(key);
  } else if (key == "x1" || key == "x2" /* ...and so on... */) {
    receiveAndExecuteCommandx(key);
  } else if (key == "y1" || key == "y2" /* ...and so on... */) {
    receiveAndExecuteCommandy(key);
  } else {
    // ADD THIS BLOCK TO MAKE THE SUBMENU WORK
    BootKeyboard.print(key); 
  }
}
# Connection and Workflow
User Input: You interact with the joystick or RFID scanner connected to the Uno.

Uno Processing: The Uno's firmware interprets the input.

If it's a joystick movement in a standard mode, it sends a command like "x1" over serial.

If it's an RFID scan, it sends the card's UID like "b37855f7" over serial.

If you are using uno2.ino and select from the submenu, it sends the full text like "Second Option" over serial.

Pro Micro Execution: The Pro Micro receives the string from the Uno. It checks the string against its known commands and executes the corresponding HID action (e.g., 

Consumer.write(MEDIA_VOLUME_UP) or BootKeyboard.write(...)).

# Wire Diagram
The physical wiring is the same for both Uno firmware options.

Inter-board Communication
Arduino Uno TX (Pin 1) -> Arduino Pro Micro RX (Pin 1)

Arduino Uno GND -> Arduino Pro Micro GND

Arduino Uno Connections
I2C LCD Display: SDA -> A4, SCL -> A5, VCC -> 5V, GND -> GND


Joystick Module: VRX -> A0, VRY -> A1, SW -> Pin 8 


MFRC522 RFID Reader: SDA(SS) -> Pin 10, SCK -> Pin 13, MOSI -> Pin 11, MISO -> Pin 12, RST -> Pin 9 


HC-SR04 Ultrasonic Sensor: Trig -> Pin 6, Echo -> Pin 7 

Arduino Pro Micro Connections

Rotary Encoder: CLK -> Pin 2, DT -> Pin 3, SW -> Pin 4 


Analog Sliders (x4): Wiper pins to A0, A1, A2, A3 

# Overview
PCMedi V2 is a versatile and adaptable project that puts you in control of your PC environment. By offering two distinct firmware paths for the main controller, it allows you to choose between simplicity and advanced features without changing any hardware. Whether you need a direct media and login tool or a more complex password manager, this project provides a solid foundation to build upon.
