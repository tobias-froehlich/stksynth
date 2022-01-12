#ifndef VOICE_H_
#define VOICE_H_

#include "ADSR.h"

class Voice {
  private:
    stk::StkFloat frequency = 440.0;
    stk::StkFloat bending = 0.0;
    stk::StkFloat phases[8];
    stk::StkFloat amplitudes[8] = {1.0/4.0, 1.0/9.0, 1.0/16.0, 1.0/25.0, 1.0/36.0, 1.0/49.0, 0.0, 1.0/81.0};
    stk::StkFloat overtones[8] = {1.0, 2.06, 3.15, 2.06*2.06, 5.35, 2.06*3.15, 7.0, 2.06 * 2.06 * 2.06};
    stk::StkFloat amplitude = 1.0;
    stk::StkFloat value;
    stk::ADSR* adsr;
  public:
    Voice();
    ~Voice();
    void setMidicode(int midicode);
    void setBending(stk::StkFloat bending);
    void noteOn();
    void noteOff();
    stk::StkFloat tick();
};

#endif
