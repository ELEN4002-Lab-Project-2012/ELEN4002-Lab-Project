#include "EmoController.h"
#include "PSDAclassifier.h"
#include "MECclassifier.h"
#include <iostream>
#include <boost\shared_ptr.hpp>
#include "Thread.h"
#include "SSVEPMonitor.h"
#include "Logger.h"
#include "CCAclassifier.h"
using namespace std;

typedef boost::shared_ptr<Thread> CppThread;
// Global variables.
extern bool testComplete;

int main()
{
    int sampleSize = 256;
    const unsigned int sampleFreq = 128;
    double SSVEPfreq1 = 10, SSVEPfreq2 = 13, SSVEPfreq3 = 18;
    vector<double> SSVEPfrequencies; 
    SSVEPfrequencies.push_back(SSVEPfreq1);
    //SSVEPfrequencies.push_back(SSVEPfreq2);
    //SSVEPfrequencies.push_back(SSVEPfreq3);

    bool useTestData = false;                    // 0 = use raw EEG data, 1 = test data.
    int numChannels = 4;
    testComplete = false;
    bool usePadding = true;
    Aquila::WindowType window = Aquila::WIN_HAMMING;   // Aquila::WIN_RECT, Aquila::WIN_HAMMING, Aquila::WIN_HANN are available
    Logger myLogger;

    // Here we choose the classifier type: PSDAclassifier, MECclassifier or CCAclassifier. In this case I chose MEC
    // The reason for have so many parameters is so that we can change them externally when testing. 
	//boost::shared_ptr<MECclassifier> myClassifier(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window));
	boost::shared_ptr<CCAclassifier> myClassifier(new CCAclassifier(sampleSize, sampleFreq, numChannels));
    boost::shared_ptr<SSVEPMonitor> myMonitor(new SSVEPMonitor(SSVEPfrequencies));

    /* Multithreading example ---------------------------------------------
       Note that the EmoController can also be run on a separate thread. To do this, you need to:
       - First create the controller as a shared_ptr -> add it to the thread -> start the thread -> wait for completion.
       Example: */
    boost::shared_ptr<EmoController> myController(new EmoController(numChannels));
    cout << "Use test data? 1 = yes, 0 = no" << endl;
    cin >> useTestData;
    myController->initClassifier(myClassifier,myMonitor,SSVEPfrequencies,useTestData);

    CppThread thread1(new Thread(myController));    // Create the thread
    thread1->start();                               // Resumes the thread & calls the loop function.
    Sleep(30000);                                   // Main thread does something else while SSVEP algorithme executes....
    testComplete = true;                            // To stop the thread, you need to change the testComplete variable (see test Suite main())
    thread1->waitForCompletion();                   // Wait for the thread to finish execution 

    cout << endl << "Accuracy of detection for frequencies: " << endl;
    FreqAccuracy accuracyStruct = myMonitor->calculateAccuracy();
    for(int i = 0; i != accuracyStruct.frequencies.size(); i++)
        cout << "Accuracy (" << accuracyStruct.frequencies.at(i) << " Hz) = " << accuracyStruct.Accuracy.at(i) << endl;

    myController->disconnectEmoEngine();

    myLogger.logR("General Test",SSVEPfreq1, myMonitor->getAllRForFreq(SSVEPfreq1));
    
    /*// We may choose to NOT run the controller on a separate thread. In this case we do the following:
    EmoController controller(numChannels);  
    controller.initClassifier(myClassifier,SSVEPfreq,useTestData);
    controller.loop();                                                      
    controller.disconnectEmoEngine();*/

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

