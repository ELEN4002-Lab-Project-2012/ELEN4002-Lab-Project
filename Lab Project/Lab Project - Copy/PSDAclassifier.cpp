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
        boost::shared_ptr<Signal> signal_ptr(new Signal(sampleSize, 2*sampleSize, samplingFreq, window));
        boost::shared_ptr<FFT> fft_ptr(new FFT(sampleSize, samplingFreq, padded));
        channels.push_back(signal_ptr);
        FFTs.push_back(fft_ptr);
    }
    cout << "PSDA classifier instantiated" << endl;
}

bool PSDAclassifier::isSSVEPpresent(double desiredFreq)
{    
    const int minCount = 3, maxCount = 5;
    bool found = false;

    // If the SSVEP frequency is detected on any channel, increment the counter
    for(int i = 0; i != nChannels; i++)
    {
        if(detectTargetFreq(FFTs.at(i), desiredFreq) == true){
            if(counter < maxCount)      // The counter can reach a maximum of maxCount. 
                counter++;
            found = true;
            cout << "target frequency found" << endl;
            break;                      // One positive detection implies a positive result
        }
        // The implicit else statement keeps "found" as false
        FFTs.at(i)->zeroSpectrum();     // NB: Reset the spectrum - a requirement of Acquilla
    }

    // This statement is placed outside the loop to ensure that a negative detection reduces count by only one
    if(found == false && counter > 0){    // Counter can reach a minimum of 0.
        counter--;
        cout << "target frequency found" << endl;
    }

    cout << "PSDA counter after frequency detection = " << counter << endl;
    
    // counter value of "minCount" indicates a successful detection.
    // "Hysteresis" of "minCount - minCount + 1". That is, if the count reaches 5, it requires 
    // 3 non-successful detections to return an overall negative SSVEP detection. 
    if(counter >= minCount && counter <= maxCount)      
        return true;
    else
        return false;
}

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
        FFTs.at(i)->calcFFT(channels.at(i), sampleSize);
    }
    FFTs.at(0)->printSpectrumCSV();
}

bool PSDAclassifier::detectTargetFreq(boost::shared_ptr<FFT> fft, double desiredFreq)
{
    double f1 = desiredFreq, f2 = 2*desiredFreq;        // Fundamental and second harmonic

    // Find the maximum within a small range on either side f1 and f2.
    double max1 = fft->getSpectrumMaxInRange(f1 - 0.1*f1, f1 + 0.1*f1);
    double max2;
    if(f2 > fft->getMaxFreqInFFT())
        max2 = 0; 
    else
        max2 = fft->getSpectrumMaxInRange(f2 - 0.1*f2, f2 + 0.1*f2);

    //cout << "Max1 = " << max1 << endl;
    //cout << "Max2 = " << max2 << endl;

    double ave = fft->getSpectrumAverage();
    if( (max1 + max2) > 2*ave )
        return true;  
    else  
        return false;
}