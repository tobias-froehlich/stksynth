
#include "AbstractReverberator.h"
#include "ReverberatorJCRev.h"
#include "const.h"

ReverberatorJCRev::ReverberatorJCRev(Config* config) : AbstractReverberator(config) {
    jcRev = new stk::JCRev();
    if (!config->name_occurs("jc-rev-mix")) {
      throw std::invalid_argument("Parameter jc-rev-mix not defined.");
    }
    mix = config->get_float("jc-rev-mix");
    if (mix < 0.0 - cFloatDelta) {
      throw std::invalid_argument("Parameter jc-ref-mix must be greater than 0.0.");
    }
    if (!config->name_occurs("jc-rev-t60")) {
      throw std::invalid_argument("Parameter jc-rev-t60 not defined.");
    }
    jcRev->setT60(config->get_float("jc-rev-t60"));
}

ReverberatorJCRev::~ReverberatorJCRev() {
    delete jcRev;
}

stk::StkFloat ReverberatorJCRev::tick(stk::StkFloat inputL, stk::StkFloat inputR, unsigned int channel) {
    lastInputL = inputL;
    lastInputR = inputR;
    jcRev->tick(inputL + inputR, 0);
    return lastOut(channel);
}
stk::StkFloat ReverberatorJCRev::lastOut(unsigned int channel) {
    if (channel == 0) {
        return lastInputL * (1.0 - mix) + jcRev->lastOut(0) * mix;
    }
    else {
        return lastInputR * (1.0 - mix) + jcRev->lastOut(1) * mix;
    }
}

