# Electromagnetic-Clock-Controller
An arduino based electromagnetic clock controller for custom wooden clocks.

## About
After searching online I could only find strange scematics for even more obscure circuits. This project is designed to make the electronics of making mechanical clocks easier.

## Usage
The program can be flashed to an arduino using the arduino IDE. The code is easily modified to accomodate different variations in clock design and function. The variables which control the timing and power of the coil can be found at the top of the program under ```Control Variables```.

The circuit diagram can be found below the components list.

1. Arduino (UNO)
2. Magnetic coil
3. L293D H-Bridge
4. DC power adapter (~12v) & Barrel jack
5. Potentiometers (x2) (optional)
6. Buttons (x2) (optional)
7. LED (optional)
8. 220 ohm resistor (x2) (optional)
9. 330 ohm resistor (optional)
10. Basic components & tools (wires/solder + iron/etc)

### Circuit
![alt text](https://github.com/mailtelenko/Electromagnetic-Clock-Controller/clock-circuit.png "Clock circuit")

## Setup
The potentiometers and buttons can be used to adjust the timing and power of the electomagnet. Although not neccessary for operation, using the potentiometer and buttons makes adjusting the clock easier compared to editing the code directly.

Once setup, the values can be set in the global variables section to ensure they stay constant during operation.

