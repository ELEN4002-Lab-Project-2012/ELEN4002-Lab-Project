#ifndef OWI535_H_INCLUDED
#define OWI535_H_INCLUDED
/** 
* @class OWI535
*
* @brief The OWI535 class procides the suer with a way to control the OWI535 USB arm. 
*
* The OWI535 class procides the suer with a way to control the OWI535 USB arm by passing it commands.
* The arm will continue to move in any direction specified by the commands untill the STOP command is called.
* This interface depdens heaviliy on the libUSB library.
*
* Cudo's to NotBrainSurgery for providing the USB protocol: http://notbrainsurgery.livejournal.com/
*
* 
*
* 
* @author Rudolf Hoehler
* 
* $Header $
*/

class OWI535
{
public:

	OWI535();
	~OWI535();

	bool armConnect();
	void armDisConnect();
	void searchlightOn ();

	void searchlightOff();

	void gripperOpen();
	void gripperClose();

	void wristUp();
	void wristDown();

	void elbowUp();
	void elbowDown();

	void shoulderUp();
	void shoulderDown();

	void baseClockwise();
	void baseCounterClockwise();

	void armStop();


private:
	
	int setupUsbAndConnect();
	int armExecute();
	void tearDownUsb();

	int moveResolution ;
	bool armIsLoaded;

	unsigned char robotCommand[3];

	#define ARM_VENDOR       0x1267
	#define ARM_PRODUCT      0
	#define CMD_DATALEN      3

	libusb_device **devs;
    libusb_device *dev;
	struct libusb_device_handle *devh;
	libusb_device * find_arm(libusb_device **devs);
};

#endif