#include "Stk.h"
#include "Synth.h"

Synth::Synth() {
  for(int i=0; i<16; i++) {
    voices[i] = new Voice();
    voices[i]->setMidicode(57 + i);
  }
}

Synth::~Synth() {
  for(int i=0; i<16; i++) {
    delete voices[i];
  }
}

void Synth::setMidicode(int channel, int midicode) {
  voices[channel]->setMidicode(midicode);
}

void Synth::setBending(int channel, stk::StkFloat bending) {
  voices[channel]->setBending(bending);
}

void Synth::noteOn(int channel) {
  voices[channel]->noteOn();
}

void Synth::noteOff(int channel) {
  voices[channel]->noteOff();
}

stk::StkFloat Synth::tick() {
  stk::StkFloat value = 0.0;
  for(int i=0; i<16; i++) {
    value += (voices[i])->tick();
  }
  value /= 16.0;
  return value;
}

stk::StkFrames& Synth::tick(stk::StkFrames& frames, unsigned int channel) {
    return frames;
}
