#ifndef eegTxRxData_H
#define eegTxRxData_H
#include"../include/EmoStateDLL.h"
#include <iostream>

#include <sstream>
#include <windows.h>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class eegTxRxData
{
private:
    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & currentAction;
		ar & currentUpperFaceAction;
		ar & currentLowerFaceAction;
		ar & currentActionPower;
		ar & currentUpperFaceActionPower;
		ar & currentLowerFaceActionPower;
        ar & CognitivIsActive;
        ar & ContactQ_O1;
        ar & ContactQ_O2;
        ar & ContactQ_P7;
        ar & ContactQ_P8;
        ar & HeadSetOn;

    }


	//Device state parameters

	//EmoState
	


public:
	EE_CognitivAction_t		currentAction;
	EE_ExpressivAlgo_t 		currentUpperFaceAction;
	EE_ExpressivAlgo_t 		currentLowerFaceAction;
	float					currentActionPower;
	float					currentUpperFaceActionPower;
	float					currentLowerFaceActionPower;
	bool					CognitivIsActive;
	EE_EEG_ContactQuality_t ContactQ_O1;
	EE_EEG_ContactQuality_t	ContactQ_O2;
	EE_EEG_ContactQuality_t	ContactQ_P7;
	EE_EEG_ContactQuality_t	ContactQ_P8;
	int						HeadSetOn;

	// Constructor
    eegTxRxData(){};
    eegTxRxData(
	EE_CognitivAction_t		_currentAction,
	EE_ExpressivAlgo_t 		_currentUpperFaceAction,
	EE_ExpressivAlgo_t 		_currentLowerFaceAction,
	float					_currentActionPower,
	float					_currentUpperFaceActionPower,
	float					_currentLowerFaceActionPower,
	bool					_CognitivIsActive,
	EE_EEG_ContactQuality_t _ContactQ_O1,
	EE_EEG_ContactQuality_t	_ContactQ_O2,
	EE_EEG_ContactQuality_t	_ContactQ_P7,
	EE_EEG_ContactQuality_t	_ContactQ_P8,
	int						_HeadSetOn ) :
	currentAction(_currentAction),
	currentUpperFaceAction(_currentUpperFaceAction),
	currentLowerFaceAction(_currentLowerFaceAction),
	currentActionPower(_currentActionPower),
	currentUpperFaceActionPower(_currentUpperFaceActionPower),
	currentLowerFaceActionPower(_currentLowerFaceActionPower),
	CognitivIsActive(_CognitivIsActive),
	ContactQ_O1(_ContactQ_O1),
	ContactQ_O2(_ContactQ_O2),
	ContactQ_P7(_ContactQ_P7),
	ContactQ_P8(_ContactQ_P8),
	HeadSetOn(_HeadSetOn)
	{}
};

#endif

/* Some fun Enum's that take forever to find
Expressiv facial expression type enumerator 

Enumerator: 
EXP_NEUTRAL   
EXP_BLINK   
EXP_WINK_LEFT   
EXP_WINK_RIGHT   
EXP_HORIEYE   
EXP_EYEBROW   
EXP_FURROW   
EXP_SMILE   
EXP_CLENCH   
EXP_LAUGH   
EXP_SMIRK_LEFT   
EXP_SMIRK_RIGHT   



*/