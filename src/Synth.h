#ifndef SYNTH_H_
#define SYNTH_H_

#include <vector>
#include "Config.h"
#include "Voice.h"

class Synth {
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
        void tick(stk::StkFloat* samples, unsigned int nChannels, unsigned int bufferSize);
};

#endif
