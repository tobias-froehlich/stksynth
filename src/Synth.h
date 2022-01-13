#ifndef SYNTH_H_
#define SYNTH_H_

#include <vector>
#include <string>
#include <FileWvOut.h>
#include "Config.h"
#include "Voice.h"

class Synth {
    private:
        unsigned int nVoices;
        std::vector<int> channels;
        std::vector<Voice*> voices;
        unsigned int getIndexFromChannel(int channel);
        stk::StkFloat maxBending;
        stk::StkFloat overallAmplitude;
        std::string outputFileName;
        stk::FileWvOut* outputFile;
        int isRecording = 0;
    public:
        Synth(Config* config);
        ~Synth();
        void setMidicode(int channel, int midicode);
        void setBending(int channel, stk::StkFloat bending);
        void noteOn(int channel);
        void noteOff(int channel);
        void startRecording();
        void stopRecording();
        stk::StkFloat tick();
        void tick(stk::StkFloat* samples, unsigned int nChannels, unsigned int bufferSize);
};

#endif
