#ifndef SINK
#define SINK

#include <omnetpp.h>
#include <string.h>

using namespace omnetpp;

class Sink : public cSimpleModule {
  private:
    // stats
    int packetsReceived;
    cOutVector delayVector;
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
    delayVector.setName("Delay");
    packetsReceived = 0;
    packetsReceivedVector.setName("Received packets");
}

void Sink::finish() {
    // Record stats at the end of simulation
    recordScalar("Received packets", packetsReceived);
}

void Sink::handleMessage(cMessage *msg) {
    // Compute queuing delay
    simtime_t delay = simTime() - msg->getCreationTime();

    // Update stats
    delayVector.record(delay);
    packetsReceived++;
    packetsReceivedVector.record(packetsReceived);

    // Delete msg
    delete msg;
}

#endif /* SINK */
