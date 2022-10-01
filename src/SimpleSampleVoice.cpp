#include <math.h>
#include <Stk.h>
#include <ADSR.h>
#include "Voice.h"
#include "SimpleSampleVoice.h"
#include "const.h"
#include "utils.h"

SimpleSampleVoice::SimpleSampleVoice(Config* config, Sampler* sampler) : Voice(config) {
  this->sampler = sampler;
  sampleIndex = 0;
  stepSize = 1.0;
}

SimpleSampleVoice::~SimpleSampleVoice() {
  std::cout << "destroy voice.\n";
}

void SimpleSampleVoice::specificSetMidicode(int midicode) {
  sampleIndex = sampler->getSampleIndex(frequency);
  std::cout << "sampleIndex: " << sampleIndex << "\n";
  stk::StkFloat referenceFrequency = sampler->getReferenceFrequency(frequency);
  stepSize = sampler->getFileRate() / sampleRate * frequency / referenceFrequency;
}

void SimpleSampleVoice::noteOn() {
  position = 0.0;
  adsr->keyOn();
}

void SimpleSampleVoice::noteOff() {
  adsr->keyOff();
}

stk::StkFloat SimpleSampleVoice::specificTick() {
  position += stepSize;
  return sampler->getValue(sampleIndex, position) * adsr->tick() * amplitude * velocityAmplitude;
}

