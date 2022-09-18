#include <math.h>
#include <Stk.h>
#include <ADSR.h>
#include "Voice.h"
#include "SampleVoice.h"
#include "const.h"
#include "utils.h"

SampleVoice::SampleVoice(Config* config) : Voice(config) {
//  if (!config->name_occurs("sample-file-name")) {
//    throw std::invalid_argument("Parameter sample-file-name not defined");
//  }
//  this->sampleFileName = config->get_string("sample-file-name");
//
//  if (!config->name_occurs("sample-frequency")) {
//    throw std::invalid_argument("Parameter sample-frequency not defined");
//  }
//  this->sampleFrequency = config->get_float("sample-frequency");
//
//  this->sampleFileData = new stk::FileWvIn();
//  this->sampleFileData->openFile(this->sampleFileName);
  std::vector < std::string > sampleFileNames;
  for(int key = 0; key < 128; key++) {
    std::string parameterName = "sample_key_";
    parameterName.append(utils::intToStringLeadingZeros(key, 3));
    if (!config->name_occurs(parameterName)) {
      std::string errorMessage = "Parameter ";
      errorMessage.append(parameterName);
      errorMessage.append(" not defined.");
      throw std::invalid_argument(errorMessage);
    }
    std::vector< std::string > parameters = config->get_strings(parameterName);
    if (parameters.size() != 2) {
      std::string errorMessage = "The parameter ";
      errorMessage.append(parameterName);
      errorMessage.append(" needs exactly two arguments: The sample file name and the reference frequency.");
      throw std::invalid_argument(errorMessage);
    }
    std::string sampleFileName = parameters[0];
    if (!utils::stringIsFloat(parameters[1])) {
      std::string errorMessage = "The second argument of parameter ";
      errorMessage.append(parameterName);
      errorMessage.append(" must be a float, i.e. must consists of digits and exactly one point.");
      throw std::invalid_argument(errorMessage);
    }
    float referenceFrequency = std::stof(parameters[1]);
    std::cout << parameterName << " " << parameters[0] << " " << parameters[1] << "\n";
    std::cout << referenceFrequency << "\n";
    int sampleFileNameAlreadyExists = 0;
    int sampleIndex = 0;
    for (unsigned int i = 0; i < sampleFileNames.size(); i++) {
      if (sampleFileNames[i].compare(sampleFileName) == 0) {
        sampleIndex = i;
        sampleFileNameAlreadyExists = 1;
      }
    }
    if (!sampleFileNameAlreadyExists) {
      sampleIndex = sampleFileNames.size();
      sampleFileNames.push_back(sampleFileName);
    }
    sampleIndexForKey.push_back(sampleIndex);
    sampleFrequencies.push_back(referenceFrequency);
  }

  for (std::string sampleFileName : sampleFileNames) {
    stk::FileWvIn* sampleFile = new stk::FileWvIn;
    sampleFile->openFile(sampleFileName);
    sampleFile->setInterpolate(true);
    sampleFiles.push_back(sampleFile);
  }
  currentSampleFile = sampleFiles[0];
  std::cout << "Number of different sample files: " << sampleFiles.size() << "\n";
}

SampleVoice::~SampleVoice() {
  for(stk::FileWvIn* sampleFile : sampleFiles) {
    sampleFile->closeFile();
    delete sampleFile;
  }
}

void SampleVoice::setMidicode(int midicode) {
  this->frequency = frequenciesEqual[midicode] * std::pow(cTwelfthRootOfTwo, bending);
  int sampleIndex = sampleIndexForKey[midicode];
  currentSampleFile = sampleFiles[sampleIndex];
  currentSampleFile->setRate(currentSampleFile->getFileRate() / 44100.0 * frequency / sampleFrequencies[midicode]);
  std::cout << "frequency: " << frequency << ", sample frequency: " << sampleFrequencies[sampleIndex] << "\n";
  amplitude = keyAmplitudes[midicode];
}

void SampleVoice::setBending(stk::StkFloat bending) {
  this->bending = bending;
}

void SampleVoice::noteOn() {
  currentSampleFile->reset();
  adsr->keyOn();
}

void SampleVoice::noteOff() {
  adsr->keyOff();
}

stk::StkFloat SampleVoice::tick() {
    return currentSampleFile->tick() * adsr->tick() * amplitude * velocityAmplitude;
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

