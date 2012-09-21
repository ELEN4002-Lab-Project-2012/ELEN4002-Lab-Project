#include "EmoController.h"
#include "PSDAclassifier.h"
#include "MECclassifier.h"
#include <iostream>
#include <boost\shared_ptr.hpp>
#include "Thread.h"
using namespace std;

typedef boost::shared_ptr<Thread> CppThread;
// Global variables
extern bool testComplete;
extern double accuracy;

int main()
{
    int sampleSize = 512;
    const int paddedSize = sampleSize + 256;    // Note that for now, I'm not using padding at all. TO DO: include padding
    const unsigned int sampleFreq = 128;
    double SSVEPfreq = 10;
    bool useTestData = true;                   // 0 = use raw EEG data, 1 = test data.
    int numChannels = 4;
    testComplete = false;
 
    // Here we choose the classifier type: PSDAclassifier, MECclassifier or CCAclassifier. In this case I chose MEC
    boost::shared_ptr<MECclassifier> myClassifier(new MECclassifier(sampleSize, sampleFreq, numChannels));    

    /* Multithreading example ---------------------------------------------
       Note that the EmoController can also be run on a separate thread. To do this, you need to:
       - First create the controller as a shared_ptr -> add it to the thread -> start the thread -> wait for completion.
       Example: */

    boost::shared_ptr<EmoController> myController(new EmoController(numChannels));
    myController->initClassifier(myClassifier,SSVEPfreq,useTestData);
    CppThread thread1(new Thread(myController));    // Create the thread
    thread1->start();                               // Resumes the thread & calls the loop function.
    Sleep(3000);                                    // Main thread does something else while SSVEP algorithme executes....
    testComplete = true;                            // To stop the thread, you need to change the testComplete variable (see test Suite main())
    thread1->waitForCompletion();                   // Wait for the thread to finish execution
    
    /* We may choose to NOT run the controller on a separate thread. In this case we do the following:
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

