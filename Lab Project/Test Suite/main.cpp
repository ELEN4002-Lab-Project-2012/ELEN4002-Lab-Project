#include "Thread.h"
#include "AllegroFlasher.h"
//#include "simpleRunnable.h"
#include "EmoController.h"
#include "MECclassifier.h"

//typedef boost::shared_ptr<simpleRunnable> ThreadedObject;
typedef boost::shared_ptr<AllegroFlasher> Flasher;
typedef boost::shared_ptr<Thread> CppThread;

/* Code derived from
    http://www.bogotobogo.com/cplusplus/multithreaded2.php
    http://www.computersciencelab.com/MultithreadingTut1.htm
*/

/* TO DO:
- Include zero padding functionality (ideally, an option)
- Include window selection functionality
- Include an averaging/low pass filter
- Include option to change the time delay
- Include Logger class to log the R value and accuracy values. Remove global variables
- Write tests 1-3 completely
*/


// Global variables for testing purposes
const int numChannels = 4;
int sampleSize = 512;
const int paddedSize = sampleSize + 256;
const unsigned int sampleFreq = 128;

double test1(boost::shared_ptr<EmoController> controller, Flasher flasher);
double test2(boost::shared_ptr<EmoController> controller, Flasher flasher);

extern bool testComplete;
extern double accuracy;

int main() {  
    const int width = 1024, height = 720;
    boost::shared_ptr<EmoController> controller(new EmoController(numChannels));
    Flasher SSVEPflasher(new AllegroFlasher(width, height));

    double test1Accuracy = test1(controller, SSVEPflasher);
    cout << endl << "Press any key to continue to next test" << endl;
    while (!_kbhit());
    double test2Accuracy = test2(controller, SSVEPflasher);
    
    cout << endl << "Test Results ==================================" << endl;
    cout << "test1Accuracy = " << test1Accuracy << endl;
    cout << "test2Accuracy = " << test2Accuracy << endl;
    controller->disconnectEmoEngine();
    
    system("PAUSE");
	return 0;
}

// THE FOLLOWING TESTS ARE OLD - WILL UPDATE THEM SOON...

/*  Test 1: MEC algorithm. Test finds the number of false positives for different frequencies
    Procedure:
        - Display a single non-flickering white block to the user for 30s
    Results:
        - False positive rate (number of false positives / total detections from 5s to 30s)
*/
double test1(boost::shared_ptr<EmoController> controller, Flasher flasher)
{
    const int freq = 15, time = 2;           // Hz and Seconds
    testComplete = false;                   // Global variable stored in EmoController
    bool useTestData = true;
    double SSVEPfreq = 15;

    // Create and start a new SSVEP classifier thread
    boost::shared_ptr<MECclassifier> myClassifier(new MECclassifier(sampleSize, sampleFreq, numChannels));
    controller->initClassifier(myClassifier, SSVEPfreq, useTestData);
    CppThread thread1(new Thread(controller));
    thread1->start(); 

    // Create and start a new flasher thread
    flasher->initFlasher(freq, time);
    CppThread thread2(new Thread(flasher));
    thread2->start(); 
    Sleep(10);  

    // Wait for completion of the flasher thread. Once complete, trigger the completion of the SSVEP thread.
    thread2->waitForCompletion();
    testComplete = true;
    thread1->waitForCompletion();

    return accuracy;                // Accuracy is a global variable changed by SSVEP once the test is complete
}

/*  Test 2: MEC algorithm. Finds the number of false negatives for different frequencies
    Procedure:
        - Display a single flickering block (alternating black and white) to the user for 30s
    Results:
        - Accuracy rate (number of correct detections / total detections from 5s to 30s)
*/
double test2(boost::shared_ptr<EmoController> controller, Flasher flasher)
{
    const int freq = 15, time = 2;           // Hz and Seconds
    const int width = 1024, height = 720;
    testComplete = false;                   // Global variable stored in EmoController
    bool useTestData = true;
    double SSVEPfreq = 10;

    // Create and start a new SSVEP classifier thread
    boost::shared_ptr<MECclassifier> myClassifier(new MECclassifier(sampleSize, sampleFreq, numChannels));
    controller->initClassifier(myClassifier, SSVEPfreq, useTestData);
    CppThread thread1(new Thread(controller));
    thread1->start(); 

    // Create and start a new flasher thread
    //Flasher flasher(new AllegroFlasher(freq, width, height, time));
    flasher->initFlasher(freq, time);
    CppThread thread2(new Thread(flasher));
    thread2->start(); 
    Sleep(10);

    // Wait for completion of the flasher thread. Once complete, trigger the completion of the SSVEP thread.
    thread2->waitForCompletion();
    testComplete = true;
    thread1->waitForCompletion();

    return accuracy;
}

