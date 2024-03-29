#ifndef ADDITIVE_VOICE_H_
#define ADDITIVE_VOICE_H_

#include <ADSR.h>
#include "OvertoneVoice.h"
#include "Config.h"

class AdditiveVoice : public OvertoneVoice {
  private:
    std::vector<stk::StkFloat> phases;
  public:
    AdditiveVoice(Config* config);
    virtual ~AdditiveVoice();
    void specificSetMidicode(int midicode);
    void setBending(stk::StkFloat bending);
    void noteOn();
    void noteOff();
    stk::StkFloat specificTick();
};

#endif
