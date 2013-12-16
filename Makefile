all: simulation 
CFLAGS = -Wall -g
simulation: main.o 
#	g++ 
main.o: main.cpp sim.o report.o
	g++ $(CFLAGS) -o main.o main.cpp sim.o user.o topo.o binsearch.o trafficgen.o userVisitor.o coopTable.o ctableEntry.o packetfactory.o packet.o agent.o packethandler.o report.o

user.o: user.cpp user.h  packet.o topo.o util.h userVisitor.o coopTable.o ctableEntry.o packetfactory.o agent.o trafficgen.o
	g++ $(CFLAGS) -c user.cpp
trafficgen.o: trafficgen.cpp trafficgen.h util.h
	g++ $(CFLAGS) -c trafficgen.cpp
sim.o: sim.cpp sim.h user.o topo.o
	g++ $(CFLAGS) -c sim.cpp 
topo.o: topo.cpp topo.h binsearch.o
	g++ $(CFLAGS) -c topo.cpp 
binsearch.o: binsearch.cpp binsearch.h
	g++ $(CFLAGS) -c binsearch.cpp
userVisitor.o: userVisitor.cpp userVisitor.h
	g++ $(CFLAGS) -c userVisitor.cpp
coopTable.o: coopTable.h coopTable.cpp ctableEntry.o
	g++ $(CFLAGS) -c coopTable.cpp
ctableEntry.o: ctableEntry.h ctableEntry.cpp user.h
	g++ $(CFLAGS) -c ctableEntry.cpp
packetfactory.o: packetfactory.h packetfactory.cpp packet.o
	g++ $(CFLAGS) -c packetfactory.cpp
packet.o: packet.h packet.cpp
	g++ $(CFLAGS) -c packet.cpp
agent.o: agent.h agent.cpp user.h packethandler.o
	g++ $(CFLAGS) -c agent.cpp
packethandler.o: packethandler.h packethandler.cpp agent.h packet.h
	g++ $(CFLAGS) -c packethandler.cpp
report.o: report.h report.cpp
	g++ $(CFLAGS) -c report.cpp

clean:
	rm *.o
