//
// Created by Mehmet Ali Ertürk on 4.06.2022.
//

#ifndef TEMP_UTIL_H
#define TEMP_UTIL_H


#include <vector>
#include <math.h>
#include <algorithm>

class Util {

private:
    Util(){}
    double dutyCycle;
    std::vector<int> endDevSensLimit{-124, -127, -130, -133, -135, -137};
    std::vector<int> sfList{7, 8, 9, 10, 11, 12};


public:

    static Util& GetInstance()
    {
        static Util instance;
        return instance;
    }

    double getDutyCycle();

    double toa(int pktLen, int sf, int bw, int nPreamble, int h,
               int crc, int cr);

    double dataRate(int sf, int bw, int cr);

    double longDistancePathLoss(double d, double exp, int rdist, double rloss);

    double dcWait(int pktLen, int sf, int bw, int nPreamble, int h,
                  int crc, int cr);

    double distToSF(double d, double exp, int rdist,
                    double rloss);

    double dbSensToSf(int rxPow);



    virtual ~Util();
};

#endif //TEMP_UTIL_H
