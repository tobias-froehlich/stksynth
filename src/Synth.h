#ifndef SYNTH_H_
#define SYNTH_H_

#include "Generator.h"
#include "Voice.h"

class Synth : public stk::Generator {
    private:
        Voice* voices[16];
    public:
        Synth();
        ~Synth();
        void setMidicode(int channel, int midicode);
        void setBending(int channel, stk::StkFloat bending);
        void noteOn(int channel);
        void noteOff(int channel);
        stk::StkFloat tick();
        stk::StkFrames& tick( stk::StkFrames& frames, unsigned int channel = 0);
};

#endif
