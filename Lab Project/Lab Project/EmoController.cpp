#include "EmoController.h"

// Global variables for testing purposes
bool testComplete;

EmoController::EmoController(int numChannels):      
    nChannels(numChannels),
    delayTime(200)              // Default d = 200ms
{   
    initEmotiv();
    connectEmoEngine();
}

// The classifier is instantiated and connected to Emotiv once. 
// Thereafter, this function must be called to initialise the classifier for each test type
void EmoController::initClassifier(Classifier myClassifier, Monitor myMonitor, double detectionFreq, bool isTest)    
{
    classifier = myClassifier;
    monitor = myMonitor;
    testComplete = false;
    SSVEPfreqs.clear();
    SSVEPfreqs.push_back(detectionFreq);
    test = isTest;
    totDetections.push_back(0);
    positiveDetections.push_back(0); 
}

// Override
void EmoController::initClassifier(Classifier myClassifier, Monitor myMonitor, vector<double> detectionFreqs, bool isTest)
{
    classifier = myClassifier;
    monitor = myMonitor;            //Monitor(new SSVEPMonitor(detectionFreqs));
    testComplete = false;
    SSVEPfreqs.clear();
    SSVEPfreqs = detectionFreqs;
    test = isTest; 
    //accuracy.clear();
    for(int i = 0; i != SSVEPfreqs.size(); i++) {
        totDetections.push_back(0);
        positiveDetections.push_back(0);
    }
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
	eProfile		= EE_ProfileEventCreate();
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

void EmoController::loop()
{       
    if(test) {
        classifier->loadTestData();
        cout << "Loaded test data" << endl;
    }
    
    EE_DataSetBufferSizeInSec(secs);		            // Initialise the data buffer size in seconds

    // The loop is discontinued once the test is complete. testComplete is a global variable set by the main test thread  
    while (!testComplete) 
    {
		state = EE_EngineGetNextEvent(eEvent);		    // Get the EmoEvent

		if (state == EDK_OK)                            // Operation has been carried out successfully (no error)
		{						    
			EE_Event_t eventType = EE_EmoEngineEventGetType(eEvent);
			EE_EmoEngineEventGetUserId(eEvent, &userID);

			// Log the EmoState if it has been updated
		    switch (eventType) 
		    {
			    case EE_UserAdded:				
			    {			// Note that an EE_UserAdded event is created soon after EE_EngineConnect
				    std::cout << "User added" << endl << endl;
				    EE_DataAcquisitionEnable(userID,true);	// Enable data acquisition
				    readytocollect = true;					// Once a user has been added, read to collect data
				    if (!LoadProfile())
					    {
						    std::cout << "++==Loading of user profile Failed!==++ \n";
						    std::cout << "Trying to do any cognitive stuff will fail stupidly! \n";	
					    }
						    for(int a=0; a < 10 ; a++)
						    {
							    cout << "*";
							    Sleep(200);
						    }
						    cout << "\n" ;    
				    }
			    case EE_EmoStateUpdated:
			    {	
				    EE_EmoEngineEventGetEmoState(eEvent, eState); 

				    EE_CognitivAction_t actionType;
				    actionType = ES_CognitivGetCurrentAction(eState);
				    float actionPower;
				    actionPower = ES_CognitivGetCurrentActionPower(eState);

				    if  (COG_NEUTRAL  == ES_CognitivGetCurrentAction(eState) )	cout << "NEUTRAL! \n";
				    if  (COG_LIFT  == ES_CognitivGetCurrentAction(eState) )	cout << "LIFTING! \n";
				    if  ( COG_PUSH == ES_CognitivGetCurrentAction(eState) )	cout << "PUSHING ! \n";
				    if  (COG_PULL  == ES_CognitivGetCurrentAction(eState) )	cout << "PULLING ! \n";
				    if  ( COG_DROP   == ES_CognitivGetCurrentAction(eState) )	cout << "DROPPING ! \n";
				    if  ( COG_LEFT == ES_CognitivGetCurrentAction(eState) )	cout << "LEFT ! \n";

				    HANDLE pipe = CreateFile(
					    "\\\\.\\pipe\\EEG_DATA_PIPE",
					    GENERIC_WRITE, // only need write access
					    FILE_SHARE_READ | FILE_SHARE_WRITE,
					    NULL,
					    OPEN_EXISTING,
					    FILE_ATTRIBUTE_NORMAL,
					    NULL
				    );
				    DWORD OhCrap;
				    if (pipe == INVALID_HANDLE_VALUE) {
					    wcout << "Failed to connect to pipe." << endl;
					    // look up error code here using GetLastError()
							
						    OhCrap = GetLastError();
						
				    }
				    else
				    {
					    //string strData;
					    //strData = "Rudolf";

					    //populate eegDATA IF the pipe could be created, otherwise, why bother?
					    eegTxRxData currentEEGdata(
						    ES_CognitivGetCurrentAction(eState),
						    ES_CognitivGetCurrentActionPower(eState),
						    ES_CognitivIsActive(eState),
						    ES_GetContactQuality(eState,EE_CHAN_O1),
						    ES_GetContactQuality(eState,EE_CHAN_O2),
						    ES_GetContactQuality(eState,EE_CHAN_P7 ),
						    ES_GetContactQuality(eState,EE_CHAN_P8 ),
						    ES_GetHeadsetOn(eState)
						    );
				
					    //Serialize
					    std::stringstream MyStringStream ;
					    boost::archive::text_oarchive oa(MyStringStream);
					    // write class instance to archive
					    oa << currentEEGdata;
					    DWORD numBytesWritten = 0;
					    BOOL result = WriteFile(
						    pipe, // handle to our outbound pipe
						    MyStringStream.str().c_str(), // data to send
						    MyStringStream.str().length(), // length of data to send (bytes)
						    &numBytesWritten, // will store actual amount of data sent
						    NULL // not using overlapped IO
						    );
					    CloseHandle(pipe);
				    }		
					    break;
			    }

				    // Handle Cognitiv training related event
			    case EE_CognitivEvent:
			    {
				    std::cout << "COGNITIVE EVENT";
				    //handleCognitivEvent(std::cout, eEvent);
				    break;
			    }
			    default:
				    break;
			}
        }
        if (readytocollect) {
            vector<double> Ratios;
		    for(int i = 0; i != SSVEPfreqs.size(); i++) 
            {	
                // Process raw EEG data. Return a ratio for each of the multiple frequencies.
                cout << "Frequency = " << SSVEPfreqs.at(i) << ": ";
                Ratios.push_back(processEEGdata(SSVEPfreqs.at(i), test));
                cout << "Ratio = " << Ratios.at(i) << endl;
            }
            // >>>>>>>>> HERE IS THE STRUCT <<<<<<<<<<<<
            FreqDetections detections;  // Struct storing bools for each frequency
            detections = monitor->monitorSSVEPDetections(Ratios, 0.95);     // SPECIFY THRESHOLD 0.95
		}
		Sleep(delayTime);
	}
    testComplete = false;
}

double EmoController::processEEGdata(double desiredFreq, bool test)
{    
    if(!test)
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

            delete[] dataO1;
            delete[] dataO2;
            delete[] dataP7;
            delete[] dataP8;
        }
    }
    return classifier->calculateRatio(desiredFreq);
}



