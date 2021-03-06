#ifndef TOPO_H
#define TOPO_H
#include<iostream>
#include<cstdlib>
#include<vector>
using namespace std;

#include "binsearch.h"

typedef struct{
  double x;
  double y;
}Loc;


class Topology{
 private:
  int sizex;
  int sizey;
  int pu;
  int su;
  Tree *t;
  double txRange;

 public:
  Topology(int r,int c,int p,int s);
  ~Topology();
  virtual void  addLocation();
  virtual void printLocation();
  bool isNeighbor(int, int);
 protected:
  virtual void  addPULocation(int i);
  virtual void addSULocation(int i);
  virtual void addTargetLocation(int i);
  virtual Loc getUserLocation(int i);
  static int randindex(int start, int end);
  vector<Loc> nodeLocation;
};

class RoundTopology : public Topology{
 public:
  RoundTopology(int a, int b, int r);
  ~RoundTopology();
  double getRadius()const{return radius;}
 protected:
  void addPULocation(int i);
  void addSULocation(int i);
  //  void addTargetLocation(int i);
 private:
  double radius;
};
#endif
