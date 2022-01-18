#ifndef VOICE_H_
#define VOICE_H_

#include <ADSR.h>
#include <Noise.h>
#include <BiQuad.h>
#include "Config.h"

class Voice {
  protected:
    stk::StkFloat frequency = 440.0;
    std::vector<stk::StkFloat> frequenciesEqual;
    stk::StkFloat bending = 0.0;
    unsigned int nOvertones;
    std::vector<stk::StkFloat> amplitudes;
    std::vector<stk::StkFloat> keyAmplitudes;
    std::vector<stk::StkFloat> overtones;
    stk::StkFloat amplitude = 1.0;
    stk::StkFloat value;
    std::vector<stk::ADSR*> adsrs;
    void calculateFrequenciesEqual(Config* config);
    void interpolateKeyAmplitudes(std::vector<int> keyAmplitudesX,
        std::vector<stk::StkFloat> keyAmplitudesY);
  public:
    Voice(Config* config);
    virtual ~Voice();
    virtual void setMidicode(int midicode);
    virtual void setBending(stk::StkFloat bending);
    virtual void noteOn();
    virtual void noteOff();
    virtual stk::StkFloat tick();
};

#endif
