
#include "AbstractReverberator.h"
#include "ReverberatorFreeVerb.h"


ReverberatorFreeVerb::ReverberatorFreeVerb(Config* config) : AbstractReverberator(config) {
    freeVerb = new stk::FreeVerb();
  if (!config->name_occurs("free-verb-mix")) {
    throw std::invalid_argument("Parameter free-verb-mix not defined.");
  }
  freeVerb->setEffectMix(config->get_float("free-verb-mix"));
  if (!config->name_occurs("free-verb-room-size")) {
    throw std::invalid_argument("Parameter free-verb-room-size not defined.");
  }
  freeVerb->setRoomSize(config->get_float("free-verb-room-size"));
  if (!config->name_occurs("free-verb-damping")) {
    throw std::invalid_argument("Parameter free-verb-damping not defined.");
  }
  freeVerb->setDamping(config->get_float("free-verb-damping"));
  if (!config->name_occurs("free-verb-width")) {
    throw std::invalid_argument("Parameter free-verb-width not defined.");
  }
  freeVerb->setWidth(config->get_float("free-verb-width"));


}

ReverberatorFreeVerb::~ReverberatorFreeVerb() {
    delete freeVerb;
}

stk::StkFloat ReverberatorFreeVerb::tick(stk::StkFloat inputL, stk::StkFloat inputR, unsigned int channel) {
    return freeVerb->tick(inputL, inputR, channel);
}
stk::StkFloat ReverberatorFreeVerb::lastOut(unsigned int channel) {
    return freeVerb->lastOut(channel);
}

