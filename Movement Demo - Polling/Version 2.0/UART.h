/* Functions used to control the MCU's UART module */


/* Initialize UART with specific baud rate, 8 bit frame, 1 stop bit */
void USART_Init(unsigned long);

/* Receive single character */
char USART_RxChar();

/* Transmit single character */
void USART_TxChar(char);

/* Transmit a string of characters */
void USART_SendString(char*);