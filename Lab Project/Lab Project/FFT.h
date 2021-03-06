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
#include "Window.h"
using namespace std;
using namespace Aquila;

class FFT
{
public:
    FFT(int size, double rate, bool pad, Aquila::WindowType type);
    void calcFFT(boost::shared_ptr<Signal> signal, int size);
    void calcFFT(double*, int size); 
    void calcAbsAveSpectrum();
    void zeroSpectrum();
    double getSpectrumMaxInRange(int nLower, int nUpper);
    double getSpectrumMaxInRange(double f1, double f2); 
    double getSpectrumAverage();
    int getAbsFFTsize() {return absFFTsize;}
    double getMaxFreqInFFT() {return freqRes*absFFTsize;};
    double integral(double f1, double f2);

    // For testing purposes - to view the output.
    void plotFFT();
    void displaySpectrumValues();
    void printSpectrumCSV();
	void printSpectrumCSV(string fileName );
    void displayAverageSpectrum();

private:
    double* applyWindow(double* signal, int size);
    void initFFT(int complexSize, double freq);
    void initAbsoluteSpectrum();
    double* zeroPad(double* signal, int sampleSize);
    int absFFTsize;
    int complexFFTsize;
    double samplingFreq;
    double freqRes;
    vector<boost::shared_array<double>> FFTbuffer;
    bool padded;
    double* normalisedSpectrum;

    boost::shared_ptr<Aquila::OouraFft> fft_ptr;
    Aquila::ComplexType* complexSpectrum;
    boost::shared_array<double> absSpectrum;
    boost::shared_array<double> aveAbsSpectrum;
    Aquila::TextPlot plt;
    Aquila::WindowType windowType;
};

#endif