#ifndef SINK
#define SINK

#include <omnetpp.h>
#include <string.h>

using namespace omnetpp;

class Sink : public cSimpleModule {
  private:
    // stats
    cStdDev delayStats;
    cOutVector delayVector;
    int packetsReceived;
    cOutVector packetsReceivedVector;

  public:
    Sink();
    virtual ~Sink();

  protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(Sink);

Sink::Sink() {
}

Sink::~Sink() {
}

void Sink::initialize() {
    // Initialite stats
    delayStats.setName("TotalDelay");
    delayVector.setName("Delay");
    packetsReceived = 0;
    packetsReceivedVector.setName("Received packets");
}

void Sink::finish() {
    // Record stats at the end of simulation
    recordScalar("Avg delay", delayStats.getMean());
    recordScalar("Number of packets", delayStats.getCount());
}

void Sink::handleMessage(cMessage *msg) {
    // Compute queuing delay
    simtime_t delay = simTime() - msg->getCreationTime();

    // Update stats
    delayStats.collect(delay);
    delayVector.record(delay);
    packetsReceived++;
    packetsReceivedVector.record(packetsReceived);

    // Delete msg
    delete msg;
}

#endif /* SINK */
