
#include "AbstractReverberator.h"
#include "ReverberatorPRCRev.h"
#include "const.h"

ReverberatorPRCRev::ReverberatorPRCRev(Config* config) : AbstractReverberator(config) {
    prcRev = new stk::PRCRev();
    if (!config->name_occurs("prc-rev-mix")) {
      throw std::invalid_argument("Parameter prc-rev-mix not defined.");
    }
    mix = config->get_float("prc-rev-mix");
    if (mix < 0.0 - cFloatDelta) {
      throw std::invalid_argument("Parameter prc-ref-mix must be greater than 0.0.");
    }
    if (!config->name_occurs("prc-rev-t60")) {
      throw std::invalid_argument("Parameter prc-rev-t60 not defined.");
    }
    prcRev->setT60(config->get_float("prc-rev-t60"));
}

ReverberatorPRCRev::~ReverberatorPRCRev() {
    delete prcRev;
}

stk::StkFloat ReverberatorPRCRev::tick(stk::StkFloat inputL, stk::StkFloat inputR, unsigned int channel) {
    lastInputL = inputL;
    lastInputR = inputR;
    prcRev->tick(inputL + inputR, 0);
    return lastOut(channel);
}
stk::StkFloat ReverberatorPRCRev::lastOut(unsigned int channel) {
    if (channel == 0) {
        return lastInputL * (1.0 - mix) + prcRev->lastOut(0) * mix;
    }
    else {
        return lastInputR * (1.0 - mix) + prcRev->lastOut(1) * mix;
    }
}

