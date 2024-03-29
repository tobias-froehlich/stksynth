#ifndef SYNTH_H_
#define SYNTH_H_

#include <vector>
#include <string>
#include <FileWvOut.h>
#include <BiQuad.h>
#include <Chorus.h>
#include "AbstractReverberator.h"
#include "Config.h"
#include "Sampler.h"
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
        stk::StkFloat filterResonanceMix;
        stk::BiQuad* filter;
        int useChorus = 0;
        stk::Chorus* chorus;
        int useReverberator = 0;
        AbstractReverberator* reverberator;
        unsigned int getIndexFromChannel(int channel);
        Sampler sampler;
        int isLoading;
    public:
        Synth(Config* config);
        void init(Config* config);
        ~Synth();
        void destroy();
        void reload(Config* config);
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
