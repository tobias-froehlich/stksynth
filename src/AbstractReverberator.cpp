#include <Stk.h>
#include "AbstractReverberator.h"


AbstractReverberator::AbstractReverberator(Config* config) {
}

AbstractReverberator::~AbstractReverberator() {
}

stk::StkFloat AbstractReverberator::tick(stk::StkFloat inputL, stk::StkFloat inputR, unsigned int channel) {
    return 0.0;
}

stk::StkFloat AbstractReverberator::lastOut(unsigned int channel) {
    return 0.0;
}
