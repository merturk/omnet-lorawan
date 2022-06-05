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

#ifndef __LORAWAN_GW_H_
#define __LORAWAN_GW_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace lorawan {

/**
 * TODO - Generated class
 */
class GW : public cSimpleModule
{

private:
    simtime_t lastArrival;

    // state variables, event pointers
    //  bool channelBusy;
      bool sfChannelBusy[7];

      cMessage *endRxEvent[7];

      intval_t currentCollisionNumFrames;
      intval_t receiveCounter;
      simtime_t recvStartTime;
      enum { IDLE = 0, TRANSMISSION = 1, COLLISION = 2 };
      simsignal_t channelStateSignal[7];

      // statistics
      simsignal_t receiveBeginSignal;
      simsignal_t receiveSignal;
      simsignal_t collisionLengthSignal;
      simsignal_t collisionSignal;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

} //namespace

#endif
