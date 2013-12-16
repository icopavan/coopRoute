#include<fstream>
using namespace std;

#include "agent.h"
#include "packet.h"
#include "packethandler.h"
#include "report.h"
#include "packetfactory.h"

Agent::Agent(User* u, int src){
  this->u = u;
  this->src = src;
  state = ST_IDLE;
  beta = 1;
  maxAcceptedOffer = 0.0;
  minRejectedOffer = 1.0;
  offerPrev = 0.0;
  offerCurrent = 0.0;
  isAccepted = false;
}

void Agent :: addRecipient(int sender){
  if(!isOfferProvider(sender) && !isRecipient(sender)){
    recipientList[sender] = 1;
  }
}

// to break the loop, a neighbor can either be recipient or 
// offer provider
void Agent :: addOffer(Packet* pkt, int sender){
  if(!isOfferProvider(sender) && !isRecipient(sender)){
    Report::getInstance()->addReport(getUser(), sender, pkt,"r");
    coopOffer[sender] = pkt;
    if(coopOffer.size() == getUser()->numOfNeighbors() - recipientList.size()){
      // go to decision state
      setState(ST_DECIDE);
    }
  }
}

void Agent :: clear(){
  clearOffer();
  recipientList.clear();
}

void Agent :: clearOffer(){
  std::map<int, Packet*>::iterator it;
  for(it = coopOffer.begin(); it != coopOffer.end();it++){
    delete (*it).second;
  }
  coopOffer.clear();
}

void SUAgent :: step(){
  switch(getState()){
  case ST_IDLE:
   
    //    forwardPacket();
    //break;
  case ST_WAITING:
    // receives neighbors' reply, and changes the state 
    // to decide state if all received
    // waits to receive all replies from the neighbors
    // forwardPacket();
    break;
  case ST_DECIDE:
    // decides the best one, keeps record, and 
    // sends reply to its follower, and changes
    // the state to idle state.
    //    setState(ST_IDLE);
    sendPacket();
    setState(ST_IDLE);
    break;
  default:
    break;
  }
}

void SUAgent ::forwardPacket(Packet *pkt)
{
  User* u = getUser();
  switch(pkt->getPktType()){
  case ROUTE_REQ:
    for(int i = 0;i< u->numOfNeighbors();i++){
      if(!isRecipient(u->getNeighbor(i)->getId())){
	Report::getInstance()->addReport(u, u->getNeighbor(i), pkt,"s");
	u->getNeighbor(i)->receivePacket(pkt->clone(),u->getId());
      }
    }
    break;
  case ROUTE_CONFIRM:
    int id ;
    id = selectBestOffer();
    Report::getInstance()->addReport(u, u->getNeighbor(id), pkt,"c");
    u->getNeighborById(id)->receivePacket(pkt->clone(), u->getId());
    break;
  default:
    break;
  }

}

void SUAgent :: sendPacket(){
  User* u = getUser();
  PacketFactory* pktFactory;
  Packet* repPkt;
  switch(getState()){
    // there are two cases
    // it can initiate the reply
    // or, it adds its own offer and contribution
    // with it
  case ST_DECIDE:
    pktFactory = new ReplyPacketFactory();
    // reply deals with neighbors' offers and also considers its own
    // demand and contribution
    repPkt = pktFactory->create(this);

    for(int i=0;i<u->numOfNeighbors();i++){
      if(isRecipient(u->getNeighbor(i)->getId())){
	Report::getInstance()->addReport(u, u->getNeighbor(i),repPkt,"rp");
	u->getNeighbor(i)->receivePacket(repPkt->clone(),u->getId());
      }
    }
    delete repPkt;
    delete pktFactory;
    break;
  default:
    break;
  }
}

void SUAgent :: receivePacket(Packet* pkt, int sender){
  PacketHandler *h;
  switch(pkt->getPktType()){
  case ROUTE_REQ:
    h = new RouteReqPktHandler(this);
    h->handle(pkt, sender);
    delete h;
    delete pkt;
    break;
  case ROUTE_REPLY:
    h = new RouteRepPktHandler(this);
    h->handle(pkt, sender);
    delete h;
    delete pkt;
    break;
  case ROUTE_CONFIRM:
    h = new RouteConfirmPktHandler(this);
    h->handle(pkt, sender);
    delete h;
    delete pkt;
    break;
  }
}