void EmoController::initChannels(int nNewSamples)
{
    for(int i = 0; i != nChannels; i++)
    {
        channels.push_back(new double[nNewSamples]);
    }
}
//Rudolf was here
bool EmoController::LoadProfile()
{
	bool LoadSuccess = true ;

	if (EE_LoadUserProfile(userID,"RudolfHoehler.emuy")!= EDK_OK)
	{
		cout << "FAILED! to Load Profile from FILE!";
		LoadSuccess = false ;
	}
	else
	{
		cout << "Loaded Profile from FILE! GREAT SUCCESS";
		
	}
			
	if (EE_GetUserProfile(userID, eProfile) != EDK_OK) 
	{
		cout<< "Cound not load profile. \n";
		LoadSuccess = false ;
			
	}
	else
	{
		cout << "Got profile! \n";
	}


	unsigned int profileSize;
		
	if (EE_GetUserProfileSize(eProfile, &profileSize) != EDK_OK)
	{
		LoadSuccess = false ;

	}
	else
	{
		cout << "Got profile size! \n";
	}

	unsigned char* profileBuffer = NULL;
	profileBuffer = 	new unsigned char[profileSize];

		
	if (EE_GetUserProfileBytes(eProfile, profileBuffer, profileSize)!= EDK_OK )
	{
		cout << "Failed to get profile! \n";
		LoadSuccess = false ;

	}
	else
	{
		cout << "Succesfully got profile! \n";
	}

	if (EE_SetUserProfile(userID,profileBuffer, profileSize) != EDK_OK)
	{
		cout << "Failed to set profile! \n";
		LoadSuccess = false ;

	}
	else
	{
		cout << "Succesfully set profile! \n";
	}
				
	unsigned long   pTrainedActionsOut   ;

	if (EE_CognitivGetTrainedSignatureActions  ( userID, &pTrainedActionsOut )!= EDK_OK)
	{
		cout << "Failed to get trained actions! \n";
		LoadSuccess = false ;

	}
	else
	{
		cout << "Succesfully got trained actions! \n";
		cout << "This user has trained for: \n";
		float pActionSkillRatingOut ;

	//	EE_CognitivGetActionSkillRating  ( 0, 0x0010, & pActionSkillRatingOut   ) ;

		if( pTrainedActionsOut & 0x0001 ) cout <<" Neutral \n"; 
        if( pTrainedActionsOut & 0x0002 ) cout <<" Push \n"; 
        if( pTrainedActionsOut & 0x0004 ) cout <<" Pull \n"; 
        if( pTrainedActionsOut & 0x0008 ) cout <<" Lift \n"; 
        if( pTrainedActionsOut & 0x0010 ) cout <<" Drop \n"; 
        if( pTrainedActionsOut & 0x0020 ) cout <<" Left \n";
		if( pTrainedActionsOut & 0x0040 ) cout <<" Right \n";
		if( pTrainedActionsOut & 0x0100 ) cout <<" Rotate Left \n";
		if( pTrainedActionsOut & 0x0100 ) cout <<" Rotate Right \n";
		if( pTrainedActionsOut & 0x0200 ) cout <<" Rotate Clockwise \n";
		if( pTrainedActionsOut & 0x0400 ) cout <<" Rotate Counter Clockwise\n";
		if( pTrainedActionsOut & 0x0800 ) cout <<" Rotate Forwards \n";
		if( pTrainedActionsOut & 0x1000 ) cout <<" Rotate Reverse \n";
		if( pTrainedActionsOut & 0x2000 ) cout <<" Disappear \n";
				
					
	}

	return LoadSuccess ;
   
}