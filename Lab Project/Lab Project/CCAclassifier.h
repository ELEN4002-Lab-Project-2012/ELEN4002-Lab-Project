#ifndef CCACLASSIFIER_H
#define CCACLASSIFIER_H

#include "SSVEPclassifier.h"
#include "Signal.h"
#include "FFT.h"
#include <vector>
#include <boost\shared_ptr.hpp>

class CCAclassifier: public SSVEPclassifier
{
public:
    CCAclassifier(int sampleSize, double sampleFreq, int numChannels);
    virtual double calculateRatio(double desiredFreq);
    virtual void updateEEGData(double*, double*, double*, double*, int);
    virtual void loadTestData();                    // For testing purposes
	
private:
    int sampleSize;
    double samplingFreq;
    double freqRes;
    int nChannels;
    vector<boost::shared_ptr<Signal>> channels;
    mat Y;		// Y is the matrix of Ideal signals with 6 harmoics
	mat X;		// X is the Matric of sampeled data
};

#endif