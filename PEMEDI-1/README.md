## PC Media Controller & Automation Tool (PCMedi)
# Introduction
Welcome to the repository for PCMedi, a multifaceted PC interaction tool built with Arduino. This project combines an Arduino Uno and a Pro Micro to create a sophisticated, custom-built controller that goes far beyond a simple keyboard. It serves as a media controller, a password manager, a game cheat activator, and a macro pad, all wrapped in an intuitive physical interface featuring an LCD screen, joystick, rotary encoder, and RFID scanner. The system is designed to be interactive, waking up automatically when a user approaches, and providing clear feedback for every action.

# What Does It Do?
This system is a powerful bridge between physical controls and your computer, designed to streamline common tasks and enhance your PC experience.

PC Control Hub: At its core, it's a versatile controller. You can manage your PC's volume, play/pause media, and skip tracks using a joystick and a dedicated rotary encoder.

Application Volume Control with deej: The four analog sliders are configured to work with a popular open-source software called deej.exe. This allows you to map each slider to control the volume of individual applications on your PC—for instance, you can have one slider for your game, another for music, and a third for your web browser, giving you a physical audio mixing board for your running programs.


Multi-Mode Functionality: The joystick's function can be changed on the fly by pressing its button, which cycles through different modes. You can switch between 

Volume Control , 

Media Control , and a set of 

Custom Actions that you can define.


RFID-Based Automation: The integrated RFID reader allows for secure and swift actions.


User Authentication: Scan a registered card to "unlock" the device, displaying a personalized welcome message. Different cards can be assigned different roles (e.g., "Boss" vs. "Guest").


Password Entry: Scan a specific RFID card to bring up a menu of predefined text strings, such as complex passwords or email addresses. Select an item with the joystick to have it instantly typed on your PC.


Game Cheat Menu: A dedicated mode provides a menu of GTA cheat codes. Simply navigate the list with the joystick and select a cheat to have it typed into the game instantly.


Smart Display: A 20x4 LCD screen provides a rich user interface, showing current modes, menus, and status updates. It automatically wakes up when you get close and sleeps when you're away, thanks to an ultrasonic presence sensor.

# Code Explanations
The project's logic is split between two microcontrollers: the Arduino Uno, which acts as the main controller and interface, and the Arduino Pro Micro, which acts as the Human Interface Device (HID) that communicates with the PC.

Arduino Uno (The Controller - uno3.ino)
The Uno is the brain of the operation. Its primary responsibilities are:


Reading Inputs: It constantly monitors the joystick, the RFID scanner, and the ultrasonic distance sensor.


Managing the UI: It controls everything shown on the 20x4 I2C LCD screen. This includes welcome messages, mode information, and navigating the password and GTA cheat menus.


State Management: It keeps track of the current control mode (e.g., Volume, Media, etc.) and whether a special submenu is active.


Sending Commands: Based on the user's actions, the Uno sends short, simple command strings to the Pro Micro via a serial connection. These commands are prefixed to tell the Pro Micro what kind of action to perform:


R: for an RFID card UID.


J: for a joystick movement.


T: for text that needs to be typed.

Arduino Pro Micro (The HID - working_pro_micro_rotery_deej_uno_rfid_switchcase.ino)
The Pro Micro’s sole purpose is to pretend to be a keyboard, a media controller, and a generic HID for the sliders, using the 

HID-Project.h library.


Receiving Commands: It listens for serial commands sent from the Uno on Serial1.

Executing Actions: When a command is received, it parses the prefix and executes the corresponding action:

If it receives 

R:, it checks the UID and types a predefined message for specific cards.

If it receives 

J:, it triggers a media control action, like MEDIA_VOLUME_UP or MEDIA_VOLUME_DOWN.

If it receives 

T:, it types out the received text character by character, emulating a keyboard.

Direct Inputs: The Pro Micro also manages its own set of inputs independently of the Uno:

A 

Rotary Encoder is used for fine-grained volume up/down control.

The 

Encoder's Button is used to mute/unmute the volume.


Four Analog Sliders are read. The 

sendSliderValues() function then formats their values into a string separated by a | character (e.g., 1023|512|0|800). This specific format is designed to be read by the 

