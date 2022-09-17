#include <math.h>
#include <Stk.h>
#include <ADSR.h>
#include "Voice.h"
#include "SampleVoice.h"
#include "const.h"

SampleVoice::SampleVoice(Config* config) : Voice(config) {
  if (!config->name_occurs("sample-file-name")) {
    throw std::invalid_argument("Parameter sample-file-name not defined");
  }
  this->sampleFileName = config->get_string("sample-file-name");

  if (!config->name_occurs("sample-frequency")) {
    throw std::invalid_argument("Parameter sample-frequency not defined");
  }
  this->sampleFrequency = config->get_float("sample-frequency");

  this->sampleFileData = new stk::FileWvIn();
  this->sampleFileData->openFile(this->sampleFileName);

  this->sampleFileData->setInterpolate(true);

}

SampleVoice::~SampleVoice() {
  this->sampleFileData->closeFile();
  delete this->sampleFileData;
}

void SampleVoice::setMidicode(int midicode) {
  this->frequency = frequenciesEqual[midicode] * std::pow(cTwelfthRootOfTwo, bending);
  sampleFileData->setRate(sampleFileData->getFileRate() / 44100.0 * frequency / sampleFrequency);
  amplitude = keyAmplitudes[midicode];
}

void SampleVoice::setBending(stk::StkFloat bending) {
  this->bending = bending;
}

void SampleVoice::noteOn() {
  this->sampleFileData->reset();
  adsr->keyOn();
}

void SampleVoice::noteOff() {
  adsr->keyOff();
}

stk::StkFloat SampleVoice::tick() {
    return this->sampleFileData->tick() * adsr->tick() * amplitude * velocityAmplitude;
//   stk::StkFloat value = 0.0;
//   for(unsigned int i=0; i<nOvertones; i++) {
//     phases[i] += cTwoPi / 44100.0 * frequency * overtones[i];
//     if (phases[i] >= cTwoPi) {
//       phases[i] -= cTwoPi;
//     }
//     value += sin(phases[i]) * adsrs[i]->tick() * amplitudes[i];
//   }
//   return value * amplitude;
}

