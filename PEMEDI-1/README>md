🧠 PCMedi Files - Arduino-Based Multifunctional Control System
📌 Introduction
This repository contains your first PCMedi project files, focused on creating a multifunctional Arduino-based control system. It integrates rotary encoders, RFID tag detection, switch-case control, and LCD display output to act as a customizable input/output interface — ideal for PC control, automation, or prototyping.

The system uses both an Arduino Pro Micro (for Human Interface Device interactions) and an Arduino Uno (to handle RFID and display logic), allowing for modular and extendable hardware experimentation.

⚙️ What Does It Do?
This project combines input and output components to:

Read RFID cards to trigger specific modes or actions

Use rotary encoders and buttons for user input like volume control, scrolling, etc.

Display system information and feedback on a 20x4 I2C LCD

Switch between multiple control modes using a switch-case logic structure

Send keyboard/mouse commands to a PC using the Pro Micro’s HID capability

It acts as a versatile controller for your PC or embedded applications, offering interaction through RFID, rotary input, and LCD output.

🧾 Code Explanation
working_pro_micro_rotery_deej_uno_rfid_switchcase.ino
Written for the Arduino Pro Micro

Handles:

Rotary encoder input for precise control (volume, brightness, etc.)

Button press detection for triggering changes

Mode switching using a switch-case block

Serial communication with the Uno

PC input emulation using HID libraries like Keyboard.h or HID-Project.h

uno3.ino
Written for the Arduino Uno

Handles:

RFID detection using the MFRC522 module

20x4 I2C LCD display for feedback

LEDs or relays to indicate state changes or trigger outputs

Serial communication with the Pro Micro (sends RFID triggers or commands)

📺 What Does the Display Show?
The 20x4 I2C LCD, connected to the Arduino Uno, displays:

Current RFID tag identification (e.g., user name or function name)

Current system state or active control mode

Debug or feedback messages (e.g., “Card Detected”, “Mode: Volume”, etc.)

This allows for real-time monitoring of the system’s actions and user interactions.

🔌 Connection and Workflow
🔁 General Workflow:
Power on the system via USB or external power supply.

RFID card is scanned by the Uno, which identifies the tag.

Uno sends the tag ID or command to the Pro Micro via Serial.

Pro Micro responds by switching to the appropriate control mode.

Rotary encoder and button input are interpreted by the Pro Micro.

LCD (connected to Uno) displays current state or function.

Pro Micro sends output (e.g., volume up, tab switch) to the connected PC.

This creates a seamless multi-device controller with both local and PC-interfacing feedback.

🔧 Wiring Diagram
Here’s a general overview of the hardware connections:

Arduino Uno:
RC522 RFID Module

SDA → D10

SCK → D13

MOSI → D11

MISO → D12

RST → D9

VCC → 3.3V

GND → GND

20x4 I2C LCD

SDA → A4

SCL → A5

VCC → 5V

GND → GND

LEDs or Relays

Connected to D2–D6 (or other digital pins)

Arduino Pro Micro:
Rotary Encoder

CLK → D2

DT → D3

SW (Button) → D4

Serial Communication with Uno

RX/TX (consider voltage level shifting or use SoftwareSerial on Uno)

🧾 Overview
This project brings together multiple modules to create a programmable hardware interface:

Arduino Uno: Manages RFID input, LCD display, and sends command signals

Arduino Pro Micro: Reads user inputs and performs PC-side HID actions

Display & Feedback: LCD shows the current system state, while the Pro Micro handles interactive control

It’s designed with flexibility and expandability in mind, making it suitable for advanced PC controllers, automation hubs, or learning embedded systems.
