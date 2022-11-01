#ifndef REVERBERATOR_N_REV_H_
#define REVERBERATOR_N_REV_H_

#include <NRev.h>
#include "AbstractReverberator.h"

class ReverberatorNRev : public AbstractReverberator {
    private:
        stk::NRev* nRev;
        stk::StkFloat mix;
        stk::StkFloat lastInputL;
        stk::StkFloat lastInputR;
    public:
        ReverberatorNRev(Config* config);
        virtual ~ReverberatorNRev();
        stk::StkFloat tick(stk::StkFloat inputL, stk::StkFloat inputR, unsigned int channel);
        stk::StkFloat lastOut(unsigned int channel);
};

#endif