void PUAgent :: step(){
  // a primary user can be in  different states
  switch(getState()){
  case ST_IDLE:
    // check the value of beta, if beta > threshold, change 
    // the state to searching state
    if(getBeta() > THRESHOLD){
      // clear offer, recipient list
      clear();
      setState(ST_WAITING);
      sendPacket();
      setTimer(100);// this value should be different for different user
    }
    break;
  case ST_WAITING:
    // receives neighbors' reply, and changes the state 
    // to decide state if all received
    // waits to receive all replies from the neighbors
    if(isExpired()){
      setState(ST_DECIDE);
    }
    cout << "Still waiting" << endl;
    break;
  case ST_DECIDE:
    // decides the best one, keeps record, and 
    // sends reply to its follower, and changes
    // the state to idle state.
    //clears all Offer packets
    setState(ST_IDLE);
    sendPacket();
    break;
  default:
    break;
  }
}


void PUAgent :: sendPacket(){
  PacketFactory *reqPktFactory, *cfirmPktFactory;
  Packet *reqPkt,*cfPkt;
  int index;
  User* u = getUser();
  switch(getState()){
  case ST_WAITING:
    reqPktFactory = new RequestPacketFactory();
    reqPkt = reqPktFactory->create(this);
    for(int i = 0;i < u->numOfNeighbors();i++){
      if(!isRecipient(u->getNeighbor(i)->getId())){
	Report::getInstance()->addReport(u, u->getNeighbor(i),reqPkt,"s");
	u->getNeighbor(i)->receivePacket(reqPkt->clone(), u->getId());
	}  
    }
    delete reqPkt;
    delete reqPktFactory;
    break;
  case ST_DECIDE:
    index = selectBestOffer();
    cfirmPktFactory = new ConfirmPacketFactory();
    cfPkt = cfirmPktFactory->create(this);
    Report::getInstance()->addReport(u, u->getNeighbor(index),cfPkt,"c");
    u->getNeighbor(index)->receivePacket(cfPkt->clone(), u->getId());
    delete cfPkt;
    delete cfirmPktFactory;
    break;
  default:
    break;
  }
}

void PUAgent :: receivePacket(Packet* pkt, int sender){
  switch(pkt->getPktType()){
    // ignore but dont forget to delete the packet
  case ROUTE_REQ:
  case ROUTE_CONFIRM:
    delete pkt;
    break;
  case ROUTE_REPLY:
    addOffer(pkt, sender);
    /*  if(isEqual()){
      setState(ST_DECIDE);
      }*/
    break;
  }
}


int Agent :: selectBestOffer(){
  // for each offer, calculate the profit it can achieve
  // and select the best one
  double pt, lastPt=0;
  int uid =-1;
  std::map<int, Packet*> :: iterator it;
  for(it = coopOffer.begin();it != coopOffer.end();it++){
    pt = calculateProfit(it->second);
    if(pt > lastPt){
      lastPt = pt;
      uid = it->first;
    }
  }

  return uid;
}

void PUAgent :: updateRecord()
{
  User* u = getUser();
  double b = (getCurrentOffer() - getLastOffer()) > 0 ? (getCurrentOffer() - getLastOffer())/u->getRound():(-getCurrentOffer() + getLastOffer())/u->getRound();
  setBeta(b);
  u->advanceRound();
 
}

double  PUAgent :: calculateProfit(Packet* pkt){
  // we can have set/get function to access offer.
  // normalize rate and normalize delay
  return pkt->getOffer()->getRate() + pkt->getOffer()->getDelay();
}

double SUAgent :: calculateProfit(Packet* pkt){
  return -1;
}


double SUAgent :: calculateDelayOffer()
{
  double shareFactor;
  User* u = getUser();
  shareFactor = getCurrentOffer();
  // this is the first offer
  if(u->getRound() == 1){
    shareFactor = 1/u->numOfAgents();
    setCurrentOffer(shareFactor);
  }
  else if(getStatus()){
    setMaxOffer(max(shareFactor, getMaxOffer()));
    shareFactor = (shareFactor + u->getLastProfitExcept(getSrc()))/2; 
  }
  else{
    setMinOffer(min(shareFactor, getMinOffer()));
    shareFactor = shareFactor/2;
  }
  setCurrentOffer(shareFactor);
  return shareFactor;
}
