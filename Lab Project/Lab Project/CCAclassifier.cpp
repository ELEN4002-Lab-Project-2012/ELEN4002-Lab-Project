#include "CCAclassifier.h"

CCAclassifier::CCAclassifier(int size, double sampleFreq, int numChannels):
    sampleSize(size),
    samplingFreq(sampleFreq),
    freqRes(sampleFreq/size),
    nChannels(numChannels)
{
    // Initialise the channels
    for(int i = 0; i != nChannels; i++)
    {
        boost::shared_ptr<Signal> signal_ptr(new Signal(sampleSize, 2*sampleSize, samplingFreq));
        channels.push_back(signal_ptr);
    }
    cout << "CCA classifier instantiated" << endl;
}

double CCAclassifier::calculateRatio(double desiredFreq)
{
    return 0;
}

void CCAclassifier::updateEEGData(double* dataO1, double* dataO2, double* dataP7, double* dataP8, int nSamplesTaken)
{
    // Sorry, hard coded!
    channels.at(0)->updateSignal(dataO1, nSamplesTaken);
    channels.at(1)->updateSignal(dataO2, nSamplesTaken);
    channels.at(2)->updateSignal(dataP7, nSamplesTaken);
    channels.at(3)->updateSignal(dataP8, nSamplesTaken);

    for(int i = 0; i != nChannels; i++) {
        channels.at(i)->processSignal();                        // Average the signal for each channel (remove DC offset)
    }

    for(int i = 0; i != sampleSize; i++) {                      // Fill the Y matrix with data.
        for(int j = 0; j != nChannels; j++) {
            Y(i, j) = channels.at(j)->getAveEEGSignal()[i];
        }
    }
}

void CCAclassifier::loadTestData()
{
    // Initialise signal data
    for(int i = 0; i != nChannels; i++) {
        channels.at(i)->loadTestData();
    }
    // Initialise Y matrix
    for(int i = 0; i != sampleSize; i++) {       // Fill the Y matrix with data.
        for(int j = 0; j != nChannels; j++) {
            Y(i, j) = channels.at(j)->getEEGSignal()[i];
        }
    }
}