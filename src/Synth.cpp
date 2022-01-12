#include "Stk.h"
#include "Synth.h"

Synth::Synth(Config* config) {

  if (!config->name_occurs("midi-channels")) {
    throw std::invalid_argument("No midi-channels specified.");
  }
  channels = config->get_ints("midi-channels");
  std::cout << "Listening on the following midi channels: ";
  for (int channel : channels) {
    std::cout << channel << " ";
  }
  std::cout << "\n";

  nVoices = channels.size();

  for(int i=0; i<nVoices; i++) {
    voices[i] = new Voice();
  }
}

Synth::~Synth() {
  for(int i=0; i<nVoices; i++) {
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
  for(int i = 0; i < 16; i++) {
    value += voices[i]->tick();
  }
  value /= 16.0;
  return value;
}

stk::StkFrames& Synth::tick(stk::StkFrames& frames, unsigned int channel) {
    return frames;
}
