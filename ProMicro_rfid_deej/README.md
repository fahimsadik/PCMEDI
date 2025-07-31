## PCMedi - Standalone HID Controller
# Introduction
This repository contains the firmware for a compact, all-in-one PC interaction tool built on a single Arduino Pro Micro. This standalone version of PCMedi combines two powerful features into one device: a multi-channel audio mixer for controlling per-application volume and a secure RFID tag reader for automating logins. By acting as a native Human Interface Device (HID), it requires no special drivers to function with your PC.

# What It Does
This controller provides two primary functions that run simultaneously:


Application Volume Control (Deej Mixer): The device reads the positions of five analog sliders and sends their values to the PC. This data is specifically formatted to work with the open-source 

deej software, allowing you to assign each slider to control the volume of a specific application (e.g., your game, music player, browser, etc.).


RFID Secure Login: An integrated MFRC522 RFID reader constantly scans for a tag. When you scan your specific, pre-registered tag, the device automatically types a password into your PC.

Visual Feedback: Two LEDs provide immediate visual feedback for login attempts. A green LED (

gled) signals a successful login , while a red LED (

rled) signals that an unrecognized tag was scanned.

# Code Explanation (rfid_deej.ino)
The firmware uses the 

HID-Project.h library to allow the Pro Micro to be recognized as a native keyboard by the PC.

The main loop() is designed for efficiency. If no RFID card is present, it continuously runs the 

updateSliderValues() and sendSliderValues() functions to act as a deej controller.

When a new RFID card is detected, the code:

Reads the card's Unique ID (UID).

Compares this UID to the hardcoded 

rfid string variable.

If the UIDs match, it uses the 

BootKeyboard functions to press Enter, type the predefined message character by character, and press Enter again to complete the login.

If the UIDs do not match, it flashes the red LED to indicate an error.

# Hardware and Connections
This project requires the following components connected to an Arduino Pro Micro:

MFRC522 RFID Reader


SDA (SS) -> Pro Micro Pin 10 


RST -> Pro Micro Pin 9 

SCK -> Pro Micro Pin 15 (Hardware SPI)

MOSI -> Pro Micro Pin 16 (Hardware SPI)

MISO -> Pro Micro Pin 14 (Hardware SPI)

3.3V -> Pro Micro VCC (or 3.3V pin)

GND -> Pro Micro GND

Analog Sliders (x5)

The wiper pin of each slider connects to pins 

A0, A1, A2, A3, and A4.

LEDs

The code uses 

gled (green LED) and rled (red LED) as status indicators. You must define these pins at the top of your sketch. For example:

C++

const int gled = 5;
const int rled = 6;
# How to Use
Configure Your Credentials: Before uploading, you must change two variables in the code:


String rfid = "21581167122";: Replace the number string with the UID from your own RFID tag.


const char* message = "2572002";: Replace the password with your own.

Deej Setup: To use the volume mixer, run the deej.exe application on your PC and configure it to listen to the COM port of your Arduino Pro Micro.

Login: To log in, simply click on a password field on your PC and scan your registered RFID tag.