deej.exe application on the host PC, which then maps these values to control the volume of different programs.

# What Does the Display Show?
The 20x4 LCD is the primary visual feedback mechanism:


Welcome Message: Upon startup, it displays a "WELCOME BOSS" message.


Presence Detection: The display and its backlight turn on when the ultrasonic sensor detects a user nearby and turn off after a period of inactivity.


Mode Information: When a mode is selected, the screen clears and shows what each joystick direction does. For example, in 

Mode 0, it shows arrow icons indicating that up/right increases volume and down/left decreases it.


RFID Status: When an RFID card is scanned, the screen immediately shows the result: "CARD Authorized" for recognized cards or "Unauthorized ID" for unknown ones.


Submenus: When a special menu is activated (either by RFID for passwords or by selecting Mode 3 for cheats), the display shifts to a menu interface. It shows the currently selected item, which can be navigated using the joystick. When an item is selected, a confirmation message like "Sent Cheat:" is shown.

Connection and Workflow
The system's workflow is a clear example of distributed tasks between two microcontrollers.

User Interaction: The user provides an input to the Uno—by moving the joystick, pressing its button, or scanning an RFID card.

Uno Processes: The Uno's code identifies this input. It checks the current mode and determines what to do. For instance, if the joystick is moved up in Mode 0, the Uno knows this means "Volume Up".


Serial Communication: The Uno translates this action into a simple command string (e.g., "J:x1") and sends it over its TX pin to the Pro Micro's RX pin.


Pro Micro Executes: The Pro Micro receives "J:x1". Its code sees the 

J: prefix, looks up what x1 corresponds to, and executes the command Consumer.write(MEDIA_VOLUME_UP).

PC Responds: The PC receives the command from the Pro Micro (which it thinks is a standard media keyboard) and increases the system volume.

This one-way command flow is efficient. Meanwhile, the Pro Micro handles its dedicated tasks. For the sliders, the workflow is: The Pro Micro reads the analog values , formats them into a 

|-separated string, and this string is read by the 

deej.exe software on the PC to adjust application volumes.

# Wire Diagram
Here is a description of the necessary connections between the components.

Inter-board Communication
Arduino Uno TX (Pin 1) -> Arduino Pro Micro RX (Pin 1)

Arduino Uno GND -> Arduino Pro Micro GND

Arduino Uno Connections
I2C LCD Display:

SDA -> Uno Pin A4

SCL -> Uno Pin A5

VCC -> Uno 5V

GND -> Uno GND

Joystick Module:

VRX -> Uno Pin A0 

VRY -> Uno Pin A1 

SW -> Uno Pin 8 

VCC -> Uno 5V

GND -> Uno GND

MFRC522 RFID Reader:

SDA (SS) -> Uno Pin 10 

SCK -> Uno Pin 13

MOSI -> Uno Pin 11

MISO -> Uno Pin 12

RST -> Uno Pin 9 

3.3V -> Uno 3.3V

GND -> Uno GND

HC-SR04 Ultrasonic Sensor:

Trig -> Uno Pin 6 

Echo -> Uno Pin 7 

VCC -> Uno 5V

GND -> Uno GND

Arduino Pro Micro Connections
Rotary Encoder:

CLK -> Pro Micro Pin 2 

DT -> Pro Micro Pin 3 

SW -> Pro Micro Pin 4 

-> Pro Micro 5V

GND -> Pro Micro GND

Analog Sliders (x4):

Slider 1 Wiper -> Pro Micro Pin A0 

Slider 2 Wiper -> Pro Micro Pin A1 

Slider 3 Wiper -> Pro Micro Pin A2 

Slider 4 Wiper -> Pro Micro Pin A3 

(All sliders share a connection to 5V and GND on their other two pins)

# Overview
PCMedi is a testament to the power and flexibility of the Arduino platform. By intelligently dividing labor between a controller board (Uno) and a dedicated HID board (Pro Micro), it achieves a level of functionality that is both powerful and easily expandable. It provides a tactile and responsive way to manage day-to-day PC tasks, automate logins, mix application audio, and gain an edge in gaming, all through a single, custom-built device.
