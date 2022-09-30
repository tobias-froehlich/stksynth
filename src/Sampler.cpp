#include <FileWvIn.h>
#include <math.h>
#include "Sampler.h"
#include "const.h"

Sampler::Sampler() {
  std::cout << "create sampler.\n";
}

Sampler::~Sampler() {
  std::cout << "destroy sampler.\n";
}

void Sampler::readSampleFile(std::string sampleFileName, stk::StkFloat referenceFrequency) {
  sampleData.clear();
  referenceFrequencies.clear();

  stk::FileWvIn sampleFile;
  sampleFile.openFile(sampleFileName);
  fileRate = sampleFile.getFileRate();
  std::vector < stk::StkFloat > baseSampleData;
  for(unsigned long i=0; i<sampleFile.getSize(); i++) {
    baseSampleData.push_back(sampleFile.tick());
  }
  sampleFile.closeFile();
  sampleData.push_back(baseSampleData);
  referenceFrequencies.push_back(referenceFrequency);
  for(int i=1; i<cNumberOfOctavesForSampler; i++) {
    unsigned long length = sampleData.at(i-1).size();
    std::vector < stk::StkFloat > newSample;
    for(unsigned long t=1; t<length; t+=2) {
      newSample.push_back((sampleData.at(i-1)[t-1] + sampleData.at(i-1)[t]) * 0.5);
    }
    sampleData.push_back(newSample);
    referenceFrequencies.push_back(referenceFrequencies.at(i-1) * 2.0);
  }
  for(unsigned int i = 0; i < referenceFrequencies.size(); i++) {
    std::cout << "length: " << sampleData[i].size() << "\n";
    std::cout << "freq: " << referenceFrequencies[i] << "\n";
  }
}

stk::StkFloat Sampler::getFileRate() {
  return fileRate;
}

int Sampler::getSampleIndex(stk::StkFloat frequency) {
  std::cout << "find " << frequency << " in ";
  for(stk::StkFloat frequency : referenceFrequencies) {
    std::cout << frequency << " ";
  }
  std::cout << "\n";
  if (frequency < referenceFrequencies[1]) {
    return 0;
  }
  if (frequency >= referenceFrequencies[referenceFrequencies.size() - 1]) {
    return referenceFrequencies.size() - 1;
  }
  for(unsigned int i = 2; i < referenceFrequencies.size(); i++) {
    if (frequency < referenceFrequencies[i]) {
      return i - 1;
    }
  }
  return 0;
}

stk::StkFloat Sampler::getReferenceFrequency(stk::StkFloat frequency) {
  return referenceFrequencies[getSampleIndex(frequency)];
}

stk::StkFloat Sampler::getValue(int sampleIndex, stk::StkFloat position) {
   unsigned long lower = std::floor(position);
   if (lower >= sampleData[sampleIndex].size() - 1) {
     return 0.0;
   }
   stk::StkFloat rest = position - (stk::StkFloat)lower;
   return (1.0 - rest) * sampleData[sampleIndex][lower] + rest * sampleData[sampleIndex][lower + 1];
}
