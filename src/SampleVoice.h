#ifndef SAMPLE_VOICE_H_
#define SAMPLE_VOICE_H_

#include <ADSR.h>
#include <FileWvIn.h>
#include "Voice.h"
#include "Config.h"

class SampleVoice : public Voice {
  private:
    std::vector< stk::StkFloat > sampleFrequencies;
    std::vector< stk::FileWvIn* > sampleFiles;
    stk::FileWvIn* currentSampleFile;
    std::vector< int > sampleIndexForKey;
  public:
    SampleVoice(Config* config);
    virtual ~SampleVoice();
    void setMidicode(int midicode);
    void setBending(stk::StkFloat bending);
    void noteOn();
    void noteOff();
    stk::StkFloat tick();
};

#endif
