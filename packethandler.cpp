#include<fstream>
using namespace std;
#include "packethandler.h"
#include "agent.h"
#include "packet.h"
#include "report.h"

PacketHandler :: PacketHandler(Agent* a){
  agent = a;
}

RouteReqPktHandler :: RouteReqPktHandler(Agent *a)
  :PacketHandler(a){}
RouteRepPktHandler :: RouteRepPktHandler(Agent *a)
  :PacketHandler(a){}
RouteConfirmPktHandler :: RouteConfirmPktHandler(Agent *a)
  :PacketHandler(a){}

void RouteReqPktHandler :: handle(Packet *pkt, int sender){
  Agent *agent = getAgent();
  switch(agent->getState()){
  case ST_IDLE:
    if(agent->getBeta()>THRESHOLD){
      Report::getInstance()->addReport(agent->getUser(), sender,pkt,"r");
      agent->addRecipient(sender);
      if(agent->getUser()->isNeighbor(pkt->getDest())){
	// no need to forward its to neighbors
	// go directly to send reply packets
	agent->setState(ST_DECIDE);
      }
      else if(agent->getUser()->numOfNeighbors()==1){
	agent->setState(ST_DECIDE);
      }
      else{
	agent->setState(ST_WAITING);
       	agent->forwardPacket(pkt);
      }
    }

    break;
  case ST_WAITING:
    Report::getInstance()->addReport(agent->getUser(), sender,pkt,"r");
    agent->addRecipient(sender);
    break;
  case ST_DECIDE:
    // should not receive route-req pkt at this state
    break;
  }
}

void RouteRepPktHandler :: handle(Packet *pkt, int sender){
  Agent* agent = getAgent();
  switch(agent->getState()){
  case ST_IDLE:
    break;
  case ST_WAITING:
    agent->addOffer(pkt, sender);
    break;
  case ST_DECIDE:
    break;
  }  
}

void RouteConfirmPktHandler :: handle(Packet *pkt, int sender){
  // either create new state or use st-idle for it
  Agent* agent = getAgent();
  switch(agent->getState()){
  case ST_IDLE:
    // check whether you have unconfirmed offer packet in cooptable
    // it means the user is waiting for confirmation
    agent->setStatus(true);
    agent->forwardPacket(pkt);
    break;
  case ST_WAITING:
    // should not receive a confirmation pkt while waiting
    cout << "should not be here" << endl;
    break;
  case ST_DECIDE:
    // should not receive a confirmation pkt while deciding
    break;
  }
}

// recipientList (who wants reply) + coopOfferList (who replied)
// = neighborList
