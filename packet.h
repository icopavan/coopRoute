#ifndef PACKET_H
#define PACKET_H

class Offer{
 private:
  double dataRate;
  double delay;
  double demand;
  // need to save the path information 
 public:
  Offer();
  Offer(const Offer &off);
  double getRate(){return dataRate;}
  double getDelay(){return delay;}
  double getDemand(){return demand;}
  void setRate(double rate){dataRate = rate;}
  void setDelay(double d){delay = d;}
  void setDemand(double d){demand = d;}
};

class Packet{
 private:
  int id; // round number
  int pktType; // type of packet: see utils.h
  int destination; // final destination
  int source; // initiator
  Offer* off;
  Packet(); // no implementation for empty constructor
  public:
  Packet(int);
  Packet(const Packet &pkt);
  ~Packet();
  //get and set methods
  inline int getId()const{return id;}
  inline int getPktType()const{return pktType;}
  inline int getDest()const{return destination;}
  inline int getSrc()const{return source;}
  inline Offer* getOffer() const{return off;}

  virtual void addNodeinPath(int){};
  void setId(int i){id = i;}
  void setPktType(int type){pktType = type;}
  void setDest(int id){destination = id;}
  void setSrc(int id){source = id;}
  virtual Packet* clone(){ return 0;}
};

class RequestPacket:public Packet{
 public:
 RequestPacket(int t):Packet(t){}
  Packet* clone(){
    Packet* req = new RequestPacket(*this);
    return req;
  }
 RequestPacket(const RequestPacket &rPkt):Packet(rPkt){
  }
 private:
};

class ReplyPacket:public Packet{
 public:
 ReplyPacket(int t):Packet(t){}
  Packet* clone(){
    return new ReplyPacket(*this);
  }
 ReplyPacket(const ReplyPacket &rPkt):Packet(rPkt){
  }
 private:
};

class ConfirmPacket:public Packet{
 public:
 ConfirmPacket(int t):Packet(t){}
  Packet* clone(){
    return new ConfirmPacket(*this);
  }
 ConfirmPacket(const ConfirmPacket &cPkt):Packet(cPkt){
  }
 private:
};

#endif

