Arduino Joystick and RFID Control
This project uses an Arduino board to control two continuous rotation servos with a joystick, and a status LED, all activated by an RFID tag.

Components
Arduino Uno

RFID-RC522 Module

Joystick Module

Two MG995R Continuous Rotation Servos

One LED

One 220-ohm Resistor

Jumper Wires

External Power Source (e.g., a battery pack)

Wiring Diagram
1. Power and Ground
Connect the Arduino's 5V and GND pins to the power rails on your breadboard. This provides a common ground for the entire circuit.

2. RFID-RC522 Module
The RFID module communicates with the Arduino using the SPI protocol. Connect it as follows:

VCC pin to Arduino 3.3V

RST pin to Arduino Digital Pin 9

GND pin to the ground rail

MISO pin to Arduino Digital Pin 12

MOSI pin to Arduino Digital Pin 11

SCK pin to Arduino Digital Pin 13

SDA pin to Arduino Digital Pin 10

3. Joystick Module
Connect the joystick to the Arduino for input control:

VCC to the 5V power rail

GND to the ground rail

VRx to Arduino Analog Pin A0

VRy to Arduino Analog Pin A1

4. Continuous Rotation Servos
Important: For reliable operation, the MG995R servos must be powered by an external power source. Connect the grounds of the external power source and the Arduino to create a common ground.

Motor 1 (controlled by X-axis):

Signal wire to Arduino Digital Pin 5

VCC wire to the positive (+) rail of your external power supply

GND wire to the negative (-) rail of your external power supply

Motor 2 (controlled by Y-axis):

Signal wire to Arduino Digital Pin 6

VCC wire to the positive (+) rail of your external power supply

GND wire to the negative (-) rail of your external power supply

5. Status LED
Connect the long leg (anode) of the LED to one end of the 220-ohm resistor.

Connect the other end of the resistor to Arduino Digital Pin 2.

Connect the short leg (cathode) of the LED to the ground rail.

This wiring diagram will ensure all components are properly connected and powered for the project to function as intended.
