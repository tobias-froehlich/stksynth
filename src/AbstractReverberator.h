#ifndef ABSTRACT_REVERBERATOR_H_
#define ABSTRACT_REVERBERATOR_H_

#include <Stk.h>
#include "Config.h"

class AbstractReverberator {
    private:
    public:
        AbstractReverberator(Config* config);
        virtual ~AbstractReverberator();
        virtual stk::StkFloat tick(stk::StkFloat inputL, stk::StkFloat inputR, unsigned int channel);
        virtual stk::StkFloat lastOut(unsigned int channel);
};

#endif
