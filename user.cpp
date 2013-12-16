#include<iostream>
#include<fstream>
using namespace std;

#include "user.h"
#include "userVisitor.h"
#include "agent.h"
#include "report.h"

User :: User(int i, double r)
{
  id = i;
  trafficRate = r;
  round = 1;
}

PrimaryUser :: PrimaryUser(int i, double r) 
  : User(i,r)
{
  agentList.push_back(new PUAgent(this, -1));
}

SecondaryUser :: SecondaryUser(int i, double rate)
  : User(i,rate)
{

  reqPeriod  = rand()/RAND_MAX;
}

void User :: generateTraffic(double curTime){
  // int x = TrafficGen::generateNumber(getTrafficRate());
  
  // stats.nGPackets += x;
  /*
  while(x>0){
    Packet *dataPkt = new Packet(curTime);
    dataPkt->setPktType(PKT_DATA);
    dataPkt->setSrc(getId());
    que.push_back(dataPkt);
    x--;
    }*/
}

void User :: accept(UserVisitor *uv){
  uv->visitDefault(this);
}

User* User :: getNeighborById(int id){
  for(unsigned int i = 0; i<neighborList.size(); i++){
    if(id == neighborList[i]->getId()){
      return neighborList[i];
    }
  }
  return NULL;
}

void User :: addNeighbor(User* u){
  if(u == this) return;
  neighborList.push_back(u);
}

bool User :: isNeighbor(int id){
  for(unsigned int i = 0; i<neighborList.size(); i++){
    if(id == neighborList[i]->getId()){
      return true;
    }
  }
  return false;
}

User :: ~User(){
  for(unsigned int i = 0;i < agentList.size(); i++){
    delete agentList[i];
  }
  agentList.clear();
}

void User :: step(){
  for(unsigned int i = 0;i<agentList.size();i++){
    agentList[i]->step();
  }
}

double User :: getCurrentProfitFor(int i){
  if(i <(int) agentList.size()){
    return agentList[i]->getCurrentOffer();
  }
  return -1;
}

// not sure whether we need it
double User :: getLastProfitFor(int i){
  if(i < (int) agentList.size()){
    return agentList[i]->getLastOffer();
  }
  return -1;
}

double User :: getCurrentProfitExcept(int index){
  double total = 0;
  for(int i = 0; i < (int) agentList.size(); i++){
    if(index == i) continue;
    total += agentList[i]->getCurrentOffer();
  }
  return total;
}

double User :: getLastProfitExcept(int index){
  double total = 0;
  for(int i = 0; i < (int) agentList.size(); i++){
    if(index == i) continue;
    total += agentList[i]->getLastOffer();
  }
  return total;
}
// make sure each link has different data rate based on their location
double User :: getRate(int id){
  return rand()/RAND_MAX;
}
Agent* PrimaryUser :: findAgent(int src)
{
  return agentList[0];
}

Agent* SecondaryUser :: findAgent(int src){
  for(unsigned int i = 0;i < agentList.size(); i++){
    if(agentList[i]->getSrc() == src)
      return agentList[i];
  }
  Agent* agent = new SUAgent(this, src);
  agentList.push_back(agent);

  return agent;  
}

void PrimaryUser :: sendPacket()
{
}

void PrimaryUser :: receivePacket(Packet* pkt, int sender)
{
}

// no need
void SecondaryUser :: sendPacket()
{
  // who is the src?
  //int src = pkt->getSrc();
  //find(src)->sendPacket();
}

void SecondaryUser :: receivePacket(Packet* pkt, int sender){
  // who is the src?
  int src = pkt->getSrc();
  Agent* agent = findAgent(src);
  agent->receivePacket(pkt, sender);
}

