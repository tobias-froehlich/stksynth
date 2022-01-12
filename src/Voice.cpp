#include <math.h>
#include <Stk.h>
#include <ADSR.h>
#include "Voice.h"
#include "const.h"

Voice::Voice(Config* config) {
  value = 0.0;

  if (!config->name_occurs("overtones")) {
    throw std::invalid_argument("Parameter overtones not defined.");
  }
  std::vector<int> intOvertones = config->get_ints("overtones");
  nOvertones = intOvertones.size();
  for(unsigned int j=0; j<nOvertones; j++) {
    if ((intOvertones[j] < 1) || (intOvertones[j] > 32)) {
      throw std::invalid_argument("Overtones is not in the range between 1 and 32.");
    }
    for(unsigned int i=0; i<nOvertones; i++) {
      if ((i != j) && (intOvertones[i] == intOvertones[j])) {
        throw std::invalid_argument("An overtone is defined twice.");
      }
    }
  }

  if (!config->name_occurs("factor-two")) {
    throw std::invalid_argument("Parameter factor-two not defined.");
  }
  stk::StkFloat two = config->get_float("factor-two");
  if (!config->name_occurs("factor-three")) {
    throw std::invalid_argument("Parameter factor-three not defined.");
  }
  stk::StkFloat three = config->get_float("factor-three");
  if (!config->name_occurs("factor-five")) {
    throw std::invalid_argument("Parameter factor-five not defined.");
  }
  stk::StkFloat five = config->get_float("factor-five");

  for(int intOvertone : intOvertones) {
    stk::StkFloat factor;
    switch(intOvertone) {
      case 1:
        factor = 1.0;
        break;
      case 2:
        factor = two;
        break;
      case 3:
        factor = three;
        break;
      case 4:
        factor = two * two;
        break;
      case 5:
        factor = five;
        break;
      case 6:
        factor = two * three;
        break;
      case 7:
        factor = 7.0;
        break;
      case 8:
        factor = two * two * two;
        break;
      case 9:
        factor = three * three;
        break;
      case 10:
        factor = two * five;
        break;
      case 11:
        factor = 11.0;
        break;
      case 12:
        factor = two * two * three;
        break;
      case 13:
        factor = 13.0;
        break;
      case 14:
        factor = two * 7.0;
        break;
      case 15:
        factor = three * five;
        break;
      case 16:
        factor = two * two * two * two;
        break;
      case 17:
        factor = 17.0;
        break;
      case 18:
        factor = two * three * three;
        break;
      case 19:
        factor = 19.0;
        break;
      case 20:
        factor = two * two * five;
        break;
      case 21:
        factor = three * 7.0;
        break;
      case 22:
        factor = two * 11.0;
        break;
      case 23:
        factor = 23.0;
        break;
      case 24:
        factor = two * two * two * three;
        break;
      case 25:
        factor = five * five;
        break;
      case 26:
        factor = two * 13.0;
        break;
      case 27:
        factor = three * three * three;
        break;
      case 28: 
        factor = two * two * 7.0;
        break;
      case 29:
        factor = 29.0;
        break;
      case 30:
        factor = two * three * five;
        break;
      case 31:
        factor = 31.0;
        break;
      case 32:
        factor = two * two * two * two * two;
        break;
      default:
        throw std::invalid_argument("Cannot find prime decomposition.");
    }
    overtones.push_back(factor);
  }

  if (!config->name_occurs("overtone-amplitudes")) {
    throw std::invalid_argument("Parameter overtone-amplitudes not defined.");
  }
  amplitudes = config->get_floats("overtone-amplitudes");
  if (overtones.size() != amplitudes.size()) {
    throw std::invalid_argument("Parameter overtone-amplitudes has a wrong length.");
  }
  stk::StkFloat sumOfAmplitudes = 0.0;
  for(stk::StkFloat amplitude : amplitudes) {
    sumOfAmplitudes += amplitude;
  }
  for(stk::StkFloat &amplitude : amplitudes) amplitude /= sumOfAmplitudes;

  if (!config->name_occurs("attack")) {
    throw std::invalid_argument("Parameter attack not defined.");
  }
  stk::StkFloat attack = config->get_float("attack");
  if (!config->name_occurs("decay")) {
    throw std::invalid_argument("Parameter decay not defined.");
  }
  stk::StkFloat decay = config->get_float("decay");
  if (!config->name_occurs("sustain")) {
    throw std::invalid_argument("Parameter sustain not defined.");
  }
  stk::StkFloat sustain = config->get_float("sustain");
  if (!config->name_occurs("release")) {
    throw std::invalid_argument("Parameter release not defined.");
  }
  stk::StkFloat release = config->get_float("release");

  
  if (!config->name_occurs("overtone-attack")) {
    throw std::invalid_argument("Parameter overtone-attack not defined.");
  }
  std::vector<stk::StkFloat> overtoneAttack = config->get_floats("overtone-attack");
  if (overtoneAttack.size() != nOvertones) {
    throw std::invalid_argument("Parameter overtone-attack has a wrong length.");
  }
  if (!config->name_occurs("overtone-decay")) {
    throw std::invalid_argument("Parameter overtone-decay not defined.");
  }
  std::vector<stk::StkFloat> overtoneDecay = config->get_floats("overtone-decay");
  if (overtoneDecay.size() != nOvertones) {
    throw std::invalid_argument("Parameter overtone-decay has a wrong length.");
  }
  if (!config->name_occurs("overtone-sustain")) {
    throw std::invalid_argument("Parameter overtone-sustain not defined.");
  }
  std::vector<stk::StkFloat> overtoneSustain = config->get_floats("overtone-sustain");
  if (overtoneSustain.size() != nOvertones) {
    throw std::invalid_argument("Parameter overtone-sustain has a wrong length.");
  }
  if (!config->name_occurs("overtone-release")) {
    throw std::invalid_argument("Parameter overtone-release not defined.");
  }
  std::vector<stk::StkFloat> overtoneRelease = config->get_floats("overtone-release");
  if (overtoneRelease.size() != nOvertones) {
    throw std::invalid_argument("Parameter overtone-release has a wrong length.");
  }


  for(unsigned int i=0; i<nOvertones; i++) {
    phases.push_back(0.0);
  }
  
  for(unsigned int i=0; i<nOvertones; i++) {
    adsrs.push_back(new stk::ADSR());
    adsrs.back()->setAttackTime(attack * overtoneAttack[i]);
    adsrs.back()->setDecayTime(decay * overtoneDecay[i]);
    adsrs.back()->setSustainLevel(sustain * overtoneSustain[i]);
    adsrs.back()->setReleaseTime(release * overtoneRelease[i]);
  }
}

Voice::~Voice() {
  for(stk::ADSR* adsr : adsrs) {
    delete adsr;
  }
}

void Voice::setMidicode(int midicode) {
  this->frequency = cFrequenciesEqual[midicode] * std::pow(cTwelfthRootOfTwo, bending);
  amplitude = (stk::StkFloat)(148 - midicode) / 148.0;
  amplitude = amplitude * amplitude;
}

void Voice::setBending(stk::StkFloat bending) {
  this->bending = bending;
}

void Voice::noteOn() {
  for(stk::ADSR* adsr : adsrs) {
    adsr->keyOn();
  }
}

void Voice::noteOff() {
  for(stk::ADSR* adsr : adsrs) {
    adsr->keyOff();
  }
}

stk::StkFloat Voice::tick() {
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
