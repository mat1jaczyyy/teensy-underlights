# LED Strip MIDI for Teensy, Atmega32u4 - controlling LED strips via MIDI

This Arduino project allows for controlling a LED strip from a DAW program (via MIDI) over a Teensy microcontroller.

## System Requirements

* LED Strip
	* The **LPD8806** and **WS2812B** strips are implemented in separate Arduino projects. 
* USB Development Board
    * A **32-bit PJRC Teensy** is highly recommended. A [Teensy-LC](https://www.pjrc.com/store/teensylc.html) was used while developing this project and is officially supported alongside other Teensy 3 models.\
    * Most Arduino atmega32u4 boards should work (such as the [Arduino Leonardo](https://store.arduino.cc/arduino-leonardo-with-headers), these have been tested and are unofficially supported (thanks [4dvn](https://github.com/4dvn)!).
    * Teensy 2 (8-bit) boards might work with modifications, but are not supported.
* [Arduino Software](https://www.arduino.cc/en/Main/Software)
    * Developed with **version 1.8.5**, but older versions will likely work.
    * Additionally requires [Teensyduino](https://www.pjrc.com/teensy/teensyduino.html) if you're using a Teensy 3 board, and the [LPD8806 Arduino library](https://github.com/adafruit/LPD8806) if you're using the LPD8806.
* OS
    * **Windows, Linux and macOS** are officially supported.
* DAW Software
    * [Ableton Live](https://www.ableton.com/en/live/) is recommended due to its flexibility in creating MIDI Effects.

## Hardware

Connect your board with the LED strip using the [schematic](https://github.com/mat1jaczyyy/teensy-underlights/blob/master/LPD8806.svg). You can attach the strip to any object, I used a short wooden cylinder which I wrapped the strip around and it works well.

## Software

If you haven't already, install Teensyduino for Arduino, and the LPD8806 libraries if applicable (the correct path for the `LPD8806.cpp` and `LPD8806.h` files from the adafruit repository is `Documents/Arduino/libraries/LPD8806/`) Modify the Arduino script according to your LED strip configuration (`_nLED` = Number of LEDs < 128, `_dLED` = Data pin, `_cLED` = Clock pin LPD8806 only). Under Tools, make sure `USB Type: "MIDI"`, `Optimize Code: Fastest` and your board are selected, then compile and upload to your board. You can also change your MIDI device name by editing `name.c`.

## Retina Palettes

The `retinaConverter.py` Python 3 script will take a Retina palette file as input, and output equivalent `_r`, `_g` and `_b` arrays to replace in the script. Before running the script, you should uncomment one of these [lines](https://github.com/mat1jaczyyy/teensy-underlights/blob/master/retinaConverter.py#L9-L10) depending on your LED strip. Then, run it with:

```$ python3 retinaConverter.py < paletteFile > array.txt```

## Usage

Sending standard MIDI note-offs and note-ons on any channel will trigger the LED with the matching color (from the palette arrays). Sending a SysEx message in the format `F0 PP RR GG BB F7` (where `PP` is the LED index, `RR` is the red component of the color, `GG` is the green component of the color, and `BB` is the blue component of the color) will directly set the LED to a desired color.
