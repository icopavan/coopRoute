#ifndef REPORT_H
#define REPORT_H
#include<string>
//#include<iostream>
#include<fstream>
using namespace std;

class User;
class Packet;

class Report{
 private:
  std::ofstream iofile; 
  Report(char* fname);
  static Report* rInstance;
 public:
  static Report* getInstance(char* a = NULL);
  ~Report();
  void addReport(string);
  void addReport(User*, User*,Packet*, string);
  void addReport(User*, int, Packet*, string);
};
#endif
