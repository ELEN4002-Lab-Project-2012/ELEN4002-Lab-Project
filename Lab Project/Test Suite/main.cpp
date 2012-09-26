/* Code derived from
    http://www.bogotobogo.com/cplusplus/multithreaded2.php
    http://www.computersciencelab.com/MultithreadingTut1.htm
*/

/* TO DO
- Investigate the effect of making the hysteresis band larger
- Draw proper squares
- For the R values test, introduce a delay period of blank screen
*/

#include "Thread.h"
#include "AllegroFlasher.h"
#include "EmoController.h"
#include "MECclassifier.h"
#include "PSDAclassifier.h"
#include "CCAclassifier.h"
#include "Logger.h"

/* ======== typedefs ======== */
typedef boost::shared_ptr<AllegroFlasher> Flasher;
typedef boost::shared_ptr<Thread> CppThread;
typedef boost::shared_ptr<EmoController> Controller;

/* ======== Global variables for testing purposes ======== */
const int numChannels = 4;
int sampleSize = 512;
const unsigned int sampleFreq = 128;
bool useTestData = true;                // Use test data or not 
const int testRunTime = 2;              // Total time each test runs for
const unsigned int restTime = 1000;     // Total time of rest between between 
Logger testLogger("TestSuite.csv");     // Used for logging test results to file
enum SSVEPCLASSIFIER {PSDA, MEC, CCA};
boost::shared_ptr<SSVEPclassifier> myClassifier;

double testNoZeroPadding(Controller controller, Flasher flasher, SSVEPCLASSIFIER);
double testZeroPadding(Controller controller, Flasher flasher, SSVEPCLASSIFIER);
double testHammingWindow(Controller controller, Flasher flasher, SSVEPCLASSIFIER);
double testChangeDelayTime(Controller controller, Flasher flasher, SSVEPCLASSIFIER);
double testFalsePositiveDetectionRate(Controller controller, Flasher flasher, SSVEPCLASSIFIER);
void testMECRValuesOverFreqRange(Controller controller, Flasher flasher, SSVEPCLASSIFIER);
void testMultipleFrequenciesSimultaneously(Controller controller, Flasher flasher, SSVEPCLASSIFIER);

void doPSDATests();
void doMECTests();

extern bool testComplete;
extern vector<double> accuracy;
extern vector<double> R;

int main() {  
    const int width = 1024, height = 720;
    boost::shared_ptr<EmoController> controller(new EmoController(numChannels));

    Flasher SSVEPflasher(new AllegroFlasher(width, height));

    // ================ PSDA tests =================


    // ================= MEC tests =================
    cout << "Press any key to continue." << endl; cin.ignore();
    cout << endl << "Next test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    testNoZeroPadding(controller, SSVEPflasher, MEC);
    cout << "Press any key to continue." << endl; cin.ignore();
    cout << endl << "Next test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    testZeroPadding(controller, SSVEPflasher, MEC);
    cout << "Press any key to continue." << endl; cin.ignore();
    cout << endl << "Next test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    testHammingWindow(controller, SSVEPflasher, MEC);
    cout << "Press any key to continue." << endl; cin.ignore();
    cout << endl << "Next test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    testChangeDelayTime(controller, SSVEPflasher, MEC);
    cout << "Press any key to continue." << endl; cin.ignore();
    cout << endl << "Next test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    testFalsePositiveDetectionRate(controller, SSVEPflasher, MEC);
    cout << "Press any key to continue." << endl; cin.ignore();
    cout << endl << "Next test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    testMECRValuesOverFreqRange(controller, SSVEPflasher, MEC);
    //cout << "Press any key to continue." << endl; cin.ignore();
    //cout << endl << "Next test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    //testMultipleFrequenciesSimultaneously(controller, SSVEPflasher, MEC);
    controller->disconnectEmoEngine();
    
    system("PAUSE");
	return 0;
}

