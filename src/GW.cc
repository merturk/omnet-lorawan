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

#include "GW.h"

namespace lorawan {

Define_Module(GW);

void GW::initialize() {


    //endRxEvent = new cMessage("end-reception");

    for (int i = 0; i < 7; i++) {
        channelStateSignal[i] = registerSignal("channelState");

        endRxEvent[i] = new cMessage("end-reception");
        sfChannelBusy[i] = false;
        gate("into", i)->setDeliverImmediately(true);

        emit(channelStateSignal[i], IDLE);
    }



    currentCollisionNumFrames = 0;
    receiveCounter = 0;
    WATCH(currentCollisionNumFrames);

    receiveBeginSignal = registerSignal("receiveBegin");
    receiveSignal = registerSignal("receive");
    collisionSignal = registerSignal("collision");
    collisionLengthSignal = registerSignal("collisionLength");

    emit(receiveSignal, 0L);
    emit(receiveBeginSignal, 0L);

    getDisplayString().setTagArg("p", 0, par("x").doubleValue());
    getDisplayString().setTagArg("p", 1, par("y").doubleValue());
}

void GW::handleMessage(cMessage *msg) {

    if (msg->isSelfMessage()) {

        for (int i = 0; i < 7; i++) {

            if (msg == endRxEvent[i]) {
                EV << "GW reception finished\n";
                sfChannelBusy[i] = false;
                emit(channelStateSignal[i], IDLE);

                // update statistics
                simtime_t dt = simTime() - recvStartTime;
                if (currentCollisionNumFrames == 0) {
                    // start of reception at recvStartTime
                    cTimestampedValue tmp(recvStartTime, (intval_t) 1);
                    emit(receiveSignal, &tmp);
                    // end of reception now
                    emit(receiveSignal, 0);
                } else {
                    // start of collision at recvStartTime
                    cTimestampedValue tmp(recvStartTime,
                            currentCollisionNumFrames);
                    emit(collisionSignal, &tmp);

                    emit(collisionLengthSignal, dt);
                }

                currentCollisionNumFrames = 0;
                receiveCounter = 0;
                emit(receiveBeginSignal, receiveCounter);
            }

        }

    } else {
        int gateIndx = msg->getArrivalGate()->getIndex();

        cPacket *pkt = check_and_cast<cPacket*>(msg);

        ASSERT(pkt->isReceptionStart());
        simtime_t endReceptionTime = simTime() + pkt->getDuration();


        emit(receiveBeginSignal, ++receiveCounter);

        if (!sfChannelBusy[gateIndx]) {
            EV << "started receiving\n";
            recvStartTime = simTime();
            sfChannelBusy[gateIndx] = true;
            emit(channelStateSignal[gateIndx], TRANSMISSION);
            scheduleAt(endReceptionTime, endRxEvent[gateIndx]);

        } else {
            EV << "another frame arrived while receiving -- collision!\n";
            emit(channelStateSignal[gateIndx], COLLISION);

            if (currentCollisionNumFrames == 0)
                currentCollisionNumFrames = 2;
            else
                currentCollisionNumFrames++;

            if (endReceptionTime > endRxEvent[gateIndx]->getArrivalTime()) {
                cancelEvent(endRxEvent[gateIndx]);
                scheduleAt(endReceptionTime, endRxEvent[gateIndx]);

            }
        }
        sfChannelBusy[gateIndx] = true;
        delete pkt;

    }

}

void GW::finish() {
    EV << "duration: " << simTime() << endl;

    recordScalar("duration", simTime());

}

} //namespace
