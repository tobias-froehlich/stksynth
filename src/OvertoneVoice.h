#ifndef OVERTONE_VOICE_H_
#define OVERTONE_VOICE_H_

#include <ADSR.h>
#include <Noise.h>
#include <BiQuad.h>
#include "Config.h"
#include "Voice.h"

class OvertoneVoice : public Voice {
  protected:
    unsigned int nOvertones;
    std::vector<stk::StkFloat> amplitudes;
    std::vector<stk::StkFloat> overtones;
    std::vector<stk::ADSR*> adsrs;
  public:
    OvertoneVoice(Config* config);
    virtual ~OvertoneVoice();
    virtual void setBending(stk::StkFloat bending);
    virtual void noteOn();
    virtual void noteOff();
    virtual stk::StkFloat tick();
};

#endif
