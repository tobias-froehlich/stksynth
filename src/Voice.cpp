#include <math.h>
#include "Stk.h"
#include "Voice.h"
#include "ADSR.h"
#include "const.h"
#include "const.h"

Voice::Voice() {
  value = 0.0;
  for(int i=0; i<8; i++) {
    phases[i] = 0.0;
  }
  adsr = new stk::ADSR();
  adsr->setAttackTime(0.25);
  adsr->setDecayTime(1.0);
  adsr->setSustainLevel(0.5);
  adsr->setReleaseTime(1.0);
}

Voice::~Voice() {
  delete adsr;
}

void Voice::setMidicode(int midicode) {
  this->frequency = cFrequenciesEqual[midicode] * std::pow(cTwelfthRootOfTwo, bending) ;
}

void Voice::setBending(stk::StkFloat bending) {
  this->bending = bending;
}

void Voice::noteOn() {
  adsr->keyOn();
}

void Voice::noteOff() {
  adsr->keyOff();
}

stk::StkFloat Voice::tick() {
   stk::StkFloat value = 0.0;
   for(int i=0; i<8; i++) {
     phases[i] += cTwoPi / 44100.0 * frequency * overtones[i];
     if (phases[i] >= cTwoPi) {
       phases[i] -= cTwoPi;
     }
     value += sin(phases[i]) * amplitudes[i];
   }
   return value * adsr->tick();
}
