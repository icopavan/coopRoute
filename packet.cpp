#include "packet.h"
#include<iostream>
using namespace std;

Offer :: Offer()
{
  dataRate = 0;
  delay = 0;
  demand = 0;
}

Offer :: Offer(const Offer &off){
  dataRate = off.dataRate;
  delay = off.delay;
  demand = off.demand;
}
Packet::Packet(int type){
  pktType = type;
  id = 0;
  destination = -1;
  source = -1;
  //bidRequest = 0; 
  //bidOffer = 0;
  off = new Offer();

}

Packet :: ~Packet()
{
  if(off != NULL){
    delete off;
    off = NULL;
  }
}
Packet::Packet(const Packet &pkt){
  id = pkt.id;
  pktType = pkt.pktType;
  source = pkt.source;
  destination = pkt.destination;
  off = new Offer();
  off->setRate(pkt.off->getRate());
  off->setDelay(pkt.off->getDelay());
  off->setDemand(pkt.off->getDemand());
}
