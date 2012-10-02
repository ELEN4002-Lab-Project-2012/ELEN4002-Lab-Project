#include <p24F16KA102.h>
//#include <p24FJ64GB002.h>
#include "ADCprocess.h"

// Main process function
// Return sampled value
unsigned int ADCprocess(void)
{
	unsigned int signal = 0;
	AD1CHS = 0x000B;				// Select AN12 as ADC input			

    AD1CON1bits.SAMP = 1;			// Now Start the ADC Sampling
	while (AD1CON1bits.DONE != 1);	// Wait for ADC conversion
	signal = ADC1BUF0;

	return signal;
}
