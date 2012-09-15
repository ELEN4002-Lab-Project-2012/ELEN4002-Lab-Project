#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <vector>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include <fstream>
#include "global.h"
#include "OouraFft.h"
#include "TextPlot.h"
#include "Signal.h"
#include <cmath>
#include <iostream>
using namespace std;


class FFT
{
public:
    FFT(int size, double rate);
    void calcFFT(boost::shared_ptr<Signal> signal);
    void calcFFT(double*); 
    void calcAbsAveSpectrum();
    void zeroSpectrum();
    double getSpectrumMaxInRange(int nLower, int nUpper);
    double getSpectrumMaxInRange(double f1, double f2); 
    double getSpectrumAverage();
    int getAbsFFTsize() {return absFFTsize;}
    double getMaxFreqInFFT() {return freqRes*absFFTsize;};

    // For testing purposes - to view the output.
    void plotFFT();
    void displaySpectrumValues();
    void printSpectrumCSV();
    void displayAverageSpectrum();

private:
    void initAbsoluteSpectrum();

    int absFFTsize;
    int complexFFTsize;
    double samplingFreq;
    double freqRes;
    vector<boost::shared_array<double>> FFTbuffer;

    Aquila::OouraFft fft;
    Aquila::ComplexType* complexSpectrum;
    boost::shared_array<double> absSpectrum;
    boost::shared_array<double> aveAbsSpectrum;
    Aquila::TextPlot plt;
};

#endif