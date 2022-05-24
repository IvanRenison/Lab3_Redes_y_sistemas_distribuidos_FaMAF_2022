#ifndef TRANSPORT_TX
#define TRANSPORT_TX

#include <omnetpp.h>
#include <string.h>

#include "FeedbackPkt_m.h"

using namespace omnetpp;

const double CONTROL_TIMEOUT = 0.1;
const double CONTROL_REGAIN_TIME = 0.2;

class TransportTx : public cSimpleModule {
  private:
    // data
    cQueue buffer;

    // events
    cMessage *endServiceEvent;

    // temporizers
    double lastCong;

    // scalars
    double contScalar;

    // stats
    cOutVector bufferSizeVector;

    // functions
    void sendPacket();
    void enqueueMessage(cMessage *msg);
    void handleCongestion();
    void handleFlow();
    void controlSendRate();

  public:
    TransportTx();
    virtual ~TransportTx();

  protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(TransportTx);

TransportTx::TransportTx() {
    endServiceEvent = NULL;
}

TransportTx::~TransportTx() {
    cancelAndDelete(endServiceEvent);
}

void TransportTx::initialize() {
    buffer.setName("transmisor status buffer");

    // Initialize timers
    lastCong = 0;

    // Initialize scalars
    contScalar = 0.0;

    // Initialize events
    endServiceEvent = new cMessage("endService");

    // Initialize stats
    bufferSizeVector.setName("buffer size");
}

void TransportTx::finish() {
}

/* Send a packet from the queue */
void TransportTx::sendPacket() {
    // If there is a packet in buffer, send it
    if (!buffer.isEmpty()) {
        // Dequeue packet
        cPacket *pkt = (cPacket *)buffer.pop();

        // Send packet
        send(pkt, "toOut$o");

        // Start new service when the packet is sent
        simtime_t serviceTime = pkt->getDuration();
        serviceTime = (serviceTime + (serviceTime * contScalar));
        scheduleAt(simTime() + serviceTime, endServiceEvent);
    }
}

void TransportTx::enqueueMessage(cMessage *msg) {
    if (buffer.getLength() >= par("bufferSize").intValue()) {
        // Drop the packet
        delete msg;

        // Animate loss
        this->bubble("packet dropped");
    } else {
        // Enqueue the packet
        buffer.insert(msg);

        if (!endServiceEvent->isScheduled()) {
            // If there are no messages being sent, send this one now
            scheduleAt(simTime() + 0, endServiceEvent);
        }
    }
}

void TransportTx::handleCongestion() {
    if (simTime().dbl() - lastCong >= CONTROL_TIMEOUT) {
        contScalar += 0.1;
        lastCong = simTime().dbl();
    }
}

void TransportTx::controlSendRate() {
    if (simTime().dbl() - lastCong >= CONTROL_REGAIN_TIME && contScalar > 0.1) {
        contScalar -= 0.1;
    }
}

void TransportTx::handleMessage(cMessage *msg) {
    // Record stats
    bufferSizeVector.record(buffer.getLength());

    controlSendRate();

    if (msg->getKind() == 2) {
        //msg is a FeedbackPkt
        FeedbackPkt *feedbackPkt = (FeedbackPkt *)msg;

        if (feedbackPkt->getFullBufferQueue() || feedbackPkt->getFullBufferR()) {
            handleCongestion();
        }

        delete msg;
    } else {
        // msg is a data packet
        if (msg == endServiceEvent) {
            // If msg is signaling an endServiceEvent
            sendPacket();
        } else {
            // If msg is a incoming massage
            enqueueMessage(msg);
        }
    }

}

#endif /* TRANSPORT_TX */
