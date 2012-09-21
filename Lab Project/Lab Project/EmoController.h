#ifndef EMOCONTROLLER_H
#define EMOCONTROLLER_H

#include <iostream>
#include <fstream>
#include <conio.h>
#include <sstream>
#include <windows.h>
#include <map>
#include <string> 
#include <armadillo>

#include "Runnable.h"               // ################
#include "EmoStateDLL.h"
#include "edk.h"
#include "edkErrorCode.h"
#include "FFT.h"
#include "SSVEPclassifier.h"
#include <boost\shared_ptr.hpp>

typedef boost::shared_ptr<SSVEPclassifier> Classifier;

#pragma comment(lib, "../lib/edk.lib")   // Refer linker to the lib folder. 
using namespace std;
using namespace arma;

const char header[] = "COUNTER,AF3,F7,F3, FC5, T7, P7, O1, O2,P8" 
                      ", T8, FC6, F4,F8, AF4,GYROX, GYROY, TIMESTAMP, "   
                      "FUNC_ID, FUNC_VALUE, MARKER, SYNC_SIGNAL,";

class EmoController: public Runnable            // ################
{
public:
    EmoController(int numChannels);                                                                             // ################
    void loop();                                                                                                // ################
    void disconnectEmoEngine();                                                                                 // ################
    void initClassifier(Classifier myClassifier, double detectionFreq, bool isTest);    // ################
    virtual void run();
    void setDelayTime(double time);

private:
    void connectEmoEngine();
    void initEmotiv();
    void processEEGdata(double, bool);
    void initChannels(int nNewSamples);
    
    double delayTime;
    double SSVEPfreq;                           // ################
    bool test;                                  // ################
    EmoEngineEventHandle eEvent;		        // Event handler
	EmoStateHandle eState;				        // State handler
    DataHandle hData;                           // Data handler
	unsigned int userID;					
    unsigned short composerPort;	
	float secs;							        // Data buffer size in seconds
	unsigned int datarate;				
	bool readytocollect;					
	int option;							
	int state;
    Classifier classifier;
    int nChannels;
    vector<double*> channels;

    // Testing Data ################
    unsigned int totDetections;         // Record tot number of detections made through the session
    unsigned int positiveDetections;    // Record the number of positive detections made

};

#endif