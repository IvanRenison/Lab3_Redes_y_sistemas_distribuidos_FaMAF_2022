#ifndef GENERATOR
#define GENERATOR

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Generator : public cSimpleModule {
private:
    // events
    cMessage *sendMsgEvent;

    // stats
    cStdDev transmissionStats;
    int packetsSent;
    cOutVector packetsSentVector;

    // functions
    void sendPacket();
public:
    Generator();
    virtual ~Generator();
protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};
Define_Module(Generator);

Generator::Generator() {
    sendMsgEvent = NULL;
}

Generator::~Generator() {
    cancelAndDelete(sendMsgEvent);
}

void Generator::initialize() {
    // Initialite events
    sendMsgEvent = new cMessage("sendEvent");

    // Initialite stats
    transmissionStats.setName("TotalTransmissions");
    packetsSent = 0;
    packetsSentVector.setName("Sent packets");

    // Schedule the first event at random time
    scheduleAt(par("generationInterval"), sendMsgEvent);
}

void Generator::finish() {
}

/* Send a new packet */
void Generator::sendPacket() {
    // Create new packet
    cPacket *pkt = new cPacket("packet");
    pkt->setByteLength(par("packetByteSize"));

    // Send to the output
    send(pkt, "out");
}

void Generator::handleMessage(cMessage *msg) {
    // All calls are for sending a new packet
    sendPacket();

    // Update stats
    packetsSent++;
    packetsSentVector.record(packetsSent);

    // Compute the new departure time
    simtime_t departureTime = simTime() + par("generationInterval");
    // Schedule the new packet generation
    scheduleAt(departureTime, sendMsgEvent);
}

#endif /* GENERATOR */
