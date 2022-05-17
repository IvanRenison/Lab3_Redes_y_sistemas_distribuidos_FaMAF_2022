#ifndef TRANSPORT_RX
#define TRANSPORT_RX

#include <omnetpp.h>
#include <string.h>

#include "FeedbackPkt_m.h"

using namespace omnetpp;

class TransportRx : public cSimpleModule {
  private:
    // data
    cQueue buffer;

    // events
    cMessage *endServiceEvent;

    // functions    
    void sendPacket();
    void enqueueMessage(cMessage *msg);
  public:
    TransportRx();
    virtual ~TransportRx();

  protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(TransportRx);

TransportRx::TransportRx() {
    endServiceEvent = NULL;
}

TransportRx::~TransportRx() {
    cancelAndDelete(endServiceEvent);
}

void TransportRx::initialize() {
    buffer.setName("receptor status buffer");

    // Initialize events
    endServiceEvent = new cMessage("endService");
}

void TransportRx::finish() {
}

/* Send a packet from the queue */
void TransportRx::sendPacket() {
    // If there is a packet in buffer, send it
    if (!buffer.isEmpty()) {
        // Dequeue packet
        cPacket *pkt = (cPacket *)buffer.pop();

        // Send packet
        send(pkt, "toApp");

        // Start new service when the packet is sent
        simtime_t serviceTime = pkt->getDuration();
        scheduleAt(simTime() + serviceTime, endServiceEvent);
    }
}

/* Enqueue message if there is space in the buffer */
void TransportRx::enqueueMessage(cMessage *msg) {
    if (buffer.getLength() >= par("bufferSize").intValue()) {
        // Drop the packet
        delete msg;

        // Animate loss
        this->bubble("packet dropped");

        //// Update stats
        //packetsDropped++;
        //packetDropVector.record(packetsDropped);
    } else {
        // Enqueue the packet
        buffer.insert(msg);

        if (!endServiceEvent->isScheduled()) {
            // If there are no messages being sent, send this one now
            scheduleAt(simTime() + 0, endServiceEvent);
        }
    }
}

void TransportRx::handleMessage(cMessage *msg) {
    if (msg == endServiceEvent) {
    // If msg is signaling an endServiceEvent
        sendPacket();
    } else {
        // If msg is a incoming massage
        enqueueMessage(msg);
    }

    FeedbackPkt *fbkPkt = new FeedbackPkt();
    fbkPkt->setKind(2);
    send(fbkPkt, "toOut$o");
}

#endif /* TRANSPORT_RX */