//  Test to find the accuracy when using no zero padding
double testNoZeroPadding(Controller controller, Flasher flasher, SSVEPCLASSIFIER classifier)
{
    cout << "testNoZeroPadding" << endl;
    double SSVEPfreq = 13;
    bool usePadding = false;
    Aquila::WindowType window = Aquila::WIN_RECT;
    testComplete = false;                               // Global variable stored in EmoController

    // Create and start a new SSVEP classifier thread
    switch(classifier){ 
        case PSDA:
            myClassifier.reset(new PSDAclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case MEC:
            myClassifier.reset(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case CCA:
            myClassifier.reset(new CCAclassifier(sampleSize, sampleFreq, numChannels, window)); break;
    }
    controller->initClassifier(myClassifier, SSVEPfreq, useTestData);
    CppThread thread1(new Thread(controller));
    thread1->start(); 

    // Create and start a new flasher thread
    flasher->initFlasher(SSVEPfreq, testRunTime, 0, 0, 0);
    CppThread thread2(new Thread(flasher));
    thread2->start(); 
    Sleep(10);  

    // Wait for completion of the flasher thread. Once complete, trigger the completion of the SSVEP thread.
    thread2->waitForCompletion();
    testComplete = true;            // Break from the SSVEP loop
    thread1->waitForCompletion();
    testLogger.logAccuracy("testNoZeroPadding", accuracy.at(0));
    return accuracy.at(0);                // Accuracy is a global variable changed by SSVEP once the test is complete
}

//  Test to find the accuracy when using zero padding
double testZeroPadding(Controller controller, Flasher flasher, SSVEPCLASSIFIER classifier)
{
    cout << "testZeroPadding" << endl;
    double SSVEPfreq = 13;
    bool usePadding = true;
    Aquila::WindowType window = Aquila::WIN_RECT;
    testComplete = false;

    // Create and start a new SSVEP classifier thread
    switch(classifier){ 
        case PSDA:
            myClassifier.reset(new PSDAclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case MEC:
            myClassifier.reset(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case CCA:
            myClassifier.reset(new CCAclassifier(sampleSize, sampleFreq, numChannels, window)); break;
    }
    controller->initClassifier(myClassifier, SSVEPfreq, useTestData);
    CppThread thread1(new Thread(controller));
    thread1->start(); 

    // Create and start a new flasher thread
    flasher->initFlasher(SSVEPfreq, testRunTime, 0, 0, 0);
    CppThread thread2(new Thread(flasher));
    thread2->start(); 
    Sleep(10);
    thread2->waitForCompletion();
    testComplete = true;
    thread1->waitForCompletion();
    testLogger.logAccuracy("testZeroPadding", accuracy.at(0));
    return accuracy.at(0);
}

// Test the accuracy when using a rectangular window instead of a Hamming window
double testHammingWindow(Controller controller, Flasher flasher, SSVEPCLASSIFIER classifier)
{
    cout << "testHammingWindow" << endl;
    double SSVEPfreq = 13;
    bool usePadding = true;
    Aquila::WindowType window = Aquila::WIN_HAMMING;    // RECTANGULAR WINDOW
    testComplete = false;

    // Create and start a new SSVEP classifier thread
    switch(classifier){ 
        case PSDA:
            myClassifier.reset(new PSDAclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case MEC:
            myClassifier.reset(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case CCA:
            myClassifier.reset(new CCAclassifier(sampleSize, sampleFreq, numChannels, window)); break;
    }
    controller->initClassifier(myClassifier, SSVEPfreq, useTestData);
    CppThread thread1(new Thread(controller));
    thread1->start(); 

    // Create and start a new flasher thread
    flasher->initFlasher(SSVEPfreq, testRunTime, 0, 0, 0);
    CppThread thread2(new Thread(flasher));
    thread2->start(); 
    Sleep(10);
    thread2->waitForCompletion();
    testComplete = true;
    thread1->waitForCompletion();
    testLogger.logAccuracy("testRectangularWindow", accuracy.at(0));
    return accuracy.at(0);
}

// Test the accuracy when increasing the delay time of sampling
double testChangeDelayTime(Controller controller, Flasher flasher, SSVEPCLASSIFIER classifier)
{
    cout << "testChangeDelayTime" << endl;
    controller->setDelayTime(400);      // Double the delay time
    double SSVEPfreq = 13;
    bool usePadding = false;
    Aquila::WindowType window = Aquila::WIN_RECT;
    testComplete = false;

    // Create and start a new SSVEP classifier thread
    switch(classifier){ 
        case PSDA:
            myClassifier.reset(new PSDAclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case MEC:
            myClassifier.reset(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case CCA:
            myClassifier.reset(new CCAclassifier(sampleSize, sampleFreq, numChannels, window)); break;
    }
    controller->initClassifier(myClassifier, SSVEPfreq, useTestData);
    CppThread thread1(new Thread(controller));
    thread1->start(); 

    // Create and start a new flasher thread
    flasher->initFlasher(SSVEPfreq, testRunTime, 0, 0, 0);
    CppThread thread2(new Thread(flasher));
    thread2->start(); 
    Sleep(10);
    thread2->waitForCompletion();
    testComplete = true;
    thread1->waitForCompletion();
    
    testLogger.logAccuracy("testChangeDelayTime", accuracy.at(0));   
    controller->setDelayTime(200);      // Reset the delay time back to 200ms
    return accuracy.at(0);
}

// Find the rate of false positive occurances. 
double testFalsePositiveDetectionRate(Controller controller, Flasher flasher, SSVEPCLASSIFIER classifier)
{
    cout << "testFalsePositiveDetectionRate" << endl;
    double SSVEPfreq = 18;                           // SSVEP detection freq different from actual frequency 
    const int allegroFreq = 13;                      // Hz and Seconds
    bool usePadding = true;
    Aquila::WindowType window = Aquila::WIN_RECT;
    testComplete = false;

    // Create and start a new SSVEP classifier thread
    switch(classifier){ 
        case PSDA:
            myClassifier.reset(new PSDAclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case MEC:
            myClassifier.reset(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case CCA:
            myClassifier.reset(new CCAclassifier(sampleSize, sampleFreq, numChannels, window)); break;
    }
    controller->initClassifier(myClassifier, SSVEPfreq, useTestData);
    CppThread thread1(new Thread(controller));
    thread1->start(); 

    // Create and start a new flasher thread
    flasher->initFlasher(allegroFreq, testRunTime, 0, 0, 0);
    CppThread thread2(new Thread(flasher));
    thread2->start(); 
    Sleep(10);
    thread2->waitForCompletion();
    testComplete = true;
    thread1->waitForCompletion();
    
    testLogger.logAccuracy("testFalsePositiveDetectionRate", accuracy.at(0)); 
    return accuracy.at(0);
}

// Record the values of R for a range of frequencies
void testMECRValuesOverFreqRange(Controller controller, Flasher flasher, SSVEPCLASSIFIER classifier)
{
    cout << "testMECRValuesOverFreqRange" << endl; 
    bool usePadding = true;
    Aquila::WindowType window = Aquila::WIN_HAMMING;
    const int numFrequencies = 18;                  // 8 Frequencies in the test [8,25] Hz range
    double SSVEPfreq = 8;                           // Starting frequency

    for(int i = 0; i != numFrequencies; i++)
    {
        cout << "MEC R test for frequency = " << SSVEPfreq << "Hz" << endl;
        testComplete = false;
        
        // Create and start a new SSVEP classifier thread
        boost::shared_ptr<MECclassifier> myMECclassifier(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window));
        controller->initClassifier(myMECclassifier, SSVEPfreq, useTestData);
        CppThread thread1(new Thread(controller));
        thread1->start(); 

        // Create and start a new flasher thread
        flasher->initFlasher(SSVEPfreq, testRunTime, 0, 0, 1);      // Use with a blank start page for 1/3 of the time
        CppThread thread2(new Thread(flasher));
        thread2->start(); 
        Sleep(10);
        thread2->waitForCompletion();
        testComplete = true;
        thread1->waitForCompletion();
        testLogger.logR(SSVEPfreq, myMECclassifier->getRValues());
        
        SSVEPfreq++;
        cout << "Press any key to continue." << endl; cin.ignore();
        cout << endl << "Next test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    }
}

void testMultipleFrequenciesSimultaneously(Controller controller, Flasher flasher, SSVEPCLASSIFIER classifier)
{
    bool usePadding = true;
    Aquila::WindowType window = Aquila::WIN_HAMMING;
    testComplete = false;

    vector<double> SSVEPfrequencies;      // Test frequencies
    SSVEPfrequencies.push_back(8);
    SSVEPfrequencies.push_back(10);
    SSVEPfrequencies.push_back(13);
    SSVEPfrequencies.push_back(15);
    SSVEPfrequencies.push_back(18);
    SSVEPfrequencies.push_back(20);

    // Create and start a new SSVEP classifier thread
    switch(classifier){ 
        case PSDA:
            myClassifier.reset(new PSDAclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case MEC:
            myClassifier.reset(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case CCA:
            myClassifier.reset(new CCAclassifier(sampleSize, sampleFreq, numChannels, window)); break;
    }
    controller->initClassifier(myClassifier, SSVEPfrequencies, useTestData);
    CppThread thread1(new Thread(controller));
    thread1->start(); 

    // Create and start a new flasher thread
    flasher->initFlasher(SSVEPfrequencies, testRunTime, 0, 0, 0);
    CppThread thread2(new Thread(flasher));
    thread2->start(); 
    Sleep(10);
    thread2->waitForCompletion();
    testComplete = true;
    thread1->waitForCompletion();
}
