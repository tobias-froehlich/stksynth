#include <math.h>
#include <Stk.h>
#include <ADSR.h>
#include "Voice.h"
#include "OvertoneVoice.h"
#include "const.h"

OvertoneVoice::OvertoneVoice(Config* config) : Voice(config) {
  value = 0.0;
  
  if (!config->name_occurs("factor-two")) {
    throw std::invalid_argument("Parameter factor-two not defined.");
  }
  two = config->get_float("factor-two");
  if (!config->name_occurs("factor-three")) {
    throw std::invalid_argument("Parameter factor-three not defined.");
  }
  three = config->get_float("factor-three");
  if (!config->name_occurs("factor-five")) {
    throw std::invalid_argument("Parameter factor-five not defined.");
  }
  five = config->get_float("factor-five");
  if(!config->name_occurs("factor-seven")) {
    throw std::invalid_argument("Parameter factor-seven not defined.");
  }
  seven = config->get_float("factor-seven");

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


  std::cout << "two = " << two << "\n";
  std::cout << "three = " << three << "\n";
  std::cout << "five = " << five << "\n";

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
        factor = seven;
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
        factor = two * seven;
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
        factor = three * seven;
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
        factor = two * two * seven;
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

  std::cout << "overtones: ";
  for (stk::StkFloat overtone : overtones) {
    std::cout << overtone << " ";
  }

  if (!config->name_occurs("overtone-falloff-power")) {
    throw std::invalid_argument("Parameter overtone-falloff-power not defined");
  }
  stk::StkFloat overtoneFalloffPower = config->get_float("overtone-falloff-power");
  if (!config->name_occurs("overtone-amplitudes")) {
    throw std::invalid_argument("Parameter overtone-amplitudes not defined.");
  }
  amplitudes = config->get_floats("overtone-amplitudes");
  for(unsigned int i=0; i<nOvertones; i++) {
    amplitudes[i] /= pow((stk::StkFloat)(i + 1), overtoneFalloffPower);
  }
  if (overtones.size() != amplitudes.size()) {
    throw std::invalid_argument("Parameter overtone-amplitudes has a wrong length.");
  }
  stk::StkFloat sumOfAmplitudes = 0.0;
  for(stk::StkFloat amplitude : amplitudes) {
    sumOfAmplitudes += amplitude;
  }
  for(stk::StkFloat &amplitude : amplitudes) amplitude /= sumOfAmplitudes;
  for(stk::StkFloat &amplitude : amplitudes) std::cout << amplitude << " ";
  
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
    adsrs.push_back(new stk::ADSR());
    adsrs.back()->setAttackTime(overtoneAttack[i]);
    adsrs.back()->setDecayTime(overtoneDecay[i]);
    adsrs.back()->setSustainLevel(overtoneSustain[i]);
    adsrs.back()->setReleaseTime(overtoneRelease[i]);
  }
}

OvertoneVoice::~OvertoneVoice() {
  for(stk::ADSR* adsr : adsrs) {
    delete adsr;
  }
}

//void OvertoneVoice::setMidicode(int midicode) {}

void OvertoneVoice::setBending(stk::StkFloat bending) {}

void OvertoneVoice::noteOn() {}

void OvertoneVoice::noteOff() {}

stk::StkFloat OvertoneVoice::specificTick() {return 0.0;}


