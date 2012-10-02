//UART transmit function
extern void  UARTtransmit(char Ch);

//UART receive function
extern char UART1receive();
extern char UART2receive();

//UART transmit functions
extern void UART1transmit(char Ch);
extern void UART2transmit(char Ch);


// Send command string
extern void sendBTCommand(const char* command);
