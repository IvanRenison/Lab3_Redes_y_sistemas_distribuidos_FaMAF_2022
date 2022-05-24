#ifndef QUEUE
#define QUEUE

#include <omnetpp.h>
#include <string.h>

#include "FeedbackPkt_m.h"

using namespace omnetpp;

class Queue : public cSimpleModule {
  private:
    // data
    cQueue buffer;

    // events
    cMessage *endServiceEvent;

    // stats
    int packetsDropped;
    cOutVector bufferSizeVector;
    cOutVector packetDropVector;

    // functions
    void sendPacket();
    void enqueueMessage(cMessage *msg);

  public:
    Queue();
    virtual ~Queue();

  protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(Queue);

Queue::Queue() {
    endServiceEvent = NULL;
}

Queue::~Queue() {
    cancelAndDelete(endServiceEvent);
}

void Queue::initialize() {
    buffer.setName("buffer");

    // Initialize events
    endServiceEvent = new cMessage("endService");

    // Initialize stats
    packetsDropped = 0;
    bufferSizeVector.setName("buffer size");
    packetDropVector.setName("packet drop");
}

void Queue::finish() {
    recordScalar("Dropped packets", packetsDropped);
}

/* Send a packet from the queue */
void Queue::sendPacket() {
    // If there is a packet in buffer, send it
    if (!buffer.isEmpty()) {
        // Dequeue packet
        cPacket *pkt = (cPacket *)buffer.pop();

        // Send packet
        send(pkt, "out");

        // Start new service when the packet is sent
        simtime_t serviceTime = pkt->getDuration();
        scheduleAt(simTime() + serviceTime, endServiceEvent);
    }
}

/* Enqueue message if there is space in the buffer */
void Queue::enqueueMessage(cMessage *msg) {
    const int bufferMaxSize = par("bufferSize").intValue();
    const int umbral = 0.80 * bufferMaxSize;

    if (buffer.getLength() >= bufferMaxSize) {
        // Drop the packet
        delete msg;

        // Animate loss
        this->bubble("packet dropped");

        // Update stats
        packetsDropped++;
        packetDropVector.record(packetsDropped);
    } else {
        if (buffer.getLength() >= umbral) {
            FeedbackPkt *fbkPkt = new FeedbackPkt();
            fbkPkt->setKind(2);
            fbkPkt->setByteLength(1);
            fbkPkt->setFullBufferQueue(true);
            buffer.insertBefore(buffer.front(), fbkPkt);
        }
        // Enqueue the packet
        buffer.insert(msg);

        if (!endServiceEvent->isScheduled()) {
            // If there are no messages being sent, send this one now
            scheduleAt(simTime() + 0, endServiceEvent);
        }
    }
}

void Queue::handleMessage(cMessage *msg) {
    // Record stats
    bufferSizeVector.record(buffer.getLength());

    if (msg == endServiceEvent) {
        // If msg is signaling an endServiceEvent
        sendPacket();
    } else {
        // If msg is a incoming massage
        enqueueMessage(msg);
    }

}

#endif /* QUEUE */
