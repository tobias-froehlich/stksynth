#ifndef LOWPASS_FILTER_H_
#define LOWPASS_FILTER_H_

#include <Stk.h>
#include "Config.h"

class LowpassFilter {
    private:
        stk::StkFloat filterFactor;
        stk::StkFloat outputValue = 0.0;

    public:
        LowpassFilter(stk::StkFloat filterFrequency);
        ~LowpassFilter();

        stk::StkFloat tick(stk::StkFloat inputValue);
};

#endif
