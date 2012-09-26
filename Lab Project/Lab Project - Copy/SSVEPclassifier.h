#ifndef SSVEPCLASSIFIER_H
#define SSVEPCLASSIFIER_H

#include "Signal.h"
#include <armadillo>

using namespace arma;
using namespace std;

/**
    SSVEP classifier interface. 
*/
class SSVEPclassifier
{
public:
    virtual bool isSSVEPpresent(double desiredFreq) = 0;
    virtual void updateEEGData(double*, double*, double*, double*, int) = 0;
    virtual void loadTestData() = 0;

private:

};

#endif