#ifndef SYNTH_H_
#define SYNTH_H_

#include <vector>
#include <Generator.h>
#include "Config.h"
#include "Voice.h"

class Synth : public stk::Generator {
    private:
        unsigned int nVoices;
        std::vector<int> channels;
        std::vector<Voice*> voices;
        unsigned int getIndexFromChannel(int channel);
        stk::StkFloat maxBending;
    public:
        Synth(Config* config);
        ~Synth();
        void setMidicode(int channel, int midicode);
        void setBending(int channel, stk::StkFloat bending);
        void noteOn(int channel);
        void noteOff(int channel);
        stk::StkFloat tick();
        stk::StkFrames& tick( stk::StkFrames& frames, unsigned int channel = 0);
};

#endif
