#include <math.h>
#include <Stk.h>
#include <ADSR.h>
#include "NoiseVoice.h"
#include "const.h"

NoiseVoice::NoiseVoice(Config* config) : Voice(config) {
  if (!config->name_occurs("frequency-sharpness")) {
    throw std::invalid_argument("Parameter frequency-sharpness not defined.");
  }
  frequencySharpness = config->get_float("frequency-sharpness");

  for(unsigned int i=0; i<nOvertones; i++) {
    filters.push_back(new stk::BiQuad());
  }
  noise = new stk::Noise();
}

NoiseVoice::~NoiseVoice() {
  for(stk::BiQuad* filter : filters) {
    delete filter;
    
  }
  delete noise;
}

void NoiseVoice::setMidicode(int midicode) {
  this->frequency = frequenciesEqual[midicode] * std::pow(cTwelfthRootOfTwo, bending);
  for(unsigned int i=0; i<nOvertones; i++) {
    filters[i]->setResonance(frequency * overtones[i], 1.0 - pow(10.0, -frequencySharpness));
  }
  amplitude = keyAmplitudes[midicode] * pow(10.0, -frequencySharpness*0.5);
}

void NoiseVoice::setBending(stk::StkFloat bending) {
  this->bending = bending;
}

void NoiseVoice::noteOn() {
  for(stk::ADSR* adsr : adsrs) {
    adsr->keyOn();
  }
}

void NoiseVoice::noteOff() {
  for(stk::ADSR* adsr : adsrs) {
    adsr->keyOff();
  }
}

stk::StkFloat NoiseVoice::tick() {
   stk::StkFloat noiseValue = noise->tick();
   stk::StkFloat value = 0.0;
   for(unsigned int i=0; i<nOvertones; i++) {
//     phases[i] += cTwoPi / 44100.0 * frequency * overtones[i];
//     if (phases[i] >= cTwoPi) {
//       phases[i] -= cTwoPi;
//     }
//     value += sin(phases[i]) * adsrs[i]->tick() * amplitudes[i];
      value += filters[i]->tick(noiseValue) * adsrs[i]->tick() * amplitudes[i];
   }
   return value * amplitude;
}

