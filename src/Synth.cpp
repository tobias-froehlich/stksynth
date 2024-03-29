#include <Stk.h>
#include <ctime>
#include "Synth.h"
#include "Voice.h"
#include "AdditiveVoice.h"
#include "NoiseVoice.h"
#include "SampleVoice.h"
#include "SimpleSampleVoice.h"
#include "ReverberatorFreeVerb.h"
#include "ReverberatorJCRev.h"
#include "ReverberatorNRev.h"
#include "ReverberatorPRCRev.h"

Synth::Synth(Config* config) {
  isLoading = 1;
  filter = new stk::BiQuad();
  init(config);
}  

void Synth::init(Config* config) {
  if (!config->name_occurs("midi-channels")) {
    throw std::invalid_argument("No midi-channels specified.");
  }
  channels = config->get_ints("midi-channels");
  for(int &channel : channels) channel--;
  nVoices = channels.size();
  for(unsigned int j=0; j<nVoices; j++) {
    if ((channels[j] < 0) || (channels[j] > 15)) {
      throw std::invalid_argument("Midi channel not between 1 and 16.");
    }
    for(unsigned int i=0; i<nVoices; i++) {
      if ((i != j) && (channels[i] == channels[j])) {
        throw std::invalid_argument("The same midi channel occurs twice.");
      }
    }
  }
  std::cout << "Listening on the following midi channels: ";
  for (int channel : channels) {
    std::cout << channel + 1 << " ";
  }
  std::cout << "\n";

  if (!config->name_occurs("bending")) {
    throw std::invalid_argument("No bending specified.");
  }
  maxBending = config->get_float("bending");
  std::cout << "Bending: +/-" << maxBending << " seminotes\n";

  if (!config->name_occurs("overall-amplitude")) {
    throw std::invalid_argument("Parameter overall-amplitude not defined");
  }
  overallAmplitude = config->get_float("overall-amplitude");
  std::cout << "Overall amplitude is " << overallAmplitude << ".\n";

  if (!config->name_occurs("output-file-name")) {
    throw std::invalid_argument("Parameter output-file-name not defined.");
  }
  outputFileName = config->get_string("output-file-name");
  std::cout << "Recording will be written to file starting with \"" << outputFileName << "\".\n";


  if (!config->name_occurs("filter-resonance-mix")) {
    throw std::invalid_argument("Parameter filter-resonance-mix not defined.");
  }
  filterResonanceMix = config->get_float("filter-resonance-mix");
  if ((filterResonanceMix < 0.0) || (filterResonanceMix > 1.0)) {
    throw std::invalid_argument("Parameter filter-resonance-mix is smaller than 0.0 or greater than 1.0 .");
  }
  if (!config->name_occurs("filter-resonance-frequency")) {
    throw std::invalid_argument("Parameter filter-resonance-frequency not defined.");
  }
  if (!config->name_occurs("filter-resonance-radius")) {
    throw std::invalid_argument("Parameter filter-resonance-radius not defined.");
  }
  filter->setResonance(
      config->get_float("filter-resonance-frequency"),
      config->get_float("filter-resonance-radius"),
      true
  );
  if (!config->name_occurs("filter-notch-frequency")) {
    throw std::invalid_argument("Parameter filter-notch-frequency not defined.");
  }
  if (!config->name_occurs("filter-notch-radius")) {
    throw std::invalid_argument("Parameter filter-notch-radius not defined.");
  }
  filter->setNotch(
      config->get_float("filter-notch-frequency"),
      config->get_float("filter-notch-radius")
  );


  if (!config->name_occurs("chorus")) {
    throw std::invalid_argument("Paramater chorus not defined.");
  }
  if (config->get_string("chorus").compare("OFF") == 0) {
    useChorus = 0;
  } else if (config->get_string("chorus").compare("ON") == 0) {
    useChorus = 1;
    if (!config->name_occurs("chorus-delay")) {
      throw std::invalid_argument("Parameter chorus-delay not defined.");
    }
    chorus = new stk::Chorus(config->get_float("chorus-delay"));
    if (!config->name_occurs("chorus-modulation-depth")) {
      throw std::invalid_argument("Parameter chorus-modulation-depth not defined.");
    }
    chorus->setModDepth(config->get_float("chorus-modulation-depth"));
    if (!config->name_occurs("chorus-modulation-frequency")) {
      throw std::invalid_argument("Parameter chorus-modulation-frequency not defined.");
    }
    chorus->setModFrequency(config->get_float("chorus-modulation-frequency"));
  } else {
    throw std::invalid_argument("Paramater chorus must be ON or OFF.");
  }


  
  std::cout << "nVoices = " << nVoices << "\n";

  if (!config->name_occurs("voice-type")) {
    throw std::invalid_argument("Parameter voice_type not defined.");
  }
  std::string voiceType = config->get_string("voice-type");
  if (voiceType.compare("ADDITIVE") == 0) {
    for(unsigned int i=0; i<nVoices; i++) {
      voices.push_back(new AdditiveVoice(config));
    }
  } else if (voiceType.compare("NOISE") == 0) {
    for(unsigned int i=0; i<nVoices; i++) {
      voices.push_back(new NoiseVoice(config));
    }
  } else if (voiceType.compare("SAMPLE") == 0) {
    for (unsigned int i=0; i<nVoices; i++) {
      voices.push_back(new SampleVoice(config));
    }
  } else if (voiceType.compare("SIMPLESAMPLE") == 0) {
    if (!config->name_occurs("sample-file")) {
      throw std::invalid_argument("Parameter sample-file not defined.");
    }
    std::string sampleFileName = config->get_string("sample-file");
    if (!config->name_occurs("sample-reference-frequency")) {
      throw std::invalid_argument("Parameter sample-reference-frequency not defined.");
    }
    stk::StkFloat sampleReferenceFrequency = config->get_float("sample-reference-frequency");
    sampler.readSampleFile(sampleFileName, sampleReferenceFrequency);
    for (unsigned int i=0; i<nVoices; i++) {
      voices.push_back(new SimpleSampleVoice(config, &sampler));
    }
  } else {
    throw std::invalid_argument("Parameter voice-type has an unknown value.");
  }

  if (!config->name_occurs("reverb-algorithm")) {
    throw std::invalid_argument("Parameter reverb-algorithm not defined.");
  }
  if (config->get_string("reverb-algorithm").compare("OFF") == 0) {
     useReverberator = 0;
  }
  else if (config->get_string("reverb-algorithm").compare("FREE_VERB") == 0) {
    useReverberator = 1;
    reverberator = new ReverberatorFreeVerb(config);
  }
  else if (config->get_string("reverb-algorithm").compare("JC_REV") == 0) {
    useReverberator = 1;
    reverberator = new ReverberatorJCRev(config);
  }
  else if (config->get_string("reverb-algorithm").compare("N_REV") == 0) {
    useReverberator = 1;
    reverberator = new ReverberatorNRev(config);
  }
  else if (config->get_string("reverb-algorithm").compare("PRC_REV") == 0) {
    useReverberator = 1;
    reverberator = new ReverberatorPRCRev(config);
  }
  else {
    throw std::invalid_argument("Reverb algorithm is not supported.");
  }
  isLoading = 0;

}

