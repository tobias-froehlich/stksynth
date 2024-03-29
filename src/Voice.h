#ifndef VOICE_H_
#define VOICE_H_

#include <ADSR.h>
#include <Noise.h>
#include <BiQuad.h>
#include "LowpassFilter.h"
#include "Config.h"

class Voice {
  protected:
    stk::StkFloat sampleRate;
    stk::StkFloat frequency = 440.0;
    std::vector<stk::StkFloat> frequenciesEqual;
    stk::StkFloat maxBending = 0.0;
    stk::StkFloat bending = 0.0;
    std::vector<stk::StkFloat> amplitudes;
    std::vector<stk::StkFloat> keyAmplitudes;
    stk::StkFloat amplitude = 1.0;
    stk::ADSR* adsr;
    std::vector< LowpassFilter* > lowpassFilters;
    int useVelocity;
    stk::StkFloat velocityExponent;
    stk::StkFloat velocityAmplitude;
    stk::StkFloat value;
    void calculateFrequenciesEqual(Config* config);
    void interpolateKeyAmplitudes(std::vector<int> keyAmplitudesX,
        std::vector<stk::StkFloat> keyAmplitudesY);
  public:
    Voice(Config* config);
    virtual ~Voice();
    void setMidicode(int midicode);
    virtual void specificSetMidicode(int midicode);
    void setBending(stk::StkFloat bending);
    void setVelocity(int velocity);
    virtual void noteOn();
    virtual void noteOff();
    stk::StkFloat tick();
    virtual stk::StkFloat specificTick();
};

#endif
