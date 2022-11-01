#ifndef REVERBERATOR_P_R_C_REV_H_
#define REVERBERATOR_P_R_C_REV_H_

#include <PRCRev.h>
#include "AbstractReverberator.h"

class ReverberatorPRCRev : public AbstractReverberator {
    private:
        stk::PRCRev* prcRev;
        stk::StkFloat mix;
        stk::StkFloat lastInputL;
        stk::StkFloat lastInputR;
    public:
        ReverberatorPRCRev(Config* config);
        virtual ~ReverberatorPRCRev();
        stk::StkFloat tick(stk::StkFloat inputL, stk::StkFloat inputR, unsigned int channel);
        stk::StkFloat lastOut(unsigned int channel);
};

#endif
