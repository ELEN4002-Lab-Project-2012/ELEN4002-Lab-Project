
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
    double SSVEPfreq1 = 11, SSVEPfreq2 = 13, SSVEPfreq3 = 18;
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
	//boost::shared_ptr<PSDAclassifier> myClassifier(new PSDAclassifier(sampleSize, sampleFreq, numChannels, usePadding, window));
	boost::shared_ptr<MECclassifier> myClassifier(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window));
    //boost::shared_ptr<CCAclassifier> myClassifier(new CCAclassifier(sampleSize, sampleFreq, numChannels));
    boost::shared_ptr<SSVEPMonitor> myMonitor(new SSVEPMonitor(SSVEPfreq1));

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
    Sleep(5000);                                   // Main thread does something else while SSVEP algorithme executes....
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