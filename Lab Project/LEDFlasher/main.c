/* LED Flasher code */

#include <p24F16KA102.h>
#include <libpic30.h>
#include "Timer.h"

// Configuration Bits - see p24F16KA102.h
_FBS(BWRP_OFF & BSS_OFF)
_FGS(GWRP_OFF & GCP_OFF)
_FOSCSEL(FNOSC_FRCPLL & IESO_OFF)		// NB: PLL is always 4x
_FOSC(FCKSM_CSECMD & POSCFREQ_MS & OSCIOFNC_ON & POSCMOD_NONE & SOSCSEL_SOSCLP)
_FWDT(FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768)
_FPOR(MCLRE_ON & BORV_LPBOR & BOREN_BOR3 & I2C1SEL_PRI & PWRTEN_OFF)
_FICD(BKBUG_OFF & ICS_PGx3)
_FDS(DSWDTEN_OFF & DSBOREN_ON & RTCOSC_SOSC & DSWDTOSC_SOSC & DSWDTPS_DSWDTPSF) 

/*_CONFIG1(GWRP_OFF & GCP_OFF & FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768)
_CONFIG2(FNOSC_FRCPLL & IESO_OFF & FCKSM_CSECMD & OSCIOFNC_ON & POSCMOD_NONE)
_CONFIG4(DSWDTEN_OFF & DSBOREN_ON)*/

void setupPIC();
int timeConst = 7813;

int main(void)
{	
	setupPIC();
	while(1);
	return(0);
}

void setupPIC(void)
{
	// -------------------- Set up Ports --------------------
	TRISB	= 0x0000;					// All ports output.

	// -------------------- Set up Clock --------------------
	/*	Our new clock frequency is thus Fcy = 8HHz * 4 / 2 = 16Mhz
	So we operate at 16 MIPs
	(i.e. 4k instruction per 1ms sample period) */
	OSCCON	=	0b0001000110100000;
	CLKDIV	=	0;

	/* -------------------- Set up Timer1 ----------------------
	   Calculations:
		Oscillator: 8Mhz RC, 4xPLL. Thus, Fcy = 8*4/2 = 16Mhz
		Timer 1 prescaler 1:8
		PR1 = 2000
		Therefore period = 521 * 2 * 1/(8000000) * 256 = 3.33ms i.e. total freq = 14.9Hz	 */
	TMR1			=	0x0000;					// Clear contents of TMR1 register
	PR1				=	timeConst/12;		
	IPC0bits.T1IP 	= 	5;						// set interrupt priority high
	T1CON			= 	0b0000000000110000;		// turn OFF the timer, prescale 1:8
	IFS0bits.T1IF 	= 	0;	 					// reset interrupt flag
	IEC0bits.T1IE 	= 	1;	 					// turn on the timer1 interrupt
	T1CONbits.TON = 1;

	// -------------------- Set up Timer2 ----------------------
	// 11 Hz
	TMR2			=	0x0000;					// Clear contents of TMR3 register
	PR2				=	timeConst/11;					// Max. Interrupts if no INT1 interrupt occurs
	IPC1bits.T2IP 	= 	4;						// High Priority
	T2CON			= 	0b0000000000110000; 	// turn OFF the timer, prescale 1:8
	IFS0bits.T2IF 	= 	0;	 					// reset interrupt flag
	IEC0bits.T2IE 	= 	1;
	T2CONbits.TON = 1;
	
	// -------------------- Set up Timer3 ----------------------
	// 10 Hz
	TMR3			=	0x0000;					// Clear contents of TMR3 register
	PR3				=	timeConst/10;					// Max. Interrupts if no INT1 interrupt occurs
	IPC2bits.T3IP 	= 	4;						// High Priority
	T3CON			= 	0b0000000000110000; 	// turn OFF the timer, prescale 1:8
	IFS0bits.T3IF 	= 	0;	 					// reset interrupt flag
	IEC0bits.T3IE 	= 	1;
	T3CONbits.TON = 1;
}