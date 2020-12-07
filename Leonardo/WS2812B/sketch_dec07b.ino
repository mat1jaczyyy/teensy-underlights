/*
 *     USB MIDI to WS2812B by mat1jaczyyy & 4dvn
 *     ---------------------------------------
 */

/*  
 * LED Strip definition
 * --------------------
 */

const byte _NLED = 1;
const byte _DPIN = 2;

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel _LED = Adafruit_NeoPixel(_NLED, _DPIN, NEO_GRB + NEO_KHZ800);

/*
 * Color Palette
 * Generate with retinaConverter.py (Retina 2.0+ Palette)
 * ------------------------------------------------------
 */

const byte _R[128] = {0, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 45, 93, 142, 223, 190, 28, 61, 93, 190, 125, 12, 28, 45, 158, 61, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 12, 28, 45, 158, 61, 28, 61, 93, 190, 125, 45, 93, 142, 223, 190, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 36, 73, 109, 146, 182, 219, 255};
const byte _G[128] = {0, 0, 0, 0, 125, 0, 12, 28, 45, 158, 61, 28, 61, 93, 190, 125, 45, 93, 142, 223, 190, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 45, 93, 142, 223, 190, 28, 61, 93, 190, 125, 12, 28, 45, 158, 61, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 36, 73, 109, 146, 182, 219, 255};
const byte _B[128] = {0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 0, 0, 0, 125, 0, 12, 28, 45, 158, 61, 28, 61, 93, 190, 125, 45, 93, 142, 223, 190, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 61, 125, 190, 255, 255, 45, 93, 142, 223, 190, 28, 61, 93, 190, 125, 12, 28, 45, 158, 61, 36, 73, 109, 146, 182, 219, 255};

/*
 * MIDI handler
 * ------------
 */

#include "MIDIUSB.h"
#include "pitchToNote.h"
#define NUM_BUTTONS  1

const uint16_t button1 = 3;

const uint16_t buttons[NUM_BUTTONS] = {button1};
const byte notePitches[NUM_BUTTONS] = {pitchC2};

uint16_t notesTime[NUM_BUTTONS];
uint16_t pressedButtons = 0x00;
uint16_t previousButtons = 0x00;
uint16_t intensity;

const byte _PStart = 36;  // First note in array
bool update = false;

void note(byte pitch, byte velocity) {
  _LED.setPixelColor(pitch - _PStart, _R[velocity], _G[velocity], _B[velocity]);
  update = true;
}

void setup() {
  _LED.begin();
  _LED.show();

  for (int i = 0; i < NUM_BUTTONS; i++)
    pinMode(buttons[i], INPUT_PULLUP);
}

void loop() {

  midiEventPacket_t rx;
  do { 
    rx = MidiUSB.read();
    if (rx.header == 0x9) note(rx.byte2, rx.byte3);
    if (rx.header == 0x8) note(rx.byte2, 0);
  } while (rx.header == 0);

  if (update) {
    _LED.show();
    update = false;
  }
  readButtons();
  playNotes();
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void readButtons()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    if (digitalRead(buttons[i]) == LOW)
    {
      bitWrite(pressedButtons, i, 1);
      delay(2);
    }
    else
      bitWrite(pressedButtons, i, 0);
  }
}

void playNotes()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    if (bitRead(pressedButtons, i) != bitRead(previousButtons, i))
    {
      if (bitRead(pressedButtons, i))
      {
        bitWrite(previousButtons, i , 1);
        noteOn(0, notePitches[i], 100);
        MidiUSB.flush();
      }
      else
      {
        bitWrite(previousButtons, i , 0);
        noteOff(0, notePitches[i], 0);
        MidiUSB.flush();
      }
    }
  }
}

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}


//void sysEx(byte *data, unsigned int length) {
//  if (length == 6) {
//    _LED.setPixelColor(*(data+1) - _PStart, *(data+2), *(data+3), *(data+4));
//    update = true;
//  }
//}
