midi-ribbon-controller
======================

A midi instrument made with an Arduino microcontroller.

## Building your own Midi Ribbon Controller

This project aims to be simple, cheap and easy to recreate at home.

### Hardware Required

0. [A "funduino" microcontroller](http://www.dx.com/p/diy-funduino-uno-r3-development-board-microcontroller-w-usb-cable-240588) (a cheap but fully functional Arduino clone) - $15.19
1. [A 500mm SoftPot Membrane Potentiometer](https://www.sparkfun.com/products/8681) - $25.95
2. [A Force Sensitive Resistor](https://www.sparkfun.com/products/9376) - $7.95
3. [A MIDI Connector](https://www.sparkfun.com/products/9536) - $1.95
4. [3 LEDs - Any colour you like](https://www.sparkfun.com/products/9650) - $1.05 *
5. 3 220Ω resistors + A 10 kΩ resistors - Almost nothing, let's say $1 *
6. [A Rotary Potentiometer - 10k Ohm, Linear](https://www.sparkfun.com/products/9939) - $0.95 *
7. [Enough Wire](https://www.sparkfun.com/products/8023) - $2.50 *
8. [A breadboard](http://www.dx.com/p/830-point-solderless-breadboard-118354) can be nice - $5.40 *

For a total price of approximately **$61.94 (USD)** (plus taxes, plus shipping fees).
Note that items with a * can be bought at any electronic store for a really cheap price.

#### Stuff you probably have if you already use MIDI instruments :
0. A midi cable (or MIDI to USB, if you want to plug it on your computer) - Since this is a MIDI instrument, it doesn't produce any sound by itself.
1. A MIDI synthesizer. It can be a software synthesizer (such as [ZynAddSubFX](https://en.wikipedia.org/wiki/ZynAddSubFX) under GNU/Linux, OS X and Windows), or a hardware syntetiser (such as [Llammas](https://github.com/adelrune/Llammas), the Arduino MIDI synthesizer we developped along with this project).

### Assembling

Do this:

![MIDI Ribbon Controller Circuit Diagram](/circuit-diagram.png "Circuit Diagram")

### Compiling

You will need to add to your Arduino IDE (Sketch > Import Library) the [Arduino MIDI library](https://github.com/FortySevenEffects/arduino_midi_library/releases/latest). After that, simply compile and transfer the program on your Arduino board.

## Usage

Plug the MIDI In side of your MIDI cable in the controller and the MIDI Out in your synthesizer to be ready to play !
Put a finger on the force sensor and slide another on the SoftPot Membrane Potentiometer to hear magical sounds.

### Controls
- **The force sensor** determines the velocity of the notes.
- **The SoftPot Membrane Potentiometer** determines the current note.
- **The Rotary Potentiometer** determines the current octave (0 to 7).
- **The LEDs** indicate the current octave (in binary, you'll get used to it).

## Troubleshooting

If you have any problem, you can contact us at 316k [ät] legtux [döt] org and we'll be happy to help you.

## Licence

This instrument has been conceived by Nicolas Hurtubise, Guillaume Riou & Aude Forcione-Lambert and is distributed under the terms of the MIT License. See the LICENSE file for more details.

## Contributing

Contributions are always appreciated :) Just be sure to include all the required documentation to reproduce what you've done (this may include a circuit diagram).
Keep in mind that the instrument should stay fully MIDI compatible.
