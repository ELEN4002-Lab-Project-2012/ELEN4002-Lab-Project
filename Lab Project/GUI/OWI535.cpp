#include "StdAfx.h"


OWI535::OWI535 ()
{
	
	

}
OWI535::~OWI535()
{
	tearDownUsb();

}

bool OWI535::armConnect()
{
	if ( setupUsbAndConnect() != -1 )
	{

		robotCommand[0]=(unsigned char)strtol("00",NULL,16);
		robotCommand[1]=(unsigned char)strtol("00",NULL,16);
		robotCommand[2]=(unsigned char)strtol("00",NULL,16);

		armExecute();
		armIsLoaded = true;
	return true;
	}
	else
	{
		armIsLoaded = false;
		return false;
	}
}

void OWI535::armDisConnect()
{
	tearDownUsb() ;
	

}


int OWI535::setupUsbAndConnect()
{

	try
	{
		devh = NULL;
		int r;
		ssize_t cnt;

		r = libusb_init(NULL);
		if (r < 0)
		{
			fprintf(stderr, "failed to initialize libusb\n");
			return r;
		}

		libusb_set_debug(NULL,2);
    
		cnt = libusb_get_device_list(NULL, &devs);
		if (cnt < 0)
			return (int) cnt;
		dev=find_arm(devs);
		if(!dev)
		{
			fprintf(stderr, "Robot Arm not found\n");
			return -1;
		}

		r = libusb_open(dev,&devh);
		if(r!=0)
		{
			fprintf(stderr, "Error opening device\n");
       			libusb_free_device_list(devs, 1);
					libusb_exit(NULL);
			return -1;
		}

	}
	catch(int e)
	{
		// MessageBox::Show("The calculations are complete", "My Application", MessageBoxButtons::OKCancel,
      //   MessageBoxIcon::Asterisk);
	 MessageBoxA(NULL,"The USB arm could not be connected!", "Oh! Crap!", MB_OK);
		return -1;
	}

}

libusb_device * OWI535::find_arm(libusb_device **devs)
{
	libusb_device *dev;
	int i = 0;

	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0) {
			fprintf(stderr, "failed to get device descriptor");
			return NULL;
		}
		if(desc.idVendor == ARM_VENDOR &&
		   desc.idProduct == ARM_PRODUCT)
		  {
		    return dev;
		  }
	}
	return NULL;
}

void OWI535::tearDownUsb()
{
	libusb_close(devh);
	libusb_free_device_list(devs, 1);
	if (armIsLoaded)
	{
		libusb_exit(NULL);
	}
}

int OWI535::armExecute()
{
	int result;
	result = libusb_control_transfer(devh,
                                0x40, //uint8_t 	bmRequestType,
                                6, //uint8_t 	bRequest,
                                0x100, //uint16_t 	wValue,
                                0,//uint16_t 	wIndex,
                                robotCommand,
                                CMD_DATALEN,
                                0	 
    );

	return result;
	
}

void OWI535::searchlightOn()
{
	
	robotCommand[2]=(unsigned char)strtol("01",NULL,16);

	armExecute() ;


}

void OWI535::searchlightOff()
{
	
	robotCommand[2]=(unsigned char)strtol("00",NULL,16);
	
	armExecute() ;

}

void OWI535::armStop()
{
	robotCommand[0]=(unsigned char)strtol("00",NULL,16);
    robotCommand[1]=(unsigned char)strtol("00",NULL,16);
	//robotCommand[2]=(unsigned char)strtol("00",NULL,16);
	
	armExecute() ;

}

void OWI535::gripperOpen()
{
	robotCommand[0]=(unsigned char)strtol("02",NULL,16);
    
	
	armExecute() ;

}

void OWI535::gripperClose()
{
	robotCommand[0]=(unsigned char)strtol("01",NULL,16);
   
	armExecute() ;

}

void OWI535::wristUp ()
{
	robotCommand[0]=(unsigned char)strtol("04",NULL,16);
 
	
	armExecute() ;

}

void OWI535::wristDown ()
{
	robotCommand[0]=(unsigned char)strtol("08",NULL,16);
    
	
	armExecute() ;

}

void OWI535::elbowUp()
{
	robotCommand[0]=(unsigned char)strtol("10",NULL,16);
   
	
	armExecute() ;

}

void OWI535::elbowDown ()
{
	robotCommand[0]=(unsigned char)strtol("20",NULL,16);
   
	
	armExecute() ;

}

void OWI535::shoulderUp()
{
	robotCommand[0]=(unsigned char)strtol("40",NULL,16);
    
	armExecute() ;

}

void OWI535::shoulderDown ()
{
	robotCommand[0]=(unsigned char)strtol("80",NULL,16);
   
	
	armExecute() ;

}

void OWI535::baseClockwise ()
{
	
    robotCommand[1]=(unsigned char)strtol("01",NULL,16);

	
	armExecute() ;

}

void OWI535::baseCounterClockwise ()
{
	
    robotCommand[1]=(unsigned char)strtol("02",NULL,16);

	
	armExecute() ;

}

