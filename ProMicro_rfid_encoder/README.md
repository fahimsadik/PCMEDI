🎵 PCMedi - RFID & Encoder HID
This project creates a compact, dual-function PC controller using a single Arduino Pro Micro. It serves as both a secure login device using an RFID tag and a physical volume knob using a rotary encoder.

# Features
RFID Login 💳: Scan a pre-registered RFID tag to automatically type a password into your computer. The device provides visual feedback with green and red LEDs for successful or failed scans.

Rotary Encoder Volume Control 🔊: A rotary encoder gives you tactile control over your PC's system volume.

Rotating the knob adjusts the volume up or down.

Pressing the knob's button mutes or unmutes the audio.

# How It Works
The device prioritizes RFID scanning. If no card is present, the 

loop() function continuously calls checkRotaryEncoder() to monitor for volume changes. When an RFID card is scanned, the code reads its unique ID and compares it to a stored value.

If the IDs match, it simulates keyboard presses to enter a password.

If they don't match, it flashes a red LED to indicate an error.

# Hardware & Connections
Arduino Pro Micro: The central component.

MFRC522 RFID Reader:


SDA (SS) ➡️ Pin 10 


RST ➡️ Pin 9 

SCK ➡️ Pin 15 (Hardware SPI)

MOSI ➡️ Pin 16 (Hardware SPI)

MISO ➡️ Pin 14 (Hardware SPI)

3.3V ➡️ VCC

GND ➡️ GND

Rotary Encoder:


CLK ➡️ Pin 2 (labeled pinA) 


DT ➡️ Pin 3 (labeled pinB) 


SW ➡️ Pin 4 (labeled buttonPin) 

LEDs:


Green LED ➡️ Pin 7 


Red LED ➡️ Pin 4 

# Setup Instructions 🛠️
Before uploading the code, you need to customize it with your own credentials:


RFID Tag UID: Change the rfid variable to match the unique ID of your tag.


Password: Modify the message variable inside the loop() to the password you want to be typed.
