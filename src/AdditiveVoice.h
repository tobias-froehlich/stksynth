#ifndef ADDITIVE_VOICE_H_
#define ADDITIVE_VOICE_H_

#include <ADSR.h>
#include "Voice.h"
#include "Config.h"

class AdditiveVoice : public Voice {
  private:
    std::vector<stk::StkFloat> phases;
  public:
    AdditiveVoice(Config* config);
    virtual ~AdditiveVoice();
    void setMidicode(int midicode);
    void setBending(stk::StkFloat bending);
    void noteOn();
    void noteOff();
    stk::StkFloat tick();
};

#endif
