#ifndef USERVISITOR_H
#define USERVISITOR_H

#include "user.h"


class UserVisitor{
 private:
 
 public:
  UserVisitor(){}
  virtual void visitDefault(User* u){};
  /*  virtual void countDirPkts(User* u){};
  virtual void countRlPkts(User* u){};
  virtual void countDropPkts(User* u){};
  virtual void countGenPkts(User* u){};
  virtual void countNeighbors(User* u){};*/
  virtual int getDirPkts(){return 0;}
  virtual int getRlPkts(){return 0;}
  virtual int getDropPkts(){return 0;}
  virtual int getGenPkts(){return 0;}
  virtual int getNeighbors(){return 0;}
};

class CountVisitor:public UserVisitor{
 private:
  int dirPkts;
  int relPkts;
  int dropPkts;
  int genPkts;
  int neigh;
 public:
 CountVisitor():UserVisitor(){
    dirPkts = 0;
    relPkts = 0;
    dropPkts = 0;
    genPkts = 0;
    neigh = 0;
  }
  void visitDefault(User* u);
  void countDirPkts(User* u);
  void countRlPkts(User* u);
  void countDropPkts(User* u);
  void countGenPkts(User* u);
  void countNeighbors(User* u);

  int getDirPkts(){return dirPkts;}
  int getRlPkts(){return relPkts;}
  int getDropPkts(){return dropPkts;}
  int getGenPkts(){return genPkts;}
  int getNeighbors(){return neigh;}
};
#endif
