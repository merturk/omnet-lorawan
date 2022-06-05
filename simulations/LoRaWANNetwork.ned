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

package lorawan.simulations;

import lorawan.GW;
import lorawan.Node;


network LoRaWANNetwork
{
    parameters:
        int numNodes;  // number of hosts
        @display("bgi=background/terrain,s;bgb=10000,10000");
    submodules:
        gw: GW;
        nodes[numNodes]: Node {
            
        }
}
