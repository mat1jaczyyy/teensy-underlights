/*
 *     USB MIDI to WS2812B by mat1jaczyyy
 *     ----------------------------------
 */

/*  
 * LED Strip definition
 * --------------------
 */

const byte _nLED = 60;  // Number of LEDs
const byte _dLED = 4;   // Data pin

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel _LED = Adafruit_NeoPixel(_nLED, _dLED, NEO_GRB + NEO_KHZ800);

/*
 * Color Palette
 * Generate with retinaConverter.py (Retina 2.0+ Palette)
 * ------------------------------------------------------
 */

const byte _r[128] = {0, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 45, 93, 142, 223, 190, 28, 61, 93, 190, 125, 12, 28, 45, 158, 61, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 12, 28, 45, 158, 61, 28, 61, 93, 190, 125, 45, 93, 142, 223, 190, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 36, 73, 109, 146, 182, 219, 255};
const byte _g[128] = {0, 0, 0, 0, 125, 0, 12, 28, 45, 158, 61, 28, 61, 93, 190, 125, 45, 93, 142, 223, 190, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 45, 93, 142, 223, 190, 28, 61, 93, 190, 125, 12, 28, 45, 158, 61, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 36, 73, 109, 146, 182, 219, 255};
const byte _b[128] = {0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 12, 28, 45, 158, 61, 28, 61, 93, 190, 125, 45, 93, 142, 223, 190, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 45, 93, 142, 223, 190, 28, 61, 93, 190, 125, 12, 28, 45, 158, 61, 36, 73, 109, 146, 182, 219, 255};

/*
 * MIDI handler
 * ------------
 */

const byte _pStart = 36;  // First note in array
bool update = false;

void noteOn(byte ch, byte p, byte v) {
  _LED.setPixelColor(p - _pStart, _r[v], _g[v], _b[v]);
  update = true;
}

void noteOff(byte ch, byte p, byte v) {
  _LED.setPixelColor(p - _pStart, 0, 0, 0);
  update = true;
}

void sysEx(byte *data, unsigned int length) {
  if (length == 6) {
    _LED.setPixelColor(*(data+1) - _pStart, *(data+2), *(data+3), *(data+4));
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
