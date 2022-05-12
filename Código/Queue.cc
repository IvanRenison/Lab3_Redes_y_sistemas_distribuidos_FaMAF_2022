#ifndef QUEUE
#define QUEUE

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Queue: public cSimpleModule {
private:
    // data
    cQueue buffer;

    // events
    cMessage *endServiceEvent;

    // stats
    cOutVector bufferSizeVector;
    cStdDev bufferSizeStats;
    int packetsDropped;
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

    // Initialite events
    endServiceEvent = new cMessage("endService");

    // Initialite stats
    bufferSizeVector.setName("buffer size");
    bufferSizeStats.setName("buffer stats");
    packetsDropped = 0;
    packetDropVector.setName("packet drop");
}

void Queue::finish() {
    recordScalar("Promedio de paquetes", bufferSizeStats.getMean());
}

/* Send a packet from de queue */
void Queue::sendPacket() {
    // If there is a packet in buffer, send it
    if (!buffer.isEmpty()) {
        // Dequeue packet
        cPacket *pkt = (cPacket*) buffer.pop();

        // Send packet
        send(pkt, "out");

        // Start new service when the packet is sent
        simtime_t serviceTime = pkt->getDuration();
        scheduleAt(simTime() + serviceTime, endServiceEvent);
    }
}

/* Enqueue message if there is space in the buffer */
void Queue::enqueueMessage(cMessage *msg) {
    if (buffer.getLength() >= par("bufferSize").intValue()) {
        // Drop the packet 
        delete msg;

        // Animate lost
        this->bubble("packet dropped");

        // Update stats
        packetsDropped++;
        packetDropVector.record(packetsDropped);
    }
    else {
        // Enqueue the packet 
        buffer.insert(msg);
        
        if (!endServiceEvent->isScheduled()) {
            // If there are no messages being send, send these one now
            scheduleAt(simTime() + 0, endServiceEvent);
        }
    }
}

void Queue::handleMessage(cMessage *msg) {
    // Record stats
    bufferSizeVector.record(buffer.getLength());
    bufferSizeStats.collect(buffer.getLength());

    if (msg == endServiceEvent) {
        // If msg is signaling an endServiceEvent
        sendPacket();
    }
    else {
        // If msg is a incoming massage
        enqueueMessage(msg);
    }

    // Record stats
    bufferSizeVector.record(buffer.getLength());
    bufferSizeStats.collect(buffer.getLength());
}

#endif /* QUEUE */
