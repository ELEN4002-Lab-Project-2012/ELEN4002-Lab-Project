#include <p24F16KA102.h>
//#include <p24FJ64GB002.h>
#include <C:\Program Files\Microchip\MPLAB C30\include\string.h>
#include "UART.h"

//UART transmit function, parameter Ch is the character to send
void UART1transmit(char Ch)
{
	// Wait till TX buffer is empty
	while(U1STAbits.UTXBF == 1);
	
	// Then transmit the data
	U1TXREG = Ch;
	IFS0bits.U1TXIF = 0;	 	// reset TX interrupt flag (unnecessary)
}

void UART2transmit(char Ch)
{
	// Wait till TX buffer is empty
	while(U2STAbits.UTXBF == 1);
	
	// Then transmit the data
	U2TXREG = Ch;
	IFS1bits.U2TXIF = 0;	 	// reset TX interrupt flag (unnecessary)
}		

//UART receive function, returns the value received.
char UART1receive()
{
/*	char Temp = 0;
	
	//wait for buffer to fill up, wait for interrupt
	if (IFS0bits.U1RXIF == 1)
	{
		//(while(IFS0bits.U1RXIF == 0);
		Temp = U1RXREG;
		//reset interrupt
		IFS0bits.U1RXIF = 0;	

		return Temp;
	}
	else
		return Temp;		// Previous value */
	
	char Temp;
   	//wait for buffer to fill up, wait for interrupt
   	while(IFS0bits.U1RXIF == 0);
   	Temp = U1RXREG;
   	//reset interrupt
   	IFS0bits.U1RXIF = 0;
   	//return my received byte
   	return Temp;

}

char UART2receive()
{
/*	char Temp = 0;
	
	//wait for buffer to fill up, wait for interrupt
	if (IFS0bits.U1RXIF == 1)
	{
		//(while(IFS0bits.U1RXIF == 0);
		Temp = U1RXREG;
		//reset interrupt
		IFS0bits.U1RXIF = 0;	

		return Temp;
	}
	else
		return Temp;		// Previous value */
	
	char Temp;
   	//wait for buffer to fill up, wait for interrupt
   	while(IFS1bits.U2RXIF == 0);
   	Temp = U2RXREG;
   	//reset interrupt
   	IFS1bits.U2RXIF = 0;
   	//return my received byte
   	return Temp;

}

void sendBTCommand(const char* command)
{
	int i = 0;
	// Send each ASCII character in the string
	for(; i != strlen(command); i++)
		UART1transmit((char)(command[i]));	
}
