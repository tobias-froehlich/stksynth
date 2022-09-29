#ifndef SAMPLER_H_
#define SAMPLER_H_

#include <Stk.h>
#include <vector>
#include <string>

class Sampler {
  private:
    std::vector < std::vector < stk::StkFloat > > sampleData;
    std::vector < stk::StkFloat > referenceFrequencies;
    stk::StkFloat fileRate;
  public:
    Sampler();
    ~Sampler();
    void readSampleFile(std::string sampleFileName, stk::StkFloat referenceFrequency);
    stk::StkFloat getFileRate();
    int getSampleIndex(stk::StkFloat frequency);
    stk::StkFloat getReferenceFrequency(stk::StkFloat frequency);
    stk::StkFloat getValue(int sampleIndex, stk::StkFloat position);
};

#endif
