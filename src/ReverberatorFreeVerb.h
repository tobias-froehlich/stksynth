#ifndef REVERBERATOR_FREEVERB_H_
#define REVERBERATOR_FREEVERB_H_

#include <FreeVerb.h>
#include "AbstractReverberator.h"

class ReverberatorFreeVerb : public AbstractReverberator {
    private:
        stk::FreeVerb* freeVerb;
    public:
        ReverberatorFreeVerb(Config* config);
        virtual ~ReverberatorFreeVerb();
        stk::StkFloat tick(stk::StkFloat inputL, stk::StkFloat inputR, unsigned int channel);
        stk::StkFloat lastOut(unsigned int channel);
};

#endif
