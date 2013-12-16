#include "packetfactory.h"
#include "util.h"
#include "user.h"
#include "agent.h"

Packet* RequestPacketFactory::create(Agent* a){
  Packet* pkt = new RequestPacket(ROUTE_REQ);
  User* u = a->getUser();
  pkt->setDest(u->getTarget());
  pkt->setId(u->getRound());
  pkt->setSrc(u->getId());
  // pkt->setProfit();// 0 or last profit
  return pkt;
}

// first select the best offer from your neighbors
// then call your method to add your offer with it
// add your offer with the selected one
// update the ctable to indicate who is selected and what is your 
// offer
  
Packet* ReplyPacketFactory::create(Agent* a){
  Packet *pkt, *cPkt;
  double cRate = RAND_MAX, cDelay = 0;
  User* self = a->getUser();
  pkt = new ReplyPacket(ROUTE_REPLY);
  pkt->setId(a->getUser()->getRound());

  int uid = a->selectBestOffer();
  if(uid != -1){
    cPkt = a->getOffer(uid);
    cRate = cPkt->getOffer()->getRate();
    cDelay = cPkt->getOffer()->getDelay();
    pkt->setSrc(cPkt->getSrc());
    pkt->setDest(cPkt->getDest());
  }
  else{// we have to fix it
    pkt->setSrc(-1);
    pkt->setDest(-1);
  }
  
  double delayFactor = a->calculateDelayOffer();
  pkt->getOffer()->setRate(min(cRate,self->getRate(uid)));
  pkt->getOffer()->setDelay(cDelay + delayFactor*self->getResourceNeeded() + (1 - self->getResourceNeeded())*(1 - delayFactor));

  return pkt;

}

Packet* ConfirmPacketFactory::create(Agent* a){
  Packet* pkt = new ConfirmPacket(ROUTE_CONFIRM);
  pkt->setId(a->getUser()->getRound());
  pkt->setSrc(a->getUser()->getId());
  pkt->setDest(a->getUser()->getTarget());
  return pkt;
}
