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

#ifndef __LORAWAN_NODE_H_
#define __LORAWAN_NODE_H_

#include <omnetpp.h>
#include "Util.h"

using namespace omnetpp;

namespace lorawan {

/**
 * TODO - Generated class
 */
class Node: public cSimpleModule {

private:
    // parameters
    simtime_t radioDelay;
    double txRate;
    cPar *iaTime;
    cPar *pkLenBits;
    simtime_t slotTime;
    bool isSlotted;

    int bw;
    int sf = 12;
    int nPreambles;
    int headerEn;
    int crc;
    int codingRate;

    double ex;
    double rad;
    double plc;

    // state variables, event pointers etc
    cModule *gw;
    cMessage *endTxEvent = nullptr;
    enum {
        IDLE = 0, TRANSMIT = 1
    } state;
    simsignal_t stateSignal;
    int pkCounter;

    // position on the canvas, unit is m
    double x, y;

    // speed of light in m/s
    const double propagationSpeed = 299792458.0;

    cPacket *lastPacket = nullptr; // a copy of the last sent message, needed for animation
    mutable cRingFigure *transmissionRing = nullptr; // shows the last packet
    mutable std::vector<cOvalFigure*> transmissionCircles; // ripples inside the packet ring

public:
    virtual ~Node();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    simtime_t getNextTransmissionTime();

    Util &util = Util::GetInstance();
};

} //namespace

#endif
