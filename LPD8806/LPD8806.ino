/*
 *     USB MIDI to LPD8806 by mat1jaczyyy
 *     ----------------------------------
 */

/*  
 * LED Strip definition
 * --------------------
 */

const byte _nLED = 42;  // Number of LEDs
const byte _dLED = 4;   // Data pin
const byte _cLED = 3;   // Clock pin

#include <LPD8806.h>
LPD8806 _LED = LPD8806(_nLED, _dLED, _cLED);

/*
 * Color Palette
 * Generate with retinaConverter.py (Retina 2.0+ Palette)
 * ------------------------------------------------------
 */

const byte _r[128] = {0, 32, 65, 127, 127, 127, 65, 32, 127, 127, 65, 32, 127, 127, 65, 32, 67, 40, 20, 10, 36, 0, 0, 0, 36, 0, 0, 0, 36, 0, 0, 0, 36, 0, 0, 0, 36, 0, 0, 0, 36, 0, 0, 0, 22, 0, 0, 0, 52, 22, 12, 6, 127, 127, 65, 32, 127, 127, 65, 32, 127, 75, 58, 16, 0, 0, 0, 0, 0, 8, 62, 14, 127, 93, 87, 48, 6, 0, 0, 0, 12, 44, 87, 20, 127, 67, 56, 0, 28, 42, 26, 44, 24, 52, 105, 127, 127, 91, 71, 65, 28, 0, 6, 10, 10, 50, 65, 109, 107, 127, 79, 50, 10, 109, 62, 77, 71, 30, 56, 111, 79, 26, 12, 2, 91, 30, 89, 36};
const byte _g[128] = {0, 32, 65, 127, 30, 0, 0, 0, 93, 30, 16, 8, 87, 127, 65, 32, 127, 127, 65, 32, 127, 127, 65, 32, 127, 127, 65, 32, 127, 127, 65, 32, 127, 127, 65, 32, 97, 83, 42, 22, 67, 42, 22, 12, 18, 0, 0, 0, 26, 0, 0, 0, 30, 0, 0, 0, 32, 0, 0, 0, 6, 26, 40, 26, 28, 36, 10, 0, 34, 0, 62, 14, 0, 127, 117, 127, 69, 127, 83, 20, 0, 0, 12, 8, 24, 111, 127, 127, 127, 127, 127, 69, 40, 40, 14, 0, 34, 83, 127, 44, 20, 36, 38, 10, 14, 28, 0, 32, 36, 95, 111, 89, 10, 105, 117, 75, 50, 30, 56, 127, 0, 0, 103, 32, 87, 24, 40, 10};
const byte _b[128] = {0, 32, 65, 127, 30, 0, 0, 0, 52, 0, 0, 0, 22, 0, 0, 0, 24, 0, 0, 0, 36, 0, 0, 0, 46, 12, 6, 2, 44, 42, 22, 12, 91, 75, 36, 18, 127, 127, 65, 32, 127, 127, 65, 32, 127, 127, 65, 32, 125, 127, 65, 32, 127, 127, 65, 32, 54, 40, 20, 10, 0, 0, 0, 2, 0, 12, 54, 127, 38, 101, 62, 14, 0, 22, 2, 4, 0, 46, 127, 127, 127, 127, 60, 0, 0, 2, 10, 0, 18, 54, 101, 127, 97, 115, 127, 44, 0, 0, 0, 2, 0, 6, 16, 20, 44, 12, 0, 20, 8, 18, 22, 6, 22, 52, 69, 127, 127, 30, 56, 127, 0, 0, 0, 0, 0, 0, 0, 0};

/*
 * MIDI handler
 * ------------
 */

const byte _pStart = 36;  // First note in array
bool update = false;

void noteOn(byte ch, byte p, byte v) {
  _LED.setPixelColor(p - _pStart, _r[v], _b[v], _g[v]);  // LPD8806 uses RBG format
  update = true;
}

void noteOff(byte ch, byte p, byte v) {
  _LED.setPixelColor(p - _pStart, 0, 0, 0);
  update = true;
}

void sysEx(byte *data, unsigned int length) {
  if (length == 6) {
    _LED.setPixelColor(*(data+1) - _pStart, *(data+2), *(data+4), *(data+3));
    update = true;
  }
}

/*
 * Initialize LED Strip and USB MIDI
 * ---------------------------------
 */

void setup() {
  _LED.begin();
  _LED.show();
  
  usbMIDI.setHandleNoteOn(noteOn);
  usbMIDI.setHandleNoteOff(noteOff);
  usbMIDI.setHandleSystemExclusive(sysEx);
}

/*
 * Read MIDI and update Strip
 * --------------------------
 */
void loop() {
  while (usbMIDI.read());
  if (update) {
    _LED.show();
    update = false;
  }
}
