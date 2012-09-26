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
        ar & currentActionPower;
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
	float					currentActionPower;
	bool					CognitivIsActive;
	EE_EEG_ContactQuality_t ContactQ_O1;
	EE_EEG_ContactQuality_t	ContactQ_O2;
	EE_EEG_ContactQuality_t	ContactQ_P7;
	EE_EEG_ContactQuality_t	ContactQ_P8;
	int						HeadSetOn;


    eegTxRxData(){};
    eegTxRxData(
	EE_CognitivAction_t		_currentAction,
	float					_currentActionPower,
	bool					_CognitivIsActive,
	EE_EEG_ContactQuality_t _ContactQ_O1,
	EE_EEG_ContactQuality_t	_ContactQ_O2,
	EE_EEG_ContactQuality_t	_ContactQ_P7,
	EE_EEG_ContactQuality_t	_ContactQ_P8,
	int						_HeadSetOn ) :
	currentAction(_currentAction),
	currentActionPower(_currentActionPower),
	CognitivIsActive(_CognitivIsActive),
	ContactQ_O1(_ContactQ_O1),
	ContactQ_O2(_ContactQ_O2),
	ContactQ_P7(_ContactQ_P7),
	ContactQ_P8(_ContactQ_P8),
	HeadSetOn(_HeadSetOn)
	{}
};

#endif