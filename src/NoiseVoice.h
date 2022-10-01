#ifndef NOISE_VOICE_H_
#define NOISE_VOICE_H_

#include <ADSR.h>
#include <Noise.h>
#include <BiQuad.h>
#include "Voice.h"
#include "OvertoneVoice.h"
#include "Config.h"

class NoiseVoice : public OvertoneVoice {
  private:
    stk::StkFloat frequencySharpness;
    std::vector<stk::Noise*> noiseGenerators;
    std::vector< std::vector < stk::BiQuad* > > filters;
    unsigned int nFilters = 3;
    std::vector<int> sounding;
    stk::StkFloat maxBeforeFilter;
    stk::StkFloat maxAfterFilter;
    std::vector<stk::StkFloat> factor;
  public:
    NoiseVoice(Config* config);
    virtual ~NoiseVoice();
    void specificSetMidicode(int midicode);
    void setBending(stk::StkFloat bending);
    void noteOn();
    void noteOff();
    stk::StkFloat specificTick();
};

#endif
