#ifndef SIM_H
#define SIM_H
#include<iostream>
#include<vector>
using namespace std;

#include "user.h"
#include "topo.h"
#include "userVisitor.h"

class Simulation{

 private:// simulation parameters
		Topology *topo;
		vector<User *> userList;
		int curTime;
		int simTime;
                int nPUser;
                int nSUser;
 		Simulation(int t, int np, int ns, int nr, int nc);
		static Simulation* simInstance;
		void createNodeList(int,int,double,double);
		void createNeighborList(bool);
		void generatePkts(double);
		void routeSetup(double);

	public:
		static Simulation* getInstance(int t, int np, int ns, int nr, int nc);
		void init(int chcond, int alg, double pr, double sr);
                void run();
		void show_results();
                ~Simulation();
};

#endif
