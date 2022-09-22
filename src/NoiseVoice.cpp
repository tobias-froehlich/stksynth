#include <math.h>
#include <Stk.h>
#include <ADSR.h>
#include "NoiseVoice.h"
#include "const.h"

NoiseVoice::NoiseVoice(Config* config) : OvertoneVoice(config) {
  if (!config->name_occurs("frequency-sharpness")) {
    throw std::invalid_argument("Parameter frequency-sharpness not defined.");
  }
  frequencySharpness = config->get_float("frequency-sharpness");

  if (!config->name_occurs("n-filters")) {
    throw std::invalid_argument("Parameter n-filters not defined.");
  }
  nFilters = config->get_int("n-filters");

  for(unsigned int i=0; i<nOvertones; i++) {
    filters.push_back({});
    for(unsigned int j=0; j<nFilters; j++) {
        filters.back().push_back(new stk::BiQuad());
    }
    noiseGenerators.push_back(new stk::Noise(std::rand()));
    sounding.push_back(0);
  }
  factor = {0, 0, 0};
}

NoiseVoice::~NoiseVoice() {
  for(std::vector<stk::BiQuad*> vector : filters) {
    for(stk::BiQuad* filter : vector) {
      delete filter;    
    }
  }
  for(stk::Noise* noiseGenerator : noiseGenerators) {
    delete noiseGenerator;
  }
}

void NoiseVoice::setMidicode(int midicode) {
  this->frequency = frequenciesEqual[midicode] * std::pow(cTwelfthRootOfTwo, bending);
  for(unsigned int i=0; i<nOvertones; i++) {
    for(unsigned int j=0; j<nFilters; j++) {
      std::cout << overtones[i] << " ";
      filters[i][j]->setResonance(frequency * overtones[i], 1.0 - pow(10.0, -frequencySharpness), false);
    }
  }
  amplitude = keyAmplitudes[midicode];// * pow(10.0, -frequencySharpness*0.3/nFilters);

  factor[0] = pow(5, -frequencySharpness*pow(440.0/frequency, 0.15));
  factor[1] = pow(10, -frequencySharpness*pow(440.0/frequency, 0.15));
  factor[2] = pow(40, -frequencySharpness*pow(440.0/frequency, 0.10));
}

void NoiseVoice::setBending(stk::StkFloat bending) {
  this->bending = bending;
}

void NoiseVoice::noteOn() {
  for(unsigned int i=0; i<nOvertones; i++) {
    adsrs[i]->keyOn();
    sounding[i] = 1;
  }
  adsr->keyOn();
  maxBeforeFilter = 0.0;
  maxAfterFilter = 0.0;
}

void NoiseVoice::noteOff() {
  for(unsigned int i=0; i<nOvertones; i++) {
    adsrs[i]->keyOff();
  }
  adsr->keyOff();
  std::cout << maxBeforeFilter << " " << maxAfterFilter << "\n";
}

stk::StkFloat NoiseVoice::specificTick() {

   stk::StkFloat value = 0.0;
   for(unsigned int i=0; i<nOvertones; i++) {
      stk::StkFloat noiseValue = 0.0;
      if (sounding[i]) {
        if (adsrs[i]->getState() != stk::ADSR::IDLE) {
          noiseValue = noiseGenerators[i]->tick() * amplitudes[i];
        } else {
          sounding[i] = 0;
          for(unsigned int j=0; j<nFilters; j++) {
            filters[i][j]->setResonance(0.0, 0.0);
          }
        }
      }
      stk::StkFloat adsrValue = adsrs[i]->tick();
      if (i == 0) {
        if (noiseValue > maxBeforeFilter) {
          maxBeforeFilter = noiseValue;
        }
      }
      for(unsigned int j=0; j<nFilters; j++) {
          noiseValue = filters[i][j]->tick(noiseValue) * factor[j];
      }
      if (i == 0) {
        if (noiseValue > maxAfterFilter) {
           maxAfterFilter = noiseValue;
        }
      }
//      value += filters[i][0]->tick(noiseValue * adsrValue * amplitudes[i] * amplitude) * adsrValue ;
      value += noiseValue * adsrValue * amplitude * velocityAmplitude;
   }
   return value;
//   stk::StkFloat value = 0.0;
//   stk::StkFloat overallAdsrValue = adsr->tick();
//    for(unsigned int i=0; i<nOvertones; i++) {
//      stk::StkFloat noiseValue = 0.0;
//      if (sounding[i]) {
//        if (adsrs[i]->getState() != stk::ADSR::IDLE) {
//          noiseValue = noiseGenerators[i]->tick() * amplitudes[i];
//        } else {
//          sounding[i] = 0;
//          for(unsigned int j=0; j<nFilters; j++) {
//            filters[i][j]->setResonance(0.0, 0.0);
//          }
//        }
//      }
//      stk::StkFloat adsrValue = adsrs[i]->tick() * overallAdsrValue;
//      if (i == 0) {
//        if (noiseValue > maxBeforeFilter) {
//          maxBeforeFilter = noiseValue;
//        }
//      }
//      for(unsigned int j=0; j<nFilters; j++) {
//          noiseValue = filters[i][j]->tick(noiseValue) * factor[j];
//      }
//      if (i == 0) {
//        if (noiseValue > maxAfterFilter) {
//           maxAfterFilter = noiseValue;
//        }
//      }
////      value += filters[i][0]->tick(noiseValue * adsrValue * amplitudes[i] * amplitude) * adsrValue ;
//      value += noiseValue * adsrValue * amplitude;
//   }
//   return value;
}

