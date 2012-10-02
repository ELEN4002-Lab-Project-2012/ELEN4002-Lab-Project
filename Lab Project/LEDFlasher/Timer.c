#include <p24F16KA102.h>
//#include <p24FJ64GB002.h>
#include "Timer.h"
#include "ADCprocess.h"

 extern unsigned int Counter;

// Timer interrupt service routines

void _ISR _T1Interrupt(void)
{
	//PR1 = 200000*ADCprocess();		// Change the period depending on sampled value

	//Counter++;
	toggleRB15();
	IFS0bits.T1IF = 0;
	return;
}

void _ISR _T2Interrupt(void)
{
	toggleRB14();
	IFS0bits.T2IF 	= 0; 					// Reset Timer1 interrupt flag
}

void _ISR _T3Interrupt(void)
{
	toggleRB13();
	IFS0bits.T3IF 	= 0; 					// Reset Timer1 interrupt flag
}

void toggleRB15()
{
	if (LATBbits.LATB15 == 1)		// Toggle
		LATBbits.LATB15 = 0;
	else if (LATBbits.LATB15 == 0)
		LATBbits.LATB15 = 1;
}

void toggleRB14()
{
	if (LATBbits.LATB14 == 1)		// Toggle
		LATBbits.LATB14 = 0;
	else if (LATBbits.LATB14 == 0)
		LATBbits.LATB14 = 1;
}

void toggleRB13()
{
	if (LATBbits.LATB13 == 1)		// Toggle
		LATBbits.LATB13 = 0;
	else if (LATBbits.LATB13 == 0)
		LATBbits.LATB13 = 1;
}