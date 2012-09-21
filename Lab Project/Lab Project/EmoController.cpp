#include "EmoController.h"

// Global variables for testing purposes
bool testComplete;
double accuracy;

EmoController::EmoController(int numChannels):      
    nChannels(numChannels),
    delayTime(200)              // Default d = 200ms
{
    initEmotiv();
    connectEmoEngine();
}

// The classifier is instantiated and connected to Emotiv once. 
// Thereafter, this function must be called to initialise the classifier for each test type
void EmoController::initClassifier(Classifier myClassifier, double detectionFreq, bool isTest)    
{
    classifier = myClassifier;
    SSVEPfreq = detectionFreq;
    test = isTest;
    totDetections = 0;
    positiveDetections = 0;
    accuracy = 0;
}

// Virtual Runnable function. This is where the thread starts execution.
void EmoController::run()
{
    loop();
}

// Set the delay time in milliseconds
void EmoController::setDelayTime(double time)
{
    if(time < 0)
        cerr << "Cannot set negative time" << endl;
    if(time > 1000)
        cerr << "Delay time too large" << endl;

    delayTime = time;
}

void EmoController::connectEmoEngine()
{
    std::cout << "===================================================================" << std::endl;
	std::cout << "Example to show how to log EEG Data from EmoEngine/EmoComposer."	   << std::endl;
	std::cout << "===================================================================" << std::endl;
	std::cout << "Press '1' to start and connect to the EmoEngine                    " << std::endl;
	std::cout << "Press '2' to connect to the EmoComposer                            " << std::endl;
	std::cout << ">> ";

    std::string input;
    std::getline(std::cin, input, '\n');
	option = atoi(input.c_str());

	switch (option) {
		case 1:
		{
			// Connect to the EmoEngine
			if (EE_EngineConnect() != EDK_OK) {
				throw std::exception("Emotiv Engine start up failed.");
			}
			break;
		}
		case 2:
		{
			// Connect to the EmoComposer 
			std::cout << "Target IP of EmoComposer? [127.0.0.1] ";
			std::getline(std::cin, input, '\n');

			if (input.empty()) {
				input = std::string("127.0.0.1");
			}

			if (EE_EngineRemoteConnect(input.c_str(), composerPort) != EDK_OK) {
				std::string errMsg = "Cannot connect to EmoComposer on [" + input + "]";
				throw std::exception(errMsg.c_str());
			}
			break;
		}
		default:
			throw std::exception("Invalid option...");
			break;
	}
}

void EmoController::initEmotiv()
{
    eEvent          = EE_EmoEngineEventCreate();
    eState          = EE_EmoStateCreate();
    hData           = EE_DataCreate();
	userID          = 0;
	composerPort    = 1726;
	secs            = 0.5;		                    // NB: initialise the buffer with 0.5*128 = 64 samples 	
	datarate        = 0;
	readytocollect  = false;
	option          = 0;
	state           = 0;
}

void EmoController::disconnectEmoEngine()
{
    EE_EngineDisconnect();
	EE_EmoStateFree(eState);
	EE_EmoEngineEventFree(eEvent);
}

// ###############
void EmoController::loop()
{   
    if(test) {
        classifier->loadTestData();
        cout << "Loaded test data" << endl;
    }
    
    cout << "Start receiving EEG Data! Press any key to stop logging...\n" << endl;
    EE_DataSetBufferSizeInSec(secs);		// Initialise the data buffer size in seconds

    // ###############
    // The loop is discontinued once the test is complete. testComplete is a global variable set by the main test thread
    while (!testComplete) {

		state = EE_EngineGetNextEvent(eEvent);		    // Get the EmoEvent

		if (state == EDK_OK) {						    // Operation has been carried out successfully (no error)

			EE_Event_t eventType = EE_EmoEngineEventGetType(eEvent);
			EE_EmoEngineEventGetUserId(eEvent, &userID);

			// Log the EmoState if it has been updated
			if (eventType == EE_UserAdded) {			// Note that an EE_UserAdded event is created soon after EE_EngineConnect
				std::cout << "User added" << endl << endl;
				EE_DataAcquisitionEnable(userID,true);	// Enable data acquisition
				readytocollect = true;					// Once a user has been added, read to collect data
			}

            // COGNITIV SUITE CODE WILL PROBABLY GO HERE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		}

        // Process raw EEG data...
		if (readytocollect) {
			processEEGdata(SSVEPfreq, test);
		}

		Sleep(delayTime);
	}
    accuracy = ((double)positiveDetections/(double)totDetections)*100;
    testComplete = false;
}

void EmoController::processEEGdata(double desiredFreq, bool test)
{    
    // Note that the developer’s application should access the EEG
	// data at a rate that will ensure the sample buffer is not overrun.
    // Ready the latest buffered data for access via the hData handle
    // All data captured since the last call to DataUpdateHandle will be retrieved.
	EE_DataUpdateHandle(0, hData);		

	unsigned int nSamplesTaken=0;                            // Number of NEW samples
                        
    // Establish the buffer size
    // The number of samples can be used to set up a buffer for retrieval into your application as shown
	EE_DataGetNumberOfSample(hData,&nSamplesTaken);
		
	std::cout << "Updated with " << nSamplesTaken << " samples. ";

	if (nSamplesTaken != 0 ) {
                
        cout << "Updating data" << endl;
        // ED_P7, ED_O1, ED_O2, ED_P8. For now this is just hard coded
        double* dataO1 = new double[nSamplesTaken];         // new data buffer initialised with sample size
        double* dataO2 = new double[nSamplesTaken];
        double* dataP7 = new double[nSamplesTaken];
        double* dataP8 = new double[nSamplesTaken];
                
        EE_DataGet(hData, ED_O1, dataO1, nSamplesTaken);
        EE_DataGet(hData, ED_O2, dataO2, nSamplesTaken);
        EE_DataGet(hData, ED_P7, dataP7, nSamplesTaken);
        EE_DataGet(hData, ED_P8, dataP8, nSamplesTaken);

        classifier->updateEEGData(dataO1, dataO2, dataP7, dataP8, nSamplesTaken);

        // #################
        totDetections++;
        if(classifier->isSSVEPpresent(desiredFreq)) {
            cout << "SSVEP found :-)" << endl;
            positiveDetections++;
        }             
        else
            cout << "SSVEP not found :-(" << endl;

		delete[] dataO1;
        delete[] dataO2;
        delete[] dataP7;
        delete[] dataP8;
	}

    // #################
    if(test)        // Called if we are using test data
    {
        totDetections++;
        if(classifier->isSSVEPpresent(desiredFreq)) {
            cout << "SSVEP found :-)" << endl;          
            positiveDetections++;
        }
        else
            cout << "SSVEP not found :-(" << endl;
    }
}



void EmoController::initChannels(int nNewSamples)
{
    for(int i = 0; i != nChannels; i++)
    {
        channels.push_back(new double[nNewSamples]);
    }
}