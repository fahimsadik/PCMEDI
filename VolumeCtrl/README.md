🎧 Simple Pro Micro Volume Controller
This project creates a minimalist, plug-and-play volume knob for your PC using an Arduino Pro Micro and a standard rotary encoder. It acts as a native USB media controller, requiring no special drivers to work.

# Features

Volume Control 🔊: Rotate the knob to increase or decrease the system volume.


Mute Function 🔇: Press the knob's built-in button to mute or unmute your computer's audio.


Plug and Play 🔌: Thanks to the HID-Project library, your computer will instantly recognize it as a media controller.

# How It Works
The code continuously monitors the state of the three pins connected to the rotary encoder.

By detecting the sequence of pulses from the 

CLK (ROTARY_A) and DT (ROTARY_B) pins, it determines the direction of rotation and sends the appropriate MEDIA_VOLUME_UP or MEDIA_VOLUME_DOWN command.

A press of the switch pin (

ROTARY_C) sends the MEDIA_VOLUME_MUTE command.

# Hardware & Connections
Arduino Pro Micro

Rotary Encoder (EC11 type with a built-in switch)

# Connect the components as follows:

Encoder Pin	Function	Pro Micro Pin
CLK (or A)	Clock	Pin 14
DT (or B)	Data	Pin 16
SW	Switch	Pin 10
GND	Ground	GND
+	Power	VCC

The pin numbers are defined in the code as ROTARY_A, ROTARY_B, and ROTARY_C.
