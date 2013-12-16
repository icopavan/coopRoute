#include "report.h"
#include "user.h"
#include "packet.h"


Report* Report::rInstance = 0;

Report::Report(char* fname):iofile(fname){
}
Report* Report :: getInstance(char* filename){
  if(rInstance == NULL)
    rInstance = new Report(filename);
  return rInstance;

}

Report :: ~Report()
{
  iofile.close();
}

void Report :: addReport(string str)
{
  iofile << str << endl;

}

void Report :: addReport(User* src, User* dest, Packet* pkt, string s)
{
  iofile << src->getId() <<" "<< s << " " << dest->getId()<<" "
	 << pkt->getPktType()<<" "<<pkt->getSrc()<<" "<<pkt->getDest()<<endl;
}

void Report :: addReport(User* src, int dest, Packet* pkt, string s)
{
  iofile << src->getId() <<" "<< s<< " " << dest <<" "
	 << pkt->getPktType()<<" "<<pkt->getSrc()<<" "<<pkt->getDest()<<endl;
}

