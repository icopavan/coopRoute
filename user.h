#ifndef USER_H
#define USER_H
#include<vector>
#include<queue>
#include<map>

#include "topo.h"
#include "packet.h"
#include "util.h"

class UserVisitor;
class Agent;

class User
{
 private:
  int id;
  int targetId;
  Loc userPos;
  double trafficRate;
  int round;

  //profit calculation weight factor
  double wfactor;

  User();
 public:
  User(int i, double r);
  ~User();
  void accept(UserVisitor *uv);

  /* get methods */
  inline int    getId()const{return id;}
  inline int    getTarget()const{return targetId;}
  inline double getTrafficRate()const{return trafficRate;}
  inline double getFactor()const {return wfactor;}
  inline int    getRound() const {return round;}
  inline int    numOfNeighbors(){return neighborList.size();}  
  virtual double getResourceNeeded()const {return 0;}
  double getCurrentProfitFor(int);
  double getLastProfitFor(int);
  double getCurrentProfitExcept(int);
  double getLastProfitExcept(int);
  double getRate(int id);
  inline User* getNeighbor(int i){return neighborList[i];}
  inline int numOfAgents(){return agentList.size();}
  User* getNeighborById(int id);

 /* set methods */
  inline void setTarget(int id){targetId = id;}
  inline void  advanceRound(){round++;}
  inline void setFactor(double w){wfactor = w;}
  /* neighborhood and traffic related methods */
  void addNeighbor(User*);
  bool isNeighbor(int id);

  /* packet related methods */
  virtual void generateTraffic(double t);
  virtual void step();
  
  /* pure virtual functions */
  virtual void sendPacket() = 0;
  virtual void receivePacket(Packet*, int) = 0;
  virtual Agent* findAgent(int id) = 0;
  virtual double calculateDelayOffer(){return 0;}
 protected:
  /* list of neighbors*/
  std :: vector<User*> neighborList;
  /*list of data packets in queue*/
  std :: deque<Packet*> que;
  /*coop offers received from neighbors in the form of control packets*/
  std ::vector<Agent*> agentList;
  //  CoopTable cTable;
};

class PrimaryUser:public User
{
 public:
  PrimaryUser(int i, double r);
  Agent* findAgent(int id);
  virtual void sendPacket();
  virtual void receivePacket(Packet*, int);
 private:
};

class SecondaryUser:public User
{
 public:
  SecondaryUser(int i, double rate);
  virtual void sendPacket();
  virtual void receivePacket(Packet*, int);
  Agent* findAgent(int src);
  double getResourceNeeded(){return reqPeriod;}
 private:
  // needed only for sec users
  double reqPeriod;

};
#endif
