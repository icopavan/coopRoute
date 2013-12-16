#include<iostream>
using namespace std;

#include "sim.h"
#include "userVisitor.h"

Simulation* Simulation::simInstance = 0;

Simulation:: Simulation(int t, int np, int ns, int nr, int nc)
  :userList(np+ns+np){
  curTime = 0;
  simTime = t;
  nPUser = np;
  nSUser = ns;   
  topo = new RoundTopology(np, ns, nr);	
}


Simulation* Simulation:: getInstance(int t, int np, int ns, int nr, int nc){
  if (simInstance == NULL){
    simInstance = new Simulation(t, np, ns, nr, nc);
  }
  return simInstance;	
} 

void Simulation::createNodeList(int chcond, int alg, double prate, double srate)
{
  int i, nUser;
  nUser = nPUser + nSUser;

  for(i = 0;i<nPUser;i++){
    userList[i] = (User *) new PrimaryUser(i,prate);
    userList[i]->setTarget(i+nUser);
  }    
  for(;i<nUser;i++){
    userList[i] = (User *) new SecondaryUser(i,srate);
  }
  for(;i<nUser+nPUser;i++){
    userList[i] = (User*) new PrimaryUser(i, 0.0);
  }
  std::cout<< nUser << " Users created" << endl;

}

Simulation::~Simulation()
{
  int i;
  for(i=0;i<nPUser+nPUser+nSUser;i++){
    if(userList[i] != NULL){
      delete userList[i];
    }
  }
  userList.clear();
  if(topo){
    delete topo;	
  }
}

void Simulation::createNeighborList(bool isPrint = false)
{
  int nU = nPUser + nSUser;
  for(int i=0;i<nPUser;i++){
    for(int j=nPUser;j<nU;j++){
      if(topo->isNeighbor(i,j)){
	if(!isPrint){
	  userList[i]->addNeighbor(userList[j]);
	  userList[j]->addNeighbor(userList[i]);
	}
       	cout <<"user " << i <<": user " << j<< endl;
      }
    }
  }
  for(int i=nPUser;i<nU;i++){
    for(int j=i+1;j<nU;j++){
      if(topo->isNeighbor(i,j)){
	if(!isPrint){
	  userList[i]->addNeighbor(userList[j]);
	  userList[j]->addNeighbor(userList[i]);
	}
       	cout <<"user " << i <<": user " << j<< endl;
      }
    }
  }
  
  // determine the target neighborhood
  for(int i = nPUser; i < nU; i++){
    for(int j = nU; j < nU + nPUser; j++){
      if(topo->isNeighbor(i,j)){
	userList[i]->addNeighbor(userList[j]);
	cout <<"user " << i <<": user " << j<< endl;
      }
    }
  }
}

void Simulation::generatePkts(double t)
{
  int nU = nPUser + nSUser;
  for(int i=0;i<nU;i++){
    userList[i]->generateTraffic(t);
  }
}

void Simulation :: routeSetup(double curTime)
{
  // send the route request packet
  int nU = nPUser + nSUser;
  for(int i = 0;i < nU; i++){
    userList[i]->step();
  }
}

void Simulation :: show_results()
{
  int i;
  int nU = nPUser + nSUser;

  UserVisitor* pVisitor = new CountVisitor();
  for(i=0;i<nPUser;i++){
    userList[i]->accept(pVisitor);
  }
  UserVisitor* sVisitor = new CountVisitor();
  for(;i<nU;i++){
    userList[i]->accept(sVisitor);
  }

}
void Simulation::init(int chcond, int alg, double prate, double srate)
{
  createNodeList(chcond, alg, prate, srate);	
  topo->addLocation();
  createNeighborList();
}

    
void Simulation :: run()
{
  for(int curTime = 0;curTime < simTime; curTime++){
    routeSetup(curTime);
  }  
}
