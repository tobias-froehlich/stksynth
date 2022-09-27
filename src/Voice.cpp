#include <math.h>
#include <Stk.h>
#include <ADSR.h>
#include "LowpassFilter.h"
#include "Voice.h"
#include "const.h"

Voice::Voice(Config* config) {
  value = 0.0;


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

  
  if (!config->name_occurs("lowpass-frequencies")) {
    throw std::invalid_argument("Parameter lowpass-frequencies not defined.");
  }  
  std::vector< stk::StkFloat > filterFrequencies = config->get_floats("lowpass-frequencies");
  for(stk::StkFloat filterFrequency : filterFrequencies) {
    lowpassFilters.push_back(new LowpassFilter(filterFrequency));
  }


  if (!config->name_occurs("use-velocity")) {
    throw std::invalid_argument("Parameter use-velocity not defined.");
  }
  std::string useVelocityString = config->get_string("use-velocity");
  if (useVelocityString.compare("true") == 0) {
    useVelocity = 1;
  } else if (useVelocityString.compare("false") == 0) {
    useVelocity = 0;
  } else {
    throw std::invalid_argument("Parameter use-velocity must be true or false.");
  }
  if (useVelocity) {
    if (!config->name_occurs("velocity-exponent")) {
      throw std::invalid_argument("Parameter velocity-exponent not defined.");
    }
    velocityExponent = config->get_float("velocity-exponent");
    if (velocityExponent <= 0.0) {
      throw std::invalid_argument("Parameter velocity-exponent must be greater than zero.");
    }
  }

  

  if (!config->name_occurs("key-amplitudes-x")) {
    throw std::invalid_argument("Parameter key-amplitudes-x not defined.");
  }
  if (!config->name_occurs("key-amplitudes-y")) {
    throw std::invalid_argument("Parameter key-amplitudes-y not defined.");
  }
  std::vector<int> keyAmplitudesX = config->get_ints("key-amplitudes-x");
  std::vector<stk::StkFloat> keyAmplitudesY = config->get_floats("key-amplitudes-y");
  if (keyAmplitudesX.size() != keyAmplitudesY.size()) {
    throw std::invalid_argument("Parameters key-amplitudes-x and key-amplitudes-y do not have the same length.");
  }
  for(unsigned int j=0; j<keyAmplitudesX.size(); j++) {
    if ((keyAmplitudesX[j] < 0) && (keyAmplitudesX[j] >= 128)) {
      throw std::invalid_argument("Parameter key-amplitudes-x has a value that is not in the range between 0 and 127.");
    }
    for(unsigned int i=0; i<keyAmplitudesX.size(); i++) {
      if ((i != j) && (keyAmplitudesX[i] == keyAmplitudesX[j])) {
        throw std::invalid_argument("Parameter key-amplitudes-x has a value twice.");
      }
    }
  }
  interpolateKeyAmplitudes(keyAmplitudesX, keyAmplitudesY);
  
  calculateFrequenciesEqual(config);


  adsr = new stk::ADSR();
  adsr->setAttackTime(attack);
  adsr->setDecayTime(decay);
  adsr->setSustainLevel(sustain);
  adsr->setReleaseTime(release);

}

Voice::~Voice() {
  delete adsr;
  for(LowpassFilter* lowpassFilter : lowpassFilters) {
    delete lowpassFilter;
  }
}

void Voice::setMidicode(int midicode) {}

void Voice::setBending(stk::StkFloat bending) {}

void Voice::setVelocity(int velocity) {
  if (useVelocity) {
    velocityAmplitude = pow((stk::StkFloat)velocity / (stk::StkFloat)255.0, velocityExponent);
  } else {
    velocityAmplitude = 1.0;
  }
}

void Voice::noteOn() {}

void Voice::noteOff() {}

stk::StkFloat Voice::tick() {
  stk::StkFloat value = specificTick();
  for(LowpassFilter* lowpassFilter : lowpassFilters) {
    value = lowpassFilter->tick(value);
  }
  return value;
}

stk::StkFloat Voice::specificTick() {
  return 0.0;
}


void Voice::calculateFrequenciesEqual(Config* config) {
  if (!config->name_occurs("reference-midi-key")) {
    throw std::invalid_argument("Parameter reference-midi-key not defined.");
  }
  int referenceMidiKey = config->get_int("reference-midi-key");
  if (!config->name_occurs("reference-frequency")) {
    throw std::invalid_argument("Parameter reference-frequency not defined.");
  }
  stk::StkFloat referenceFrequency = config->get_float("reference-frequency");
  for(int i=0; i<128; i++) {
    frequenciesEqual.push_back(
        referenceFrequency
        * pow(cTwelfthRootOfTwo, i - referenceMidiKey)
    );
  }
}

void Voice::interpolateKeyAmplitudes(std::vector<int> keyAmplitudesX,
    std::vector<stk::StkFloat> keyAmplitudesY) {
  for(int key=0; key<128; key++) {
    int minLarger = 128;
    int maxSmaller = -1;
    for(int foundKey : keyAmplitudesX) {
      if ((foundKey <= key) && (foundKey >= maxSmaller)) {
        maxSmaller = foundKey;
      }
      if ((foundKey >= key) && (foundKey <= minLarger)) {
        minLarger = foundKey;
      }
    }
    if ((minLarger == 128) && (maxSmaller == -1)) {
      keyAmplitudes.push_back(1.0);
    } else if (minLarger == 128) {
      stk::StkFloat valueAtMaxSmaller;
      for(unsigned int j=0; j<keyAmplitudesX.size(); j++) {
        int foundKey = keyAmplitudesX[j];
       if (foundKey == maxSmaller) {
          valueAtMaxSmaller = keyAmplitudesY[j];
        }
      }   
      keyAmplitudes.push_back(valueAtMaxSmaller);
    } else if ((maxSmaller == -1) || (minLarger == maxSmaller)) {
      stk::StkFloat valueAtMinLarger;
      for(unsigned int j=0; j<keyAmplitudesX.size(); j++) {
        int foundKey = keyAmplitudesX[j];
        if (foundKey == minLarger) {
          valueAtMinLarger = keyAmplitudesY[j];
        }
      }
      keyAmplitudes.push_back(valueAtMinLarger);
    } else {
      stk::StkFloat valueAtMinLarger = 0.0;
      stk::StkFloat valueAtMaxSmaller = 0.0;
      for(unsigned int j=0; j<keyAmplitudesX.size(); j++) {
        int foundKey = keyAmplitudesX[j];
        if (foundKey == minLarger) {
          valueAtMinLarger = keyAmplitudesY[j];
        }
        if (foundKey == maxSmaller) {
          valueAtMaxSmaller = keyAmplitudesY[j];
        }
      }
      keyAmplitudes.push_back(valueAtMaxSmaller + (valueAtMinLarger - valueAtMaxSmaller) * (stk::StkFloat)((int)key - maxSmaller) / (stk::StkFloat)(minLarger - maxSmaller));
    }
  }

}
