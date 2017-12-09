/* Implementation of UART protocol functions */

#define F_CPU 1000000UL										// Define CPU clock Frequency
#define BAUD_PRESCALE (((F_CPU / (BAUDRATE * 16UL))) - 1)	// Define prescale value

/*  Initialize UART module */
void USART_Init(unsigned long BAUDRATE)						// USART init
{
	UCSRB |= (1 << RXEN) | (1 << TXEN);						// Enable USART transmitter and receiver
	UCSRC |= (1 << URSEL)| (1 << UCSZ0) | (1 << UCSZ1);		// Set 8-bit cs and 1 stop bit
	UBRRL = BAUD_PRESCALE;									// Set prescale low bits
	UBRRH = (BAUD_PRESCALE >> 8);					        // Set prescale high bits
	UCSRA |= (1 << U2X);
	UCSRB |= (1 << RXCIE);
}

/* Receive single character */
char USART_RxChar()											//Receive 1 frame
{
	while (!(UCSRA & (1 << RXC)));
	return(UDR);
}

/* Single character transmission */
void USART_TxChar(char data)								//Transmit a single character
{
	while (!(UCSRA & (1<<UDRE)));
	UDR = data;
}


/* Transmit string */
void USART_SendString(char *str)							//Send string of USART data function
{
	int i=0;
	while (str[i]!=0)
	{
		USART_TxChar(str[i]);								// Send each char of string till the NULL
		i++;
	}
}