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
    virtual bool isSSVEPpresent(double desiredFreq);
    virtual void updateEEGData(double*, double*, double*, double*, int);
    virtual void loadTestData();      // For testing purposes
    vector<double> getRValues() {return Rvalues;}

private:
    bool detectTargetFreq(double desiredFreq);
    mat findWeightedSignal(double fundamentalFreq, const double phaseShift);
    int sampleSize;
    double samplingFreq;
    double freqRes;
    int nChannels;
    int counter;
    vector<boost::shared_ptr<Signal>> channels;
    Aquila::WindowType windowFunction;
    FFT swFFT;        // FFT of the weighted signal
    mat Y;
    vector<double> Rvalues;

};

// Standalone function for testing purposes
/*
void printCSV(mat x, int size, double res)
{
    std::ofstream myfile;
    myfile.open("test2.csv");
    myfile << "Frequency (Hz),Magnitude" << endl;
    for (int i=0; i!=size; i++)
    {
        myfile << i*res << "," << x(i, 0) << endl; 
        //cout << i*res << "," << x(i, 0) << endl;
    }
    myfile.close();
}*/

#endif