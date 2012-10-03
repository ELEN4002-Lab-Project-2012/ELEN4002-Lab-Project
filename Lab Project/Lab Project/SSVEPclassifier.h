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
	// Returns R for Classification 
    virtual double calculateRatio(double desiredFreq) = 0;
	// Updates EEG data from the EMOTIV and updates data otred int he algorythm
    virtual void updateEEGData(double*, double*, double*, double*, int) = 0;
	// Populate with test data VS real data
    virtual void loadTestData() = 0;

private:

};

#endif