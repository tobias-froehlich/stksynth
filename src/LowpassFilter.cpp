#include "LowpassFilter.h"

LowpassFilter::LowpassFilter(stk::StkFloat filterFrequency) {
  filterFactor = filterFrequency / 44100.0;
}

LowpassFilter::~LowpassFilter() {
}

stk::StkFloat LowpassFilter::tick(stk::StkFloat inputValue) {
  outputValue = outputValue * (1.0 - filterFactor) + inputValue * filterFactor;                                
  return outputValue;
}
