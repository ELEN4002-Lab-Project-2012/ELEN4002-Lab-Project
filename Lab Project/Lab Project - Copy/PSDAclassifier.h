#ifndef PSDACLASSIFIER_H
#define PSDACLASSIFIER_H

#include "SSVEPclassifier.h"
#include "FFT.h"
#include "Signal.h"
#include <vector>
#include <boost\shared_ptr.hpp>

class PSDAclassifier: public SSVEPclassifier
{
public:
    PSDAclassifier(int sampleSize, double sampleFreq, int numChannels, bool padded, Aquila::WindowType window);
    virtual bool isSSVEPpresent(double desiredFreq);
    virtual void updateEEGData(double*, double*, double*, double*, int);
    virtual void loadTestData();

    bool detectTargetFreq(boost::shared_ptr<FFT> fft, double desiredFreq);

private:
    int sampleSize;
    double samplingFreq;
    double freqRes;
    int nChannels;
    int counter;
    vector<boost::shared_ptr<Signal>> channels;
    vector<boost::shared_ptr<FFT>> FFTs;
};

#endif