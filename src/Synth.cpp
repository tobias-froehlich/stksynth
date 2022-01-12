#include "Stk.h"
#include "Synth.h"

Synth::Synth(Config* config) {

  if (!config->name_occurs("midi-channels")) {
    throw std::invalid_argument("No midi-channels specified.");
  }
  channels = config->get_ints("midi-channels");
  for(int &channel : channels) channel--;
  nVoices = channels.size();
  for(int j=0; j<nVoices; j++) {
    if ((channels[j] < 0) || (channels[j] > 15)) {
      throw std::invalid_argument("Midi channel not between 1 and 16.");
    }
    for(int i=0; i<nVoices; i++) {
      if ((i != j) && (channels[i] == channels[j])) {
        throw std::invalid_argument("The same midi channel occurs twice.");
      }
    }
  }
  std::cout << "Listening on the following midi channels: ";
  for (int channel : channels) {
    std::cout << channel + 1 << " ";
  }
  std::cout << "\n";


  for(int i=0; i<nVoices; i++) {
    voices.push_back(new Voice());
  }
}

Synth::~Synth() {
  for(int i=0; i<nVoices; i++) {
    delete voices[i];
  }
}

void Synth::setMidicode(int channel, int midicode) {
  int index = getIndexFromChannel(channel);
  if (index >= 0) {
    voices[index]->setMidicode(midicode);
  }
}

void Synth::setBending(int channel, stk::StkFloat bending) {
  int index = getIndexFromChannel(channel);
  if (index >= 0) {
    voices[channel]->setBending(bending);
  }
}

void Synth::noteOn(int channel) {
  int index = getIndexFromChannel(channel);
  if (index >= 0) {
    voices[channel]->noteOn();
  }
}

void Synth::noteOff(int channel) {
  int index = getIndexFromChannel(channel);
  if (index >= 0) {
    voices[channel]->noteOff();
  }
}

stk::StkFloat Synth::tick() {
  stk::StkFloat value = 0.0;
  for(Voice* voice : voices) {
    value += voice->tick();
  }
  value /= 16.0;
  return value;
}

stk::StkFrames& Synth::tick(stk::StkFrames& frames, unsigned int channel) {
    return frames;
}

int Synth::getIndexFromChannel(int channel) {
  for(int i=0; i<nVoices; i++) {
    if (channels[i] == channel) {
      return i;
    }
  }
  return -1;
}
