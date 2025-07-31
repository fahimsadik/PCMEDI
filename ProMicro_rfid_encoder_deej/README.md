🎛️ PCMedi - All-in-One HID Controller
This project combines a multi-channel volume mixer, a secure RFID login device, and a physical volume knob into a single, powerful PC controller using just one Arduino Pro Micro.

# Features
Per-Application Volume Control 🎚️: Five analog sliders send data to your PC, compatible with the deej software for controlling individual application volumes.

RFID Secure Login 💳: Scan a pre-registered RFID tag to automatically type a password on your computer.

Rotary Encoder Volume Control 🔊: A rotary encoder provides tactile control over your PC's master volume.

Rotate to adjust volume up or down.

Press to mute or unmute audio.

Visual Feedback 🚦: Green and red LEDs provide instant confirmation of successful or failed RFID scans.

How It Works
The device is designed to be highly responsive. In its main loop, if no RFID card is detected, it continuously reads the sliders and the rotary encoder, sending volume data to the PC. When an RFID card is scanned, it momentarily pauses the other functions to read the card's ID.

If the ID matches the one stored in the code, it uses the BootKeyboard library to type your password; otherwise, it flashes a red LED to signal an error.

# Hardware & Connections
Arduino Pro Micro: The core of the project.

MFRC522 RFID Reader:


SDA (SS) ➡️ Pin 10 


RST ➡️ Pin 9 

SCK ➡️ Pin 15 (Hardware SPI)

MOSI ➡️ Pin 16 (Hardware SPI)

MISO ➡️ Pin 14 (Hardware SPI)

Analog Sliders (x5):

Wiper pins connect to 

A0, A1, A2, A3, and A4.

Rotary Encoder:


CLK ➡️ Pin 2 


DT ➡️ Pin 3 


SW ➡️ Pin 4 

LEDs:


Green LED ➡️ Pin 7 


Red LED ➡️ Pin 4 

# Setup Instructions 🛠️
Before uploading, you need to customize the code with your information:


RFID Tag UID: In the rfid variable, replace the default number string with the unique ID from your tag.


Password: Change the message variable to the password you want the device to type.
