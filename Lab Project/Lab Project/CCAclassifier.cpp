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
	// Generate matrix of ideal signals ONCE!
	double Fseek = 11;
	double f1 = Fseek, f2 = 2*Fseek, f3 = 3*Fseek;        // Fundamental, second harmonic and Third harmonic
    double dt = 1/samplingFreq;

    mat t(sampleSize, 1);                       // Time scale
    for (int i = 0; i != sampleSize; ++i)
        t(i, 0) = dt*i;
	/*
	% Pure harmonics:
    y1 = cos(2*pi*Fc_Seek*t);     % First harmonic
    y2 = sin(2*pi*Fc_Seek*t);     % First harmonic
    y3 = cos(4*pi*Fc_Seek*t);     % First harmonic
    y4 = sin(4*pi*Fc_Seek*t);     % First harmonic
    y5 = cos(6*pi*Fc_Seek*t);     % First harmonic
    y6 = sin(6*pi*Fc_Seek*t);     % First harmonic

    Y = [y1; y2; y3; y4;y5;y6 ]; 
	*/
	mat y1 = cos(2*PI*f1*t );
	mat y2 = sin(2*PI*f1*t );
	mat y3 = cos(2*PI*f2*t );
	mat y4 = sin(2*PI*f2*t );
	mat y5 = cos(2*PI*f3*t );
	mat y6 = sin(2*PI*f3*t );

	Y = join_cols(y1,y2);
	Y = join_cols(Y,y3);
	Y = join_cols(Y,y4);
	Y = join_cols(Y,y5);
	Y = join_cols(Y,y6);

   // mat x2 = join_rows(sin(2*PI*f2*t + phaseShift), cos(2*PI*f2*t + phaseShift));
  //  mat X  = join_rows(x1, x2);

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

   /* for(int i = 0; i != sampleSize; i++) 
   {                      // Fill the Y matrix with data.
        for(int j = 0; j != nChannels; j++) 
		{
            Y(i, j) = channels.at(j)->getAveEEGSignal()[i];
        }
    }
	*/
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