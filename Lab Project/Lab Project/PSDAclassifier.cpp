#include "PSDAclassifier.h"

PSDAclassifier::PSDAclassifier(int size, double sampleFreq, int numChannels, bool padded, Aquila::WindowType window):
    sampleSize(size),
    samplingFreq(sampleFreq),
    freqRes(sampleFreq/size),
    counter(0),
    nChannels(numChannels)
{
    // Initialise the channels and FFTs
    for(int i = 0; i != nChannels; i++)
    {
        boost::shared_ptr<Signal> signal_ptr(new Signal(sampleSize, 2*sampleSize, samplingFreq));
        boost::shared_ptr<FFT> fft_ptr(new FFT(sampleSize, samplingFreq, padded, window));
        channels.push_back(signal_ptr);
        FFTs.push_back(fft_ptr);
    }
}

double PSDAclassifier::calculateRatio(double desiredFreq)
{
    double f1 = desiredFreq;    // Fundamental 
    vector<double> Ratios;
    for(int i = 0; i != nChannels; i++)
    {
        double integr = FFTs.at(i)->integral(f1 - 0.05*f1, f1 + 0.05*f1);
        Ratios.push_back(integr);
        FFTs.at(i)->zeroSpectrum();
    }
    // Return only the maximum ratio found (may be on any of the channels)
    return *max_element(Ratios.begin(),Ratios.end());
}

// DO NOT DELETE: OLD PSDA METHOD
/*double PSDAclassifier::calculateRatio(double desiredFreq)
{        
    double f1 = desiredFreq, f2 = 2*desiredFreq;    // Fundamental and second harmonic
    vector<double> Ratios;
    for(int i = 0; i != nChannels; i++)
    {
        // Find the maximum within a small range on either side f1 and f2.
        double max1 = FFTs.at(i)->getSpectrumMaxInRange(f1 - 0.1*f1, f1 + 0.1*f1);
        double max2;
        if(f2 > FFTs.at(i)->getMaxFreqInFFT())
            max2 = 0; 
        else
            max2 = FFTs.at(i)->getSpectrumMaxInRange(f2 - 0.1*f2, f2 + 0.1*f2);

        double ave = FFTs.at(i)->getSpectrumAverage();    
        Ratios.push_back((max1 + max2)/(2*ave));
    }
    // Return only the maximum ratio found (may be on any of the channels)
    return *max_element(Ratios.begin(),Ratios.end());
}*/

// Note that this function has to be called before checking for SSVEPs
void PSDAclassifier::updateEEGData(double* dataO1, double* dataO2, double* dataP7, double* dataP8, int nSamplesTaken)
{
    // Sorry, hard coded!
    channels.at(0)->updateSignal(dataO1, nSamplesTaken);
    channels.at(1)->updateSignal(dataO2, nSamplesTaken);
    channels.at(2)->updateSignal(dataP7, nSamplesTaken);
    channels.at(3)->updateSignal(dataP8, nSamplesTaken);

    for(int i = 0; i != nChannels; i++) {
        channels.at(i)->processSignal();
        FFTs.at(i)->calcFFT(channels.at(i), sampleSize);
    }
    //FFTs.at(0)->printSpectrumCSV();
}

// This function is only used for testing purposes. 
// Note that all channels have identical EEG signals...
void PSDAclassifier::loadTestData()
{
    for(int i = 0; i != nChannels; i++) {
        channels.at(i)->loadTestData();
        channels.at(i)->processSignal();
        FFTs.at(i)->calcFFT(channels.at(i), sampleSize);
    }
    FFTs.at(0)->printSpectrumCSV();
}