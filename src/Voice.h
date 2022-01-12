#ifndef VOICE_H_
#define VOICE_H_

#include <ADSR.h>
#include "Config.h"

class Voice {
  private:
    stk::StkFloat frequency = 440.0;
    stk::StkFloat bending = 0.0;
    unsigned int nOvertones;
    std::vector<stk::StkFloat> phases;
    std::vector<stk::StkFloat> amplitudes;
    std::vector<stk::StkFloat> overtones;
    stk::StkFloat amplitude = 1.0;
    stk::StkFloat value;
    std::vector<stk::ADSR*> adsrs;
  public:
    Voice(Config* config);
    ~Voice();
    void setMidicode(int midicode);
    void setBending(stk::StkFloat bending);
    void noteOn();
    void noteOff();
    stk::StkFloat tick();
};

#endif
