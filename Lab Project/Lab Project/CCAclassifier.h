#ifndef CCACLASSIFIER_H
#define CCACLASSIFIER_H

#include "SSVEPclassifier.h"
#include "Signal.h"
#include <vector>
#include <boost\shared_ptr.hpp>

class CCAclassifier: public SSVEPclassifier
{
public:
    CCAclassifier(int sampleSize, double sampleFreq, int numChannels, Aquila::WindowType window);
    virtual bool isSSVEPpresent(double desiredFreq);
    virtual void updateEEGData(double*, double*, double*, double*, int);
    virtual void loadTestData();                    // For testing purposes

private:
    bool detectTargetFreq(double desiredFreq);

    int sampleSize;
    double samplingFreq;
    double freqRes;
    int nChannels;
    vector<boost::shared_ptr<Signal>> channels;
    mat Y;                                          // Y is the raw EEG data matrix
};

#endif