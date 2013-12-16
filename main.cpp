#include<iostream>
#include<fstream>
#include<string>
using namespace std;
#include "sim.h"
#include "util.h"
#include "report.h"


int main(){
  int time, np, ns, nrow, ncol;
  double purate,surate;
  time = 5;
  np = 1;
  ns = 10;
  nrow = 20;
  ncol = 5;
  purate = 0.2; // range from 0.1 to 1
  surate = 0.1; // range from 0.1 to 1
  //  const char* str = "output.txt";
  Report *r = Report::getInstance("output.txt");
  Simulation* sim = Simulation::getInstance(time,np,ns,nrow,ncol);
  /* create nodes, place them, calculate neighbors */
  sim->init(1,1,purate,surate);	
  sim->run();
  //sim->show_results();
  delete sim;
  Report::getInstance()->addReport("win");
  delete r;

  return 0;
}
