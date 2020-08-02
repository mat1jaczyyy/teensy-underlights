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

const byte _r[128] = {0, 65, 130, 255, 255, 255, 130, 65, 255, 255, 130, 65, 255, 255, 130, 65, 134, 81, 40, 20, 73, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 45, 0, 0, 0, 105, 45, 24, 12, 255, 255, 130, 65, 255, 255, 130, 65, 255, 150, 117, 32, 0, 0, 0, 0, 0, 16, 125, 28, 255, 186, 174, 97, 12, 0, 0, 0, 24, 89, 174, 40, 255, 134, 113, 0, 57, 85, 53, 89, 49, 105, 210, 255, 255, 182, 142, 130, 57, 0, 12, 20, 20, 101, 130, 219, 215, 255, 158, 101, 20, 219, 125, 154, 142, 61, 113, 223, 158, 53, 24, 4, 182, 61, 178, 73};
const byte _g[128] = {0, 65, 130, 255, 61, 0, 0, 0, 186, 61, 32, 16, 174, 255, 130, 65, 255, 255, 130, 65, 255, 255, 130, 65, 255, 255, 130, 65, 255, 255, 130, 65, 255, 255, 130, 65, 194, 166, 85, 45, 134, 85, 45, 24, 36, 0, 0, 0, 53, 0, 0, 0, 61, 0, 0, 0, 65, 0, 0, 0, 12, 53, 81, 53, 57, 73, 20, 0, 69, 0, 125, 28, 0, 255, 235, 255, 138, 255, 166, 40, 0, 0, 24, 16, 49, 223, 255, 255, 255, 255, 255, 138, 81, 81, 28, 0, 69, 166, 255, 89, 40, 73, 77, 20, 28, 57, 0, 65, 73, 190, 223, 178, 20, 210, 235, 150, 101, 61, 113, 255, 0, 0, 206, 65, 174, 49, 81, 20};
const byte _b[128] = {0, 65, 130, 255, 61, 0, 0, 0, 105, 0, 0, 0, 45, 0, 0, 0, 49, 0, 0, 0, 73, 0, 0, 0, 93, 24, 12, 4, 89, 85, 45, 24, 182, 150, 73, 36, 255, 255, 130, 65, 255, 255, 130, 65, 255, 255, 130, 65, 251, 255, 130, 65, 255, 255, 130, 65, 109, 81, 40, 20, 0, 0, 0, 4, 0, 24, 109, 255, 77, 202, 125, 28, 0, 45, 4, 8, 0, 93, 255, 255, 255, 255, 121, 0, 0, 4, 20, 0, 36, 109, 202, 255, 194, 231, 255, 89, 0, 0, 0, 4, 0, 12, 32, 40, 89, 24, 0, 40, 16, 36, 45, 12, 45, 105, 138, 255, 255, 61, 113, 255, 0, 0, 0, 0, 0, 0, 0, 0};

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
