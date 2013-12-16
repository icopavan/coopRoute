#ifndef AGENT_H
#define AGENT_H
#include "user.h"
#include "util.h"

class Packet;

class Agent
{
 private:
  User* u;
  int src;                 // represents which primary user's flow
  int state;
  double beta;             // calculates the profit difference for each flow
  double maxAcceptedOffer;  // starts from 0 to upwards
  double minRejectedOffer; // starts from 1 to downwards
  double offerPrev;        // offer in previous round
  double offerCurrent;     // offer in current round
  bool isAccepted;         // is the offer accepted?
  int timer;
 public:
  Agent(User* u, int a);
  User* getUser()const{return u;}
  int getSrc(){return src;}

  virtual void addOffer(Packet*, int);
  virtual void addRecipient(int sender);
  virtual double calculateDelayOffer(){return 0;}
  inline int    getState() const {return state;}
  inline bool getStatus()const{return isAccepted;}
  inline bool isOfferProvider(int sender) {return coopOffer.count(sender)>0;}
  inline bool isRecipient(int sender){return recipientList.count(sender)>0;}
  inline bool isEqual(){return recipientList.size()==coopOffer.size();}
  inline double getBeta()const{return beta;}
  inline double getLastOffer()const{return offerPrev;}
  inline double getCurrentOffer()const{return offerCurrent;}
  inline double getMaxOffer()const{return maxAcceptedOffer;}
  inline double getMinOffer()const{return minRejectedOffer;}

  inline void setBeta(double s){beta = s;}
  inline void setState(int s){state = s;}
  inline void setStatus(bool b){isAccepted = b;}
  inline void setMaxOffer(double off){maxAcceptedOffer = off;}
  inline void setMinOffer(double off){minRejectedOffer = off;}
  inline void setTimer(int t){timer = t;}
  inline bool isExpired(){return timer--<= 0;}
  Packet* getOffer(int id){return coopOffer[id];}
  void setCurrentOffer(double p){
    offerPrev = offerCurrent;
    offerCurrent = p;
  }
  virtual void step(){}
  virtual void sendPacket()=0;
  virtual void receivePacket(Packet*, int)=0;
  virtual double calculateProfit(Packet*)=0;
  
  virtual void forwardPacket(Packet*){}
  virtual void clear();
  virtual void clearOffer();  
  virtual int selectBestOffer();
  virtual void updateRecord(){}
 protected:
  std :: map <int,Packet*> coopOffer;
  std :: map <int, int> recipientList;
};

class PUAgent : public Agent
{
 private:
 public:
 PUAgent(User* u, int a):Agent(u,a){}
  void step();
  void sendPacket();
  void receivePacket(Packet*, int);
  virtual double calculateProfit(Packet*);
  void updateRecord();
 protected:
};
class SUAgent : public Agent
{
 private:
 public:
 SUAgent(User* u, int a): Agent(u,a){}
  void step();
  void sendPacket();
  void receivePacket(Packet*, int);
  void forwardPacket(Packet* pkt);
  virtual double calculateProfit(Packet*);
  double calculateDelayOffer();
 protected:
};
#endif
