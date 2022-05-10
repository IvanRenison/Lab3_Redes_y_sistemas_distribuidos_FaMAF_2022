#ifndef QUEUE
#define QUEUE

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Queue: public cSimpleModule {
private:
    // data
    cQueue buffer;
    simtime_t serviceTime;

    // events
    cMessage *endServiceEvent;

    // stats
    cOutVector bufferSizeVector;
    cStdDev bufferSizeStats;
    int packetsDropped;
    cOutVector packetDropVector;
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
    packetDropVector.setName("packet drop");
    bufferSizeVector.setName("buffer size");
    bufferSizeStats.setName("buffer stats");
    endServiceEvent = new cMessage("endService");
    packetsDropped = 0;
}

void Queue::finish() {
    recordScalar("Promedio de paquetes", bufferSizeStats.getMean());
}

void Queue::handleMessage(cMessage *msg) {
    // if msg is signaling an endServiceEvent
    if (msg == endServiceEvent) {
        // if packet in buffer, send next one
        if (!buffer.isEmpty()) {
            // dequeue packet
            cPacket *pkt = (cPacket*) buffer.pop();
            // send packet
            send(pkt, "out");
            // start new service
            serviceTime = pkt->getDuration();
            scheduleAt(simTime() + serviceTime, endServiceEvent);
        }
    } else if (buffer.getLength() >= par("bufferSize").intValue()) {
        //drop the packet 
        delete msg;
        this->bubble("packet dropped");
        packetsDropped++;
        packetDropVector.record(packetsDropped);
    } else {
        //enqueue the packet 
        bufferSizeVector.record(buffer.getLength());
        bufferSizeStats.collect(buffer.getLength());
        buffer.insert(msg);
        //if the server is idle
        if (!endServiceEvent->isScheduled()) {
            //start the service now 
            scheduleAt(simTime() + 0, endServiceEvent);
        }
    }
}

#endif /* QUEUE */
