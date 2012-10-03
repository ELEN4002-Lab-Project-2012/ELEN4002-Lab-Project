#ifndef MECCLASSIFIER_H
#define MECCLASSIFIER_H

#include "SSVEPclassifier.h"
#include "FFT.h"
#include "Signal.h"
#include <vector>
#include <boost\shared_ptr.hpp>

class MECclassifier: public SSVEPclassifier
{
public:
    MECclassifier(int sampleSize, double sampleFreq, int numChannels, bool padding, Aquila::WindowType window);
    virtual double calculateRatio(double desiredFreq);
    virtual void updateEEGData(double*, double*, double*, double*, int);
    virtual void loadTestData();      // For testing purposes

private:
    mat findWeightedSignal(double fundamentalFreq, const double phaseShift);
    int sampleSize;
    double samplingFreq;
    double freqRes;
    int nChannels;
    int counter;
    vector<boost::shared_ptr<Signal>> channels;     // Stores original raw EEG
    FFT swFFT;        
    mat Y;                                          // Processed data stored in a matrix now


};

#endif