#ifndef TRANSPORT_TX
#define TRANSPORT_TX

#include <omnetpp.h>
#include <string.h>

using namespace omnetpp;

class TransportTx : public cSimpleModule {
  private:
    // data
    cQueue buffer;

    // events
    cMessage *endServiceEvent;

    void sendPacket();
    void enqueueMessage(cMessage *msg);

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

    // Initialize events
    endServiceEvent = new cMessage("endService");
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

void TransportTx::handleMessage(cMessage *msg) {
    if (msg->getKind() == 2) {
        // msg is a FeedbackPkt
        // FeedbackPkt* feedbackPkt = (FeedbackPkt*)msg;

        // Do something with the feedback info
        delete msg;
    } else if (msg->getKind() == 0) {
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
