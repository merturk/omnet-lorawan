//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Node.h"

namespace lorawan {

Define_Module(Node);

void Node::initialize() {

    stateSignal = registerSignal("state");
    gw = getModuleByPath("gw");

    iaTime = &par("iaTime");
    pkLenBits = &par("pkLenBits");
    bw = par("bw");

    nPreambles = par("nPreambles");
    headerEn = par("headerEn");
    crc = par("crc");
    codingRate = par("codingRate");

    ex = par("ex");
    rad = par("rad");
    plc = par("plc");

    isSlotted = false;
    WATCH(iaTime);

    endTxEvent = new cMessage("send/endTx");
    state = IDLE;
    emit(stateSignal, state);
    pkCounter = 0;
    WATCH((int& )state);
    WATCH(pkCounter);

    x = par("x").doubleValue();
    y = par("y").doubleValue();

    double gwX = gw->par("x").doubleValue();
    double gwY = gw->par("y").doubleValue();

    double dist = std::sqrt((x - gwX) * (x - gwX) + (y - gwY) * (y - gwY));
    radioDelay = dist / propagationSpeed;

    sf = util.distToSF(dist, ex, rad, plc);

    if (sf == 0)
        sf = 12;

    EV << "Setting sf value: " << sf << "\n";

    double dataRate = util.dataRate(sf, bw, crc);


    txRate = dataRate;

    getDisplayString().setTagArg("p", 0, x);
    getDisplayString().setTagArg("p", 1, y);

    scheduleAt(getNextTransmissionTime(), endTxEvent);

}

void Node::handleMessage(cMessage *msg) {
    ASSERT(msg == endTxEvent);

    if (state == IDLE) {
        // generate packet and schedule timer when it ends
        char pkname[40];
        sprintf(pkname, "pk-%d-#%d", getId(), pkCounter++);
        EV << "generating packet " << pkname << endl;

        state = TRANSMIT;
        emit(stateSignal, state);

        cPacket *pk = new cPacket(pkname);
        pk->setBitLength(pkLenBits->intValue());

        if (sf < 7 || sf > 12)
            sf = 12;

        double toa = util.toa(pkLenBits->intValue() / 8, sf, bw, nPreambles, headerEn, crc, codingRate);
        simtime_t duration = SimTime(toa * 1000, SIMTIME_MS);

        //sendDirect(pk, radioDelay, duration, gw->gate("in"));

        EV << "Before senddirect sf: "<< sf << "duration: "<< duration <<"\n";
        sendDirect(pk, radioDelay, duration, gw->gate("into", sf-7));

        scheduleAt(simTime() + duration, endTxEvent);


        // let visualization code know about the new packet
        if (transmissionRing != nullptr) {
            delete lastPacket;
            lastPacket = pk->dup();
        }
    } else if (state == TRANSMIT) {
        // endTxEvent indicates end of transmission
        state = IDLE;
        emit(stateSignal, state);

        // schedule next sending
        scheduleAt(getNextTransmissionTime(), endTxEvent);
    } else {
        throw cRuntimeError("invalid state");
    }
}

simtime_t Node::getNextTransmissionTime() {

    simtime_t t = simTime();

    if (pkCounter != 0) {
        double dc = util.dcWait(pkLenBits->intValue() / 8, sf, bw, nPreambles, headerEn, crc, codingRate);

        t = t + SimTime(dc, SIMTIME_S); // DC
    } else
        t = t + iaTime->doubleValue();

    return t;

}

Node::~Node() {

    delete lastPacket;
    cancelAndDelete(endTxEvent);
}

void Node::finish() {
    EV << "scheduledPacket: " << pkCounter << endl;

    recordScalar("pkCounter", pkCounter);
}

} //namespace
