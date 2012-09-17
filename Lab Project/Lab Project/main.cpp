#include "EmoController.h"
#include "PSDAclassifier.h"
#include "MECclassifier.h"
#include <iostream>
#include <boost\shared_ptr.hpp>
using namespace std;

int main()
{
    int sampleSize = 512;
    const int paddedSize = sampleSize + 256;     // Note that for now, I'm not using padding at all. TO DO: include padding
    const unsigned int sampleFreq = 128;
    double SSVEPfreq = 15;
    int numChannels = 4;
 
    // PSDAclassifier, MECclassifier or CCAclassifier
    boost::shared_ptr<MECclassifier> myClassifier(new MECclassifier(sampleSize, sampleFreq, numChannels));    

    EmoController controller(myClassifier, numChannels);        
    controller.loop(SSVEPfreq, 0);                          // Starting the loop with option '1' means use test data.                             
    controller.disconnectEmoEngine();

	system("PAUSE");

    return 0;
}

/*  ORIGINAL AQUILA CODE
    // input signal parameters
    const std::size_t SIZE = 64;
    const Aquila::FrequencyType sampleFreq = 2000;
    const double dt = 1.0/sampleFreq;
    const Aquila::FrequencyType f1 = 125, f2 = 700;

    double* x = new double[SIZE];
    for (std::size_t i = 0; i < SIZE; ++i)
    {
        x[i] = 32*std::sin(2*M_PI*f1*i*dt) + 8*std::sin(2*M_PI*f2*i*dt + 0.75*M_PI);
    }

    const int paddedSize = SIZE + 64;

    printCSV(x, SIZE);
    double* paddedSample = zeroPad(x, SIZE, paddedSize);
    cout << endl << "After:" << endl;
    for(int i = 0; i != paddedSize; i++)
    {
        cout << paddedSample[i] << " ";
    }
    //printCSV(paddedSample, paddedSize);
    Aquila::TextPlot plt("Input signal");
    plt.plot(x, SIZE);

    // calculate the FFT
    Aquila::OouraFft fft(SIZE);
    Aquila::ComplexType spectrum[SIZE];
    fft.fft(x, spectrum);

    plt.setTitle("Spectrum");
    plt.plotSpectrum(spectrum, SIZE);
	system("PAUSE");

    return 0;
    */

