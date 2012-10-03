/* Code derived from
    http://www.bogotobogo.com/cplusplus/multithreaded2.php
    http://www.computersciencelab.com/MultithreadingTut1.htm
*/

/* TO DO
- Investigate the effect of making the hysteresis band larger
- Draw proper squares
- For the R values  test, introduce a delay period of blank screen
*/

#include "Thread.h"
#include "AllegroFlasher.h"
#include "EmoController.h"
#include "MECclassifier.h"
#include "PSDAclassifier.h"
#include "CCAclassifier.h"
#include "Logger.h"
#include "SSVEPMonitor.h"
#include <sstream>

/* ======== typedefs ======== */
typedef boost::shared_ptr<AllegroFlasher> Flasher;
typedef boost::shared_ptr<Thread> CppThread;
typedef boost::shared_ptr<EmoController> Controller;

/* ======== Global variables for testing purposes ======== */
const int numChannels = 4;
int sampleSize = 256;
const unsigned int sampleFreq = 128;
bool useTestData = false;               // Use test data or not 
int testRunTime = 20;                   // Second: Total time each test runs for
const unsigned int restTime = 6000;     // Milliseconds: Total time of rest between between 
Logger testLogger;                      // Used for logging test results to file
enum SSVEPCLASSIFIER {PSDA, MEC, CCA};
boost::shared_ptr<SSVEPclassifier> myClassifier;

// Allegro Tests 
double allegroNoZeroPadding(Controller controller, Flasher flasher, SSVEPCLASSIFIER);
double allegroZeroPadding(Controller controller, Flasher flasher, SSVEPCLASSIFIER);
double allegroRectangularWindow(Controller controller, Flasher flasher, SSVEPCLASSIFIER);
double allegroChangeDelayTime(Controller controller, Flasher flasher, SSVEPCLASSIFIER);
double allegroFalsePositiveDetectionRate(Controller controller, Flasher flasher, SSVEPCLASSIFIER);
void allegroMECRValuesOverFreqRange(Controller controller, Flasher flasher, SSVEPCLASSIFIER);
void allegroMultipleFrequenciesSimultaneously(Controller controller, Flasher flasher, SSVEPCLASSIFIER);
void allegroMECRValues(Controller controller, Flasher flasher, SSVEPCLASSIFIER);

// LED flasher tests
double LEDNoZeroPadding(Controller controller, SSVEPCLASSIFIER);
double LEDPadding(Controller controller, SSVEPCLASSIFIER);
double LEDRectangularWindow(Controller controller, SSVEPCLASSIFIER);
double LEDChangeDelayTime(Controller controller, SSVEPCLASSIFIER);
double LEDFalsePositiveDetectionRate(Controller controller, SSVEPCLASSIFIER);
void LEDRValuesOverFreqRange(Controller controller, SSVEPCLASSIFIER);
void LEDMultipleFrequenciesInIsolation(Controller controller, SSVEPCLASSIFIER);
void LEDMultipleFrequenciesSimultaneously(Controller controller, SSVEPCLASSIFIER);

// General Test Groupings

// Global variable to stop the main controller loop once the test is complete.
extern bool testComplete;

