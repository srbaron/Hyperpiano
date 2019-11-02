// LIBRARIES
#include <FastLED.h>

// MIDI PATH
const int MIDI_Channel = 10;

// POTENTIOMETERS
const int numPOT = 4; 
const int POTS[4] = {A0, A1, A2, A3};
int potValue[numPOT] = {0};
int previousPOTValue[numPOT] = {0};
int val = 0;

//FSR'S
const int numFSR = 3;
const int FSRS[3] = {A4, A5, A6};
int fsrValue[numFSR] = {0};
int previousFSRValue[numFSR] = {0};

//NEOPIXELS
const int MIDI_Receive_Channel = 11;  
const int NeoPin = 1;                 
const int NumberOfNeos = 2;     
const int saturation = 255;    
int hue = 150; 
int brightness[NumberOfNeos] = {0};
CRGB leds[NumberOfNeos];


void setup() {
  analogReadResolution(7);
  FastLED.addLeds<NEOPIXEL, NeoPin>(leds, NumberOfNeos);
  usbMIDI.setHandleControlChange(OnControlChange);
}

void loop() {
  // POTENTIOMETERS
  for(int i=0; i<numPOT; i++) {
    potValue[i] = analogRead(POTS[i]);    
    
    if(potValue[i] != previousPOTValue[i]) {
      usbMIDI.sendControlChange(i, potValue[i], MIDI_Channel);
      previousPOTValue[i] = potValue[i];
    }
  }

  // FSR'S
  for(int i=0; i<numFSR; i++) {
    fsrValue[i] = analogRead(FSRS[i]);

    if(fsrValue[i] != previousFSRValue[i] && fsrValue[i] >= 30) {
      usbMIDI.sendControlChange(i+numPOT, fsrValue[i], MIDI_Channel);
      previousFSRValue[i] = fsrValue[i]; 
    }
    else if(fsrValue[i] < 30) {
      usbMIDI.sendControlChange(i+numPOT, 0, MIDI_Channel);
    }
  } 

  //NEOPIXELS 
  usbMIDI.read(MIDI_Receive_Channel);

  for(int i=0;i<NumberOfNeos;i++) {
    
    leds[i] = CHSV(random(0,255), saturation, brightness[i]);
  }
  FastLED.show();
}

void OnControlChange(byte channel, byte control, byte value) {
 if(control == 0) {
    hue = value << 1;
  }
  if(control == 1) {
    brightness[0] = value << 1;
  }
  if(control == 2) {
    brightness[1] = value << 1;
  }
} 
 
