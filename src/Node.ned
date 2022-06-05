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

package lorawan;

//
// TODO auto-generated module
//
simple Node
{
    parameters:
        @display("i=device/pocketpc");
        double sendInterval @unit(s) = default(1s);
        
        @signal[state](type="long");
        @statistic[radioState](source="state";title="Radio state";enum="IDLE=0,TRANSMIT=1";record=vector);
        volatile int pkLenBits @unit(b);   // packet length in bits
        volatile double iaTime @unit(s);   // packet interarrival time
        double x @unit(m);                 // the x coordinate of the host
        double y @unit(m);                 // the y coordinate of the host
        
        
        int bw;
        int nPreambles;
        int headerEn;
        int crc;
        int codingRate;
        
        double ex;
        double rad;
        double plc;
   
   
}
