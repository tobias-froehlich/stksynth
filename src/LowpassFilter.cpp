#include "LowpassFilter.h"

LowpassFilter::LowpassFilter(stk::StkFloat sampleRate, stk::StkFloat filterFrequency) {
  filterFactor = filterFrequency / sampleRate;
}

LowpassFilter::~LowpassFilter() {
}

stk::StkFloat LowpassFilter::tick(stk::StkFloat inputValue) {
  outputValue = outputValue * (1.0 - filterFactor) + inputValue * filterFactor;                                
  return outputValue;
}
