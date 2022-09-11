#include <math.h>
#include <Stk.h>
#include <ADSR.h>
#include "AdditiveVoice.h"
#include "const.h"

AdditiveVoice::AdditiveVoice(Config* config) : OvertoneVoice(config) {
  for(unsigned int i=0; i<nOvertones; i++) {
    phases.push_back(0.0);
  }
  
}

AdditiveVoice::~AdditiveVoice() {
}

void AdditiveVoice::setMidicode(int midicode) {
  std::cout << "additive voice set midi code\n";
  this->frequency = frequenciesEqual[midicode] * std::pow(cTwelfthRootOfTwo, bending);
  std::cout << "frequency set\n";
  amplitude = keyAmplitudes[midicode];
  std::cout << "keyAmplitudes set\n";
}

void AdditiveVoice::setBending(stk::StkFloat bending) {
  this->bending = bending;
}

void AdditiveVoice::noteOn() {
  for(stk::ADSR* overtoneAdsr : adsrs) {
    overtoneAdsr->keyOn();
  }
  adsr->keyOn();
}

void AdditiveVoice::noteOff() {
  for(stk::ADSR* overtoneAdsr : adsrs) {
    overtoneAdsr->keyOff();
  }
  adsr->keyOn();
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
   return value * amplitude * adsr->tick() * velocityAmplitude;
}

