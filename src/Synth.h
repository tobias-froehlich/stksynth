#ifndef SYNTH_H_
#define SYNTH_H_

#include <vector>
#include <string>
#include <FileWvOut.h>
#include <BiQuad.h>
#include <Chorus.h>
#include <FreeVerb.h>
#include "Config.h"
#include "Voice.h"

class Synth {
    private:
        unsigned int nVoices;
        std::vector<int> channels;
        std::vector<Voice*> voices;
        stk::StkFloat maxBending;
        stk::StkFloat overallAmplitude;
        std::string outputFileName;
        stk::FileWvOut* outputFile;
        int recording = 0;
        stk::BiQuad* filter;
        stk::Chorus* chorus;
        stk::FreeVerb* freeVerb;
        unsigned int getIndexFromChannel(int channel);
    public:
        Synth(Config* config);
        ~Synth();
        void setMidicode(int channel, int midicode);
        void setVelocity(int channel, int velocity);
        void setBending(int channel, stk::StkFloat bending);
        void noteOn(int channel);
        void noteOff(int channel);
        void startRecording();
        void stopRecording();
        int isRecording();
        stk::StkFloat tick();
        void tick(stk::StkFloat* samples, unsigned int nChannels, unsigned int bufferSize);
};

#endif
