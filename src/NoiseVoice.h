#ifndef NOISE_VOICE_H_
#define NOISE_VOICE_H_

#include <ADSR.h>
#include <Noise.h>
#include <BiQuad.h>
#include "Voice.h"
#include "Config.h"

class NoiseVoice : public Voice {
  private:
    stk::StkFloat frequencySharpness;
    stk::Noise* noise;
    std::vector<stk::BiQuad*> filters;
  public:
    NoiseVoice(Config* config);
    virtual ~NoiseVoice();
    void setMidicode(int midicode);
    void setBending(stk::StkFloat bending);
    void noteOn();
    void noteOff();
    stk::StkFloat tick();
};

#endif