Synth::~Synth() {
  isLoading = 1;
  destroy();
  delete filter;
  if (useReverberator) {
    delete reverberator;
  }
  isLoading = 0;
}

void Synth::destroy() {
  while (voices.size() > 0) {
    delete voices.back();
    voices.pop_back();
  }
  nVoices = 0;
  delete chorus;
  chorus = 0;
}

void Synth::reload(Config* config) {
  isLoading = 1;
  destroy();
  init(config);
  isLoading = 0;
}

void Synth::setMidicode(int channel, int midicode) {
  int index = getIndexFromChannel(channel);
  if (index >= 0) {
    voices[index]->setMidicode(midicode);
  }
}

void Synth::setVelocity(int channel, int velocity) {
  int index = getIndexFromChannel(channel);
  if (index >= 0) {
    voices[index]->setVelocity(velocity);
  }
}

void Synth::setBending(int channel, stk::StkFloat bending) {
  int index = getIndexFromChannel(channel);
  if (index >= 0) {
    voices[index]->setBending(bending * maxBending);
  }
}

void Synth::noteOn(int channel) {
  int index = getIndexFromChannel(channel);
  std::cout << "channel (on): " << channel << " on  index " << index << "\n";
  if (index >= 0) {
    voices[index]->noteOn();
  }
}

void Synth::noteOff(int channel) {
  std::cout << "channel (off): " << channel << "\n";
  int index = getIndexFromChannel(channel);
  if (index >= 0) {
    voices[index]->noteOff();
  }
}

void Synth::startRecording() {
  time_t now;
  std::time(&now);
  char buf[sizeof "yyyy-mm-ddThh:mm:ssZ"];
  strftime(buf, sizeof buf, "%FT%TZ", gmtime(&now));
  std::string timeString = std::string(buf);
  std::string fullFileName = outputFileName + timeString + ".wav";
  std::cout << fullFileName << "\n";

  outputFile = new stk::FileWvOut();
  outputFile->openFile(fullFileName, 2, stk::FileWrite::FILE_WAV, stk::Stk::STK_SINT16);
  recording = 1;
  std::cout << "Recording started. Data written to " << fullFileName << " .\n";
}

void Synth::stopRecording() {
  if (recording) {
    recording = 0;
    outputFile->closeFile();
    delete outputFile;
    std::cout << "Recording stopped.\n";
  }
}

int Synth::isRecording() {
  return recording;
}

stk::StkFloat Synth::tick() {
  stk::StkFloat value = 0.0;
  for(Voice* voice : voices) {
    value += voice->tick();
  }
  value *= overallAmplitude / (stk::StkFloat)nVoices;
  return value;
}

void Synth::tick(stk::StkFloat* samples, unsigned int nChannels, unsigned int nBufferSize) {
  for(unsigned int i=0; i<nBufferSize; i++) {
    stk::StkFloat valueLeft = 0.0;
    stk::StkFloat valueRight = 0.0;
    if (!isLoading) {
      stk::StkFloat value = tick();
      value = (1.0 - filterResonanceMix) * value + filterResonanceMix * filter->tick(value);
      if (useChorus) {
        valueLeft = chorus->tick(value, 0);
        valueRight = chorus->lastOut(1);
      } else {
        valueLeft = value;
        valueRight = value;
      }

      if (useReverberator) {
        valueLeft = reverberator->tick(valueLeft, valueRight, 0);
        valueRight = reverberator->lastOut(1);
      }
    }

    *samples++ = valueLeft;
    *samples++ = valueRight;;

    if (recording) {
      stk::StkFrames* frames = new stk::StkFrames(1, 2);
      (*frames)[0] = valueLeft;
      (*frames)[1] = valueRight;
      outputFile->tick(*frames);
    }      
  }
}

unsigned int Synth::getIndexFromChannel(int channel) {
  for(unsigned int i=0; i<nVoices; i++) {
    if (channels[i] == channel) {
      return i;
    }
  }
  return -1;
}
