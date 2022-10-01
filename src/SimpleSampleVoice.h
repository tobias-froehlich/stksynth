#ifndef SIMPLE_SAMPLE_VOICE_H_
#define SIMPLE_SAMPLE_VOICE_H_

#include <ADSR.h>
#include "Voice.h"
#include "Config.h"
#include "Sampler.h"

class SimpleSampleVoice : public Voice {
  private:
    Sampler* sampler;
    int sampleIndex;
    stk::StkFloat position;
    stk::StkFloat stepSize;
  public:
    SimpleSampleVoice(Config* config, Sampler* sampler);
    virtual ~SimpleSampleVoice();
    void specificSetMidicode(int midicode);
    void setBending(stk::StkFloat bending);
    void noteOn();
    void noteOff();
    stk::StkFloat specificTick();
};

#endif