int main() {  
    const int width = 1024, height = 720;
    boost::shared_ptr<EmoController> controller(new EmoController(numChannels));
    cout << "Use test data? 1 = yes, 0 = no" << endl;
    cin >> useTestData;
    //Flasher SSVEPflasher(new AllegroFlasher(width, height));
    enum SSVEPCLASSIFIER testClassifier = MEC;    // Chosen classifier type (PSDA, MEC or CCA)

    // ============================ LED Tests ==============================
    // To perform the tests, choose the SSVEPCLASSIFIER enum type (see above)

    /*LEDNoZeroPadding(controller, testClassifier);
    LEDPadding(controller, testClassifier);
    LEDRectangularWindow(controller, testClassifier);
    LEDChangeDelayTime(controller, testClassifier);
    LEDFalsePositiveDetectionRate(controller, testClassifier);
    LEDRValuesOverFreqRange(controller, testClassifier);*/
    LEDMultipleFrequenciesInIsolation(controller, testClassifier);
    LEDMultipleFrequenciesSimultaneously(controller, testClassifier);

    // ================= MEC allegro tests ==================
    //plotMECRValues(controller, SSVEPflasher, MEC);
    /*allegroNoZeroPadding(controller, SSVEPflasher, MEC);
    allegroZeroPadding(controller, SSVEPflasher, MEC);
    allegroRectangularWindow(controller, SSVEPflasher, MEC);
    allegroChangeDelayTime(controller, SSVEPflasher, MEC);
    allegroFalsePositiveDetectionRate(controller, SSVEPflasher, MEC);
    allegroMECRValuesOverFreqRange(controller, SSVEPflasher, MEC);*/

    controller->disconnectEmoEngine();
    
    system("PAUSE");
	return 0;
}

/* --------------------------------------------------------------------------------------------------
   LED TESTS
---------------------------------------------------------------------------------------------------*/

