#include <math.h>
#include <Stk.h>
#include <ADSR.h>
#include "Voice.h"
#include "AdditiveVoice.h"
#include "const.h"

AdditiveVoice::AdditiveVoice(Config* config) : Voice(config) {
  for(unsigned int i=0; i<nOvertones; i++) {
    phases.push_back(0.0);
  }
  
}

AdditiveVoice::~AdditiveVoice() {
  for(stk::ADSR* adsr : adsrs) {
    delete adsr;
  }
}

void AdditiveVoice::setMidicode(int midicode) {
  this->frequency = frequenciesEqual[midicode] * std::pow(cTwelfthRootOfTwo, bending);
  amplitude = keyAmplitudes[midicode];
}

void AdditiveVoice::setBending(stk::StkFloat bending) {
  this->bending = bending;
}

void AdditiveVoice::noteOn() {
  for(stk::ADSR* adsr : adsrs) {
    adsr->keyOn();
  }
}

void AdditiveVoice::noteOff() {
  for(stk::ADSR* adsr : adsrs) {
    adsr->keyOff();
  }
}

stk::StkFloat AdditiveVoice::tick() {
   stk::StkFloat value = 0.0;
   for(unsigned int i=0; i<nOvertones; i++) {
     phases[i] += cTwoPi / 44100.0 * frequency * overtones[i];
     if (phases[i] >= cTwoPi) {
       phases[i] -= cTwoPi;
     }
     value += sin(phases[i]) * adsrs[i]->tick() * amplitudes[i];
   }
   return value * amplitude;
}

