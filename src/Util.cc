//
// Created by Mehmet Ali Ertürk on 4.06.2022.
//

#include "Util.h"
#include <iostream>






double Util::getDutyCycle() {
    this->dutyCycle = 1.0 / 100.0;
    return this->dutyCycle;
}

double Util::toa(int pktLen, int sf, int bw, int nPreamble, int h,
                 int crc, int cr) {

    int de = 0;
    if (sf == 11 || sf == 12)
        de = 1;

    double tSym = pow(2, sf) / bw;

    double tPreamble = (float(nPreamble) + 4.25) * tSym;

    double num = 8 * pktLen - 4 * sf + 28 + 16 * crc - 20 * h;

    double den = 4 * (sf - 2 * de);

    double payloadSymbNb = 8 + std::max(ceil(num / den) * (cr + 4), 0.0);

    double tPayload = payloadSymbNb * tSym;

    double tTotal = tPreamble + tPayload;

    return tTotal;
}

double Util::dataRate(int sf, int bw, int cr) {

    return sf * ((float) bw / pow(2, sf)) * (4.0 / (4.0 + cr));

}

double Util::longDistancePathLoss(double d, double exp, int rdist,
                                  double rloss) {


    double txPower = 14;  // 25mW 14dBm
    if (d == 0.0) {
        d = 1.0;
    }

    double pathLossDb = 10.0 * exp * log10(d / rdist);
    double rx = -rloss - pathLossDb;

    return txPower + rx;
}

double Util::dbSensToSf(int rxPow) {

    auto smin = *std::min_element(this->endDevSensLimit.begin(), this->endDevSensLimit.end());
    std::cout<<"snf " << smin  <<"\n";
    if (rxPow <  smin)
        return 0;

    int idx = 0;
    for(auto &i : this->endDevSensLimit){
        if(rxPow > i)
            return this->sfList[idx];
        idx++;
    }

    return this->sfList[idx];
}

double Util::dcWait(int pktLen, int sf, int bw, int nPreamble, int h,
                    int crc, int cr) {
    double toa = this->toa(pktLen, sf, bw, nPreamble, h, crc, cr);

    return ceil(((float)toa / this->getDutyCycle()) - (float)toa);
}

double Util::distToSF(double d, double exp, int rdist,
                      double rloss) {
        return Util::dbSensToSf(Util::longDistancePathLoss(d, exp, rdist, rloss));
}

Util::~Util() {

}

