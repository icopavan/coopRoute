#include "userVisitor.h"

void CountVisitor::visitDefault(User* u){
  countDirPkts(u);
  countRlPkts(u);
  countDropPkts(u);
  countGenPkts(u);
}

void CountVisitor::countDirPkts(User* u){
  //  dirPkts += u->getDirPkts();
}
void CountVisitor::countRlPkts(User* u){
  //  relPkts += u->getRelPkts();
}
void CountVisitor::countDropPkts(User* u){
  //  dropPkts += u->getDropPkts();
}
void CountVisitor::countGenPkts(User* u){
  //  genPkts += u->getGenPkts();
}
void CountVisitor::countNeighbors(User* u){
  neigh += u->numOfNeighbors();
}
