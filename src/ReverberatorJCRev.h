#ifndef REVERBERATOR_J_C_REV_H_
#define REVERBERATOR_J_C_REV_H_

#include <JCRev.h>
#include "AbstractReverberator.h"

class ReverberatorJCRev : public AbstractReverberator {
    private:
        stk::JCRev* jcRev;
        stk::StkFloat mix;
        stk::StkFloat lastInputL;
        stk::StkFloat lastInputR;
    public:
        ReverberatorJCRev(Config* config);
        virtual ~ReverberatorJCRev();
        stk::StkFloat tick(stk::StkFloat inputL, stk::StkFloat inputR, unsigned int channel);
        stk::StkFloat lastOut(unsigned int channel);
};

#endif
