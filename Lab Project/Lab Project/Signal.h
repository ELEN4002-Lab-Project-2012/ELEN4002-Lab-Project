#ifndef SIGNAL_H
#define SIGNAL_H

#include <iostream>
#include "global.h"
#include "TextPlot.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib> 
#include <ctime>
using namespace std;
#define PI 3.1415926535897932384626433832795

/** Class representing the sampled signal
*/
class Signal
{
public:
    Signal(int size, int paddedSize, double rate);
    void updateSignal(double *buffer, int bufferSize);
    int getNumSamples() {return sampleSize;}
    double getSampleFreq() {return samplingFreq;}
    double* getEEGSignal() {return signal;}
    double* getAveEEGSignal() {return averagedSignal;}
    void eliminateDCOffset(double *x, int size);
    void averageSignal();
    void loadTestData();

    // For testing purposes - to view the output.
    void plotSignal();                              // "Plot" functions wrap Aquila plotting tools.
    void displaySignalValues();
    void printPaddedSignalCSV();
    void printSignalCSV();
    void displayAveragedSignal();

private:
    void zeroPadSignal();
    double getWindowAverage(double* x, int size);

    double* signal;
    double* paddedSignal;
    double* averagedSignal;
    int sampleSize;  
    int paddedSize;
    double samplingFreq;
    double freqRes;
    static bool ready;
    static int counter;
    Aquila::TextPlot plt;

};

#endif