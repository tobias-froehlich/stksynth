
#include "AbstractReverberator.h"
#include "ReverberatorNRev.h"
#include "const.h"

ReverberatorNRev::ReverberatorNRev(Config* config) : AbstractReverberator(config) {
    nRev = new stk::NRev();
    if (!config->name_occurs("n-rev-mix")) {
      throw std::invalid_argument("Parameter n-rev-mix not defined.");
    }
    mix = config->get_float("n-rev-mix");
    if (mix < 0.0 - cFloatDelta) {
      throw std::invalid_argument("Parameter n-ref-mix must be greater than 0.0.");
    }
    if (!config->name_occurs("n-rev-t60")) {
      throw std::invalid_argument("Parameter n-rev-t60 not defined.");
    }
    nRev->setT60(config->get_float("n-rev-t60"));
}

ReverberatorNRev::~ReverberatorNRev() {
    delete nRev;
}

stk::StkFloat ReverberatorNRev::tick(stk::StkFloat inputL, stk::StkFloat inputR, unsigned int channel) {
    lastInputL = inputL;
    lastInputR = inputR;
    nRev->tick(inputL + inputR, 0);
    return lastOut(channel);
}
stk::StkFloat ReverberatorNRev::lastOut(unsigned int channel) {
    if (channel == 0) {
        return lastInputL * (1.0 - mix) + nRev->lastOut(0) * mix;
    }
    else {
        return lastInputR * (1.0 - mix) + nRev->lastOut(1) * mix;
    }
}