double LEDNoZeroPadding(Controller controller, SSVEPCLASSIFIER classifier)
{
    double SSVEPfreq = 11;
    bool usePadding = false;
    Aquila::WindowType window = Aquila::WIN_HAMMING;
    testComplete = false;                               // Global variable stored in EmoController
    Monitor monitor(new SSVEPMonitor(SSVEPfreq));

    // Create and start a new SSVEP classifier thread
    switch(classifier){ 
        case PSDA:
            myClassifier.reset(new PSDAclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case MEC:
            myClassifier.reset(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case CCA:
            myClassifier.reset(new CCAclassifier(sampleSize, sampleFreq, numChannels)); break;
    }
    controller->initClassifier(myClassifier, monitor, SSVEPfreq, useTestData);
    cout << "Press any key to continue." << endl; cin.ignore();
    cout << endl << "LEDNoZeroPadding test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    CppThread thread1(new Thread(controller));
    thread1->start(); 

    // Sleep for the total test time.
    Sleep(testRunTime*1000);        // Wait for the test run time before stopping the classifier
    testComplete = true;            // Break from the SSVEP loop
    thread1->waitForCompletion();
    FreqAccuracy accuracyStruct = monitor->calculateAccuracy();
    testLogger.logAccuracy("LEDNoZeroPadding", accuracyStruct.Accuracy.at(0));
    testLogger.logR("LEDNoZeroPadding", SSVEPfreq, monitor->getAllRForFreq(SSVEPfreq));
    return accuracyStruct.Accuracy.at(0);
}

double LEDPadding(Controller controller, SSVEPCLASSIFIER classifier)
{
    cout << "Press any key to continue." << endl; cin.ignore();
    cout << endl << "LEDPadding test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    double SSVEPfreq = 11;
    bool usePadding = true;
    Aquila::WindowType window = Aquila::WIN_HAMMING;
    testComplete = false;
    Monitor monitor(new SSVEPMonitor(SSVEPfreq));

    // Create and start a new SSVEP classifier thread
    switch(classifier){ 
        case PSDA:
            myClassifier.reset(new PSDAclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case MEC:
            myClassifier.reset(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case CCA:
            myClassifier.reset(new CCAclassifier(sampleSize, sampleFreq, numChannels)); break;
    }
    controller->initClassifier(myClassifier, monitor, SSVEPfreq, useTestData);
    CppThread thread1(new Thread(controller));
    thread1->start(); 

    Sleep(testRunTime*1000);        
    testComplete = true;           
    thread1->waitForCompletion();
    FreqAccuracy accuracyStruct = monitor->calculateAccuracy();
    testLogger.logAccuracy("LEDPadding", accuracyStruct.Accuracy.at(0));
    testLogger.logR("LEDPadding", SSVEPfreq, monitor->getAllRForFreq(SSVEPfreq));
    return accuracyStruct.Accuracy.at(0);
}

double LEDRectangularWindow(Controller controller, SSVEPCLASSIFIER classifier)
{
cout << "Press any key to continue." << endl; cin.ignore();
    cout << endl << "LEDRectangularWindow test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    double SSVEPfreq = 11;
    bool usePadding = true;
    Aquila::WindowType window = Aquila::WIN_RECT;    // RECTANGULAR WINDOW
    testComplete = false;
    Monitor monitor(new SSVEPMonitor(SSVEPfreq));

    // Create and start a new SSVEP classifier thread
    switch(classifier){ 
        case PSDA:
            myClassifier.reset(new PSDAclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case MEC:
            myClassifier.reset(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case CCA:
            myClassifier.reset(new CCAclassifier(sampleSize, sampleFreq, numChannels)); break;
    }
    controller->initClassifier(myClassifier, monitor, SSVEPfreq, useTestData);
    CppThread thread1(new Thread(controller));
    thread1->start(); 

    Sleep(testRunTime*1000);        
    testComplete = true;           
    thread1->waitForCompletion();
    FreqAccuracy accuracyStruct = monitor->calculateAccuracy();
    testLogger.logAccuracy("LEDRectangularWindow", accuracyStruct.Accuracy.at(0));
    testLogger.logR("LEDRectangularWindow", SSVEPfreq, monitor->getAllRForFreq(SSVEPfreq));
    return accuracyStruct.Accuracy.at(0);
}

double LEDChangeDelayTime(Controller controller, SSVEPCLASSIFIER classifier)
{
cout << "Press any key to continue." << endl; cin.ignore();
    cout << endl << "LEDChangeDelayTime test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    controller->setDelayTime(400);      // Double the delay time
    double SSVEPfreq = 11;
    bool usePadding = true;
    Aquila::WindowType window = Aquila::WIN_HAMMING;
    testComplete = false;
    Monitor monitor(new SSVEPMonitor(SSVEPfreq));

    // Create and start a new SSVEP classifier thread
    switch(classifier){ 
        case PSDA:
            myClassifier.reset(new PSDAclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case MEC:
            myClassifier.reset(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case CCA:
            myClassifier.reset(new CCAclassifier(sampleSize, sampleFreq, numChannels)); break;
    }
    controller->initClassifier(myClassifier, monitor, SSVEPfreq, useTestData);
    CppThread thread1(new Thread(controller));
    thread1->start(); 

    Sleep(testRunTime*1000);        
    testComplete = true;           
    thread1->waitForCompletion();
    FreqAccuracy accuracyStruct = monitor->calculateAccuracy();
    testLogger.logAccuracy("testChangeDelayTime", accuracyStruct.Accuracy.at(0));
    testLogger.logR("testChangeDelayTime", SSVEPfreq, monitor->getAllRForFreq(SSVEPfreq));
    controller->setDelayTime(200);      // Reset the delay time back to 200ms
    return accuracyStruct.Accuracy.at(0);
}

double LEDFalsePositiveDetectionRate(Controller controller, SSVEPCLASSIFIER classifier)
{
cout << "Press any key to continue." << endl; cin.ignore();
    cout << endl << "LEDFalsePositiveDetectionRate test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    double SSVEPfreq = 18;                           // SSVEP detection freq different from actual frequency 
    bool usePadding = true;
    Aquila::WindowType window = Aquila::WIN_HAMMING;
    testComplete = false;
    Monitor monitor(new SSVEPMonitor(SSVEPfreq));

    // Create and start a new SSVEP classifier thread
    switch(classifier){
        case PSDA:
            myClassifier.reset(new PSDAclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case MEC:
            myClassifier.reset(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case CCA:
            myClassifier.reset(new CCAclassifier(sampleSize, sampleFreq, numChannels)); break;
    }
    controller->initClassifier(myClassifier, monitor, SSVEPfreq, useTestData);
    CppThread thread1(new Thread(controller));
    thread1->start(); 

    Sleep(testRunTime*1000);        
    testComplete = true;           
    thread1->waitForCompletion();
    FreqAccuracy accuracyStruct = monitor->calculateAccuracy();
    testLogger.logAccuracy("LEDFalsePositiveDetectionRate", accuracyStruct.Accuracy.at(0));
    testLogger.logR("LEDFalsePositiveDetectionRate", SSVEPfreq, monitor->getAllRForFreq(SSVEPfreq));
    return accuracyStruct.Accuracy.at(0);
}

void LEDRValuesOverFreqRange(Controller controller, SSVEPCLASSIFIER classifier)
{
    bool usePadding = true;
    Aquila::WindowType window = Aquila::WIN_HAMMING;
    const int numFrequencies = 18;                      // 8 Frequencies in the test [8,25] Hz range
    double SSVEPfreq = 8;                               // Starting frequency
    Monitor monitor(new SSVEPMonitor(SSVEPfreq));
    
    switch(classifier){
        case PSDA:
            myClassifier.reset(new PSDAclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case MEC:
            myClassifier.reset(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case CCA:
            myClassifier.reset(new CCAclassifier(sampleSize, sampleFreq, numChannels)); break;
    }

    for(int i = 0; i != numFrequencies; i++)
    {
        cout << "Press any key to continue." << endl; cin.ignore();
        cout << endl << "LEDRValuesOverFreqRange test for frequency = " << SSVEPfreq << " starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
        testComplete = false;
        controller->initClassifier(myClassifier, monitor, SSVEPfreq, useTestData);
        CppThread thread1(new Thread(controller));
        thread1->start(); 

        Sleep(testRunTime*1000);        
        testComplete = true;           
        thread1->waitForCompletion();
        std::stringstream out;      // Convert double to string in order to log the frequency
        out << SSVEPfreq;
        FreqAccuracy accuracyStruct = monitor->calculateAccuracy();
        testLogger.logAccuracy("LEDRvaluesOverFreq" + out.str(), accuracyStruct.Accuracy.at(0));        // Index 0 because there is only 1 freq
        testLogger.logR("LEDRvaluesOverFreq", SSVEPfreq, monitor->getAllRForFreq(SSVEPfreq));
        
        SSVEPfreq++;
        monitor.reset(new SSVEPMonitor(SSVEPfreq));
    }
}

/* For this test, the two stimulation sources should be directly adjacent to each other flashing continuously
   The user is requested to look at the one then the other */
void LEDMultipleFrequenciesSimultaneously(Controller controller, SSVEPCLASSIFIER classifier)
{
    cout << "Press any key to continue." << endl; cin.ignore();
    cout << endl << "LEDMultipleFrequenciesSimultaneously test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    double SSVEPfreq0 = 10, SSVEPfreq1 = 13;
    bool usePadding = true;
    Aquila::WindowType window = Aquila::WIN_HAMMING;
    testComplete = false; 

    vector<double> SSVEPfrequencies;        // Test frequencies
    SSVEPfrequencies.push_back(SSVEPfreq0);
    SSVEPfrequencies.push_back(SSVEPfreq1);
    Monitor monitor(new SSVEPMonitor(SSVEPfrequencies));

    // Create and start a new SSVEP classifier thread
    switch(classifier){ 
        case PSDA:
            myClassifier.reset(new PSDAclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case MEC:
            myClassifier.reset(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case CCA:
            myClassifier.reset(new CCAclassifier(sampleSize, sampleFreq, numChannels)); break;
    }
    controller->initClassifier(myClassifier, monitor, SSVEPfrequencies, useTestData);
    CppThread threadClassifier(new Thread(controller));
    threadClassifier->start(); 

    
    for(int i = 0; i != SSVEPfrequencies.size(); i++)
    {
        cout << endl << endl << endl << endl << endl;
        cout << "===============================================================================";
        cout << "PLEASE START LOOKING AT LED WITH FREQUENCY = " << SSVEPfrequencies.at(i) << endl;
        cout << "===============================================================================";
        cout << endl << endl << endl << endl << endl;
        Sleep(testRunTime*1000);
    }
    testComplete = true;
    threadClassifier->waitForCompletion();
    
    std::stringstream out;      // Convert double to string in order to log the accuracy per frequency 
    FreqAccuracy accuracyStruct = monitor->calculateAccuracy();
    // Log R values for the whole experiment
    for(int i = 0; i != SSVEPfrequencies.size(); i++) {
        out.str(""); out << SSVEPfrequencies.at(i);         // Add the frequency to the name using stringstream
        // Values are collected for the whole period so accuracy is not really valuable in this test. A guestimate is 2 x Accuracy.at(i)
        testLogger.logAccuracy("MultipleFreqSimul" + out.str() + "Hz", 2*accuracyStruct.Accuracy.at(i));
        testLogger.logR("LEDRvaluesForMultipleFreqSimul", SSVEPfrequencies.at(i), monitor->getAllRForFreq(SSVEPfrequencies.at(i)));
    }
}

void LEDMultipleFrequenciesInIsolation(Controller controller, SSVEPCLASSIFIER classifier)
{
    cout << "Press any key to continue." << endl; cin.ignore();
    cout << endl << "LEDMultipleFrequenciesInIsolation test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    double SSVEPfreq0 = 10, SSVEPfreq1 = 13;
    bool usePadding = true;
    Aquila::WindowType window = Aquila::WIN_HAMMING;
    testComplete = false; 

    vector<double> SSVEPfrequencies;        // Test frequencies
    SSVEPfrequencies.push_back(SSVEPfreq0);
    SSVEPfrequencies.push_back(SSVEPfreq1);
    Monitor monitor(new SSVEPMonitor(SSVEPfrequencies));

    // Create and start a new SSVEP classifier thread
    switch(classifier){ 
        case PSDA:
            myClassifier.reset(new PSDAclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case MEC:
            myClassifier.reset(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case CCA:
            myClassifier.reset(new CCAclassifier(sampleSize, sampleFreq, numChannels)); break;
    }
    controller->initClassifier(myClassifier, monitor, SSVEPfrequencies, useTestData);
    CppThread threadClassifier(new Thread(controller));
    threadClassifier->start(); 

    
    for(int i = 0; i != SSVEPfrequencies.size(); i++)
    {
        cout << endl << endl << endl << endl << endl;
        cout << "===============================================================================";
        cout << "PLEASE START LOOKING AT LED WITH FREQUENCY = " << SSVEPfrequencies.at(i) << endl;
        cout << "===============================================================================";
        cout << endl << endl << endl << endl << endl;
        Sleep(testRunTime*1000);
    }
    testComplete = true;
    threadClassifier->waitForCompletion();
    
    std::stringstream out;      // Convert double to string in order to log the accuracy per frequency 
    FreqAccuracy accuracyStruct = monitor->calculateAccuracy();
    // Log R values for the whole experiment
    for(int i = 0; i != SSVEPfrequencies.size(); i++) {
        out.str(""); out << SSVEPfrequencies.at(i);         // Add the frequency to the name using stringstream
        testLogger.logAccuracy("MultipleFreqIsolation" + out.str() + "Hz", 2*accuracyStruct.Accuracy.at(i));
        testLogger.logR("LEDRvaluesForMultipleFreqIsol", SSVEPfrequencies.at(i), monitor->getAllRForFreq(SSVEPfrequencies.at(i)));
    }
}

/* --------------------------------------------------------------------------------------------------
   ALLEGRO TESTS
---------------------------------------------------------------------------------------------------*/

//  Test to find the accuracy when using no zero padding
double allegroNoZeroPadding(Controller controller, Flasher flasher, SSVEPCLASSIFIER classifier)
{
    cout << "Press any key to continue." << endl; cin.ignore();
    cout << endl << "Next test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    cout << "testNoZeroPadding" << endl;
    double SSVEPfreq = 13;
    bool usePadding = false;
    Aquila::WindowType window = Aquila::WIN_HAMMING;
    testComplete = false;                               // Global variable stored in EmoController
    Monitor monitor(new SSVEPMonitor(SSVEPfreq));

    // Create and start a new SSVEP classifier thread
    switch(classifier){ 
        case PSDA:
            myClassifier.reset(new PSDAclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case MEC:
            myClassifier.reset(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case CCA:
            myClassifier.reset(new CCAclassifier(sampleSize, sampleFreq, numChannels)); break;
    }
    controller->initClassifier(myClassifier, monitor, SSVEPfreq, useTestData);
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
    FreqAccuracy accuracyStruct = monitor->calculateAccuracy();
    testLogger.logAccuracy("testNoZeroPadding", accuracyStruct.Accuracy.at(0));
    return accuracyStruct.Accuracy.at(0);                // Accuracy is a global variable changed by SSVEP once the test is complete
}

//  Test to find the accuracy when using zero padding
double allegroZeroPadding(Controller controller, Flasher flasher, SSVEPCLASSIFIER classifier)
{
    cout << "Press any key to continue." << endl; cin.ignore();
    cout << endl << "Next test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    cout << "testZeroPadding" << endl;
    double SSVEPfreq = 13;
    bool usePadding = true;
    Aquila::WindowType window = Aquila::WIN_HAMMING;
    testComplete = false;
    Monitor monitor(new SSVEPMonitor(SSVEPfreq));

    // Create and start a new SSVEP classifier thread
    switch(classifier){ 
        case PSDA:
            myClassifier.reset(new PSDAclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case MEC:
            myClassifier.reset(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case CCA:
            myClassifier.reset(new CCAclassifier(sampleSize, sampleFreq, numChannels)); break;
    }
    controller->initClassifier(myClassifier, monitor, SSVEPfreq, useTestData);
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
    FreqAccuracy accuracyStruct = monitor->calculateAccuracy();
    testLogger.logAccuracy("testZeroPadding", accuracyStruct.Accuracy.at(0));
    return accuracyStruct.Accuracy.at(0);
}

// Test the accuracy when using a rectangular window instead of a Hamming window
double allegroRectangularWindow(Controller controller, Flasher flasher, SSVEPCLASSIFIER classifier)
{
    cout << "Press any key to continue." << endl; cin.ignore();
    cout << endl << "Next test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    cout << "testRectangularWindow" << endl;
    double SSVEPfreq = 13;
    bool usePadding = true;
    Aquila::WindowType window = Aquila::WIN_RECT;    // RECTANGULAR WINDOW
    testComplete = false;
    Monitor monitor(new SSVEPMonitor(SSVEPfreq));

    // Create and start a new SSVEP classifier thread
    switch(classifier){ 
        case PSDA:
            myClassifier.reset(new PSDAclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case MEC:
            myClassifier.reset(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case CCA:
            myClassifier.reset(new CCAclassifier(sampleSize, sampleFreq, numChannels)); break;
    }
    controller->initClassifier(myClassifier, monitor, SSVEPfreq, useTestData);
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
    FreqAccuracy accuracyStruct = monitor->calculateAccuracy();
    testLogger.logAccuracy("testRectangularWindow", accuracyStruct.Accuracy.at(0));
    return accuracyStruct.Accuracy.at(0);
}

// Test the accuracy when increasing the delay time of sampling
double allegroChangeDelayTime(Controller controller, Flasher flasher, SSVEPCLASSIFIER classifier)
{
    cout << "Press any key to continue." << endl; cin.ignore();
    cout << endl << "Next test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    cout << "testChangeDelayTime" << endl;
    controller->setDelayTime(400);      // Double the delay time
    double SSVEPfreq = 13;
    bool usePadding = false;
    Aquila::WindowType window = Aquila::WIN_RECT;
    testComplete = false;
    Monitor monitor(new SSVEPMonitor(SSVEPfreq));

    // Create and start a new SSVEP classifier thread
    switch(classifier){ 
        case PSDA:
            myClassifier.reset(new PSDAclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case MEC:
            myClassifier.reset(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case CCA:
            myClassifier.reset(new CCAclassifier(sampleSize, sampleFreq, numChannels)); break;
    }
    controller->initClassifier(myClassifier, monitor, SSVEPfreq, useTestData);
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
       
    FreqAccuracy accuracyStruct = monitor->calculateAccuracy();
    testLogger.logAccuracy("testChangeDelayTime", accuracyStruct.Accuracy.at(0));
    controller->setDelayTime(200);      // Reset the delay time back to 200ms
    return accuracyStruct.Accuracy.at(0);
}

// Find the rate of false positive occurances. 
double allegroFalsePositiveDetectionRate(Controller controller, Flasher flasher, SSVEPCLASSIFIER classifier)
{
    cout << "Press any key to continue." << endl; cin.ignore();
    cout << endl << "Next test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    cout << "testFalsePositiveDetectionRate" << endl;
    double SSVEPfreq = 18;                           // SSVEP detection freq different from actual frequency 
    const int allegroFreq = 13;                      // Hz and Seconds
    bool usePadding = true;
    Aquila::WindowType window = Aquila::WIN_RECT;
    testComplete = false;
    Monitor monitor(new SSVEPMonitor(SSVEPfreq));

    // Create and start a new SSVEP classifier thread
    switch(classifier){ 
        case PSDA:
            myClassifier.reset(new PSDAclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case MEC:
            myClassifier.reset(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case CCA:
            myClassifier.reset(new CCAclassifier(sampleSize, sampleFreq, numChannels)); break;
    }
    controller->initClassifier(myClassifier, monitor, SSVEPfreq, useTestData);
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
    
    FreqAccuracy accuracyStruct = monitor->calculateAccuracy();
    testLogger.logAccuracy("testFalsePositiveDetectionRate", accuracyStruct.Accuracy.at(0));
    return accuracyStruct.Accuracy.at(0);
}

// Record the values of R for a range of frequencies
void allegroMECRValuesOverFreqRange(Controller controller, Flasher flasher, SSVEPCLASSIFIER classifier)
{
    cout << "Press any key to continue." << endl; cin.ignore();
    cout << endl << "Next test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    cout << "testMECRValuesOverFreqRange" << endl; 
    bool usePadding = true;
    Aquila::WindowType window = Aquila::WIN_HAMMING;
    const int numFrequencies = 18;                  // 8 Frequencies in the test [8,25] Hz range
    double SSVEPfreq = 8;                           // Starting frequency
    boost::shared_ptr<MECclassifier> myMECclassifier(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window));
    Monitor monitor(new SSVEPMonitor(SSVEPfreq));

    for(int i = 0; i != numFrequencies; i++)
    {
        cout << "MEC R test for frequency = " << SSVEPfreq << "Hz" << endl;
        testComplete = false;
        
        // Create and start a new SSVEP classifier thread
        myMECclassifier.reset(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window));
        controller->initClassifier(myMECclassifier, monitor, SSVEPfreq, useTestData);
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
        testLogger.logR("AllegroRValsOverFreq", SSVEPfreq, monitor->getAllRForFreq(SSVEPfreq));
        
        SSVEPfreq++;
        cout << "Press any key to continue." << endl; cin.ignore();
        cout << endl << "Next test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    }
}

void allegroMultipleFrequenciesSimultaneously(Controller controller, Flasher flasher, SSVEPCLASSIFIER classifier)
{
    cout << "Press any key to continue." << endl; cin.ignore();
    cout << endl << "Next test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    cout << "testMultipleFrequenciesSimultaneously" << endl;
    double SSVEPfreq0 = 8, SSVEPfreq1 = 10, SSVEPfreq2 = 13, SSVEPfreq3 = 15;
    bool usePadding = false;
    Aquila::WindowType window = Aquila::WIN_RECT;
    testComplete = false; 
    const int width = 200, height = 200;

    vector<double> SSVEPfrequencies;      // Test frequencies
    SSVEPfrequencies.push_back(SSVEPfreq0);
    SSVEPfrequencies.push_back(SSVEPfreq1);
    SSVEPfrequencies.push_back(SSVEPfreq2);
    SSVEPfrequencies.push_back(SSVEPfreq3);
    Monitor monitor(new SSVEPMonitor(SSVEPfrequencies));

    // Create four flashing screens on four threads.
    Flasher flasher0(new AllegroFlasher(width, height));
    Flasher flasher1(new AllegroFlasher(width, height));
    Flasher flasher2(new AllegroFlasher(width, height));
    Flasher flasher3(new AllegroFlasher(width, height));
    
    flasher0->initFlasher(SSVEPfreq0, testRunTime, 0, 0, 0); flasher0->positionWindow(10, 10);
    flasher1->initFlasher(SSVEPfreq1, testRunTime, 0, 0, 0); flasher1->positionWindow(300, 10);
    flasher2->initFlasher(SSVEPfreq2, testRunTime, 0, 0, 0); flasher2->positionWindow(10, 300);
    flasher3->initFlasher(SSVEPfreq3, testRunTime, 0, 0, 0); flasher3->positionWindow(300, 300);

    // Create and start a new SSVEP classifier thread
    switch(classifier){ 
        case PSDA:
            myClassifier.reset(new PSDAclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case MEC:
            myClassifier.reset(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window)); break;
        case CCA:
            myClassifier.reset(new CCAclassifier(sampleSize, sampleFreq, numChannels)); break;
    }
    controller->initClassifier(myClassifier, monitor, SSVEPfrequencies, useTestData);
    CppThread threadClassifier(new Thread(controller));
    threadClassifier->start(); 

    // Create and start a new flasher thread
    CppThread threadFlasher0(new Thread(flasher));
    CppThread threadFlasher1(new Thread(flasher));
    CppThread threadFlasher2(new Thread(flasher));
    CppThread threadFlasher3(new Thread(flasher));

    threadFlasher0->start();
    threadFlasher1->start();
    threadFlasher2->start();
    threadFlasher3->start();
    Sleep(10);
    threadFlasher0->waitForCompletion();
    threadFlasher1->waitForCompletion();
    threadFlasher2->waitForCompletion();
    threadFlasher3->waitForCompletion();
    testComplete = true;
    threadClassifier->waitForCompletion();

    threadFlasher0.reset();
    threadFlasher1.reset();
    threadFlasher2.reset();
    threadFlasher3.reset();
}

void allegroMECRValues(Controller controller, Flasher flasher, SSVEPCLASSIFIER classifier)
{
    cout << "Press any key to continue." << endl; cin.ignore();
    cout << endl << "Next test starts in " << restTime/1000 << " seconds..." << endl; Sleep(restTime);
    testRunTime = 600;
    double SSVEPfreq = 13;
    bool usePadding = true;
    Aquila::WindowType window = Aquila::WIN_HAMMING;
    testComplete = false;
    boost::shared_ptr<MECclassifier> myMECclassifier(new MECclassifier(sampleSize, sampleFreq, numChannels, usePadding, window));
    Monitor monitor(new SSVEPMonitor(SSVEPfreq));

    controller->initClassifier(myMECclassifier, monitor, SSVEPfreq, useTestData);
    CppThread thread1(new Thread(controller));
    thread1->start(); 

    // Create and start a new flasher thread
    flasher->initFlasher(SSVEPfreq, testRunTime, 0, 0, 0);
    CppThread thread2(new Thread(flasher));
    thread2->start(); 
    Sleep(10);
    while(1)
    {
        testLogger.plotR(monitor->getAllRForFreq(SSVEPfreq));
        Sleep(500);
    }

    thread2->waitForCompletion();
    testComplete = true;
    thread1->waitForCompletion();
    myMECclassifier.reset();
    testRunTime = 20;
}

