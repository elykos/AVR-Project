/*
 * MyApp5.c
 *
 * Authors : Manolis Lykos, Argyriou Maria, Vardaxis Ioannis
 *
 * This project demonstrates the communication between an 
 * Android device and an AVR MCU through Bluetooth module
 * in order to control the movement of a robot car.
 *
 * This program's functionality is to initialize and handle
 * the MCU's UART communication module to establish commu-
 * nication between the Bluetooth module HC-06 and the MCU
 * and also, to move the robot car accordingly by manipulating
 * the available ports.
 *
 */ 

#include <avr/io.h>

#define F_CPU 1000000UL										// Define CPU clock Frequency 
#define BAUD_PRESCALE (((F_CPU / (BAUDRATE * 16UL))) - 1)	// Define prescale value


/*
 * Motor driver definition and functionality
 *
 * The microbot m2 motor control module uses 4 inputs to control the
 * robot car's motors. These inputs are named A1,A2,B1,B2 and their 
 * functionality is described as follows : Setting A1 module input 
 * rotates the car's left wheels forward whilst setting A2 input 
 * rotates the right wheels the same way. Setting B1 driver module 
 * input rotates the left wheels backwards whilst setting B2 rotates
 * the right wheels.
 *
 */
#define A1	PORTB0
#define B1	PORTB1
#define A2	PORTB2
#define B2	PORTB3

#define MOVEMENT_PORT PORTB
		
/* Initial declaration of USART module parameterization functions */
void USART_Init(unsigned long);				
char USART_RxChar();						
void USART_TxChar(char);					
void USART_SendString(char*);			

/* Initial declaration of movement related functions */
void moveForward();
void moveReverse();
void sharpRight();
void sharpLeft();	


int main(void)
{
	char Data_in;				// Variable that stores the received data
	DDRD &= ~(1<<PIND0);		// Set PORTD0 as input
	PORTD |= (1<<PIND0);		// Enable pull-up resistor for PORTD2
    DDRB = 0xff;				// Set entire PORTB as outputs			
	USART_Init(9600);			// Init UART as 9600 baud rate			
	PORTB = 0x00;				// Clear PORTB outputs
	
	OSCCAL = 0xB5;				// Set oscillator calibration value
	
	volatile int count=0;
	volatile int delayFlag=0;
	
	while(1)
	{
		/* Android device transmits continuously 0xff and 0x00 bytes
		 * in order to change the state of the demonstration leds.
		 * In order to demonstrate movement control from the device
		 * we use 4 leds, each one corresponding to a single motor
		 * driver input (A1,A2,B1,B2). Each time we switch from 0xff
		 * to 0x00 (or opposite from 0xff to 0x00) we cycle from one 
		 * movement state to another (cycling from all four and retu-
		 * rning to start). 
		 */
		
		Data_in = USART_RxChar();							// Receive data 
		if(Data_in == 0xff && delayFlag==0)
		{
			if(count%4==0)
			{
				moveForward();
				USART_SendString("ON");	
			}
			else if(count%4==1)
			{
				moveReverse();
				USART_SendString("ON");	
			}
			else if(count%4==2)
			{
				sharpRight();
				USART_SendString("ON");	
			}
			else if(count%4==3)
			{
				sharpLeft();
				USART_SendString("ON");	
			}
			
			count ++;
			
			delayFlag=1;
			
			//USART_SendString("ON");							
			
		}
		else if(Data_in ==0x00 && delayFlag==1)
		{
			PORTB = 0x00;							       //Reset ports
			USART_SendString("OFF"); 				
			delayFlag = 0;
		}
		else
			USART_SendString("Unknown command");	    
		
		
	}		
	return 0;
}

/*  Initialize UART module */
void USART_Init(unsigned long BAUDRATE)						// USART init
{
	UCSRB |= (1 << RXEN) | (1 << TXEN);						        // Enable USART transmitter and receiver 
	UCSRC |= (1 << URSEL)| (1 << UCSZ0) | (1 << UCSZ1);		// Set 8-bit cs and 1 stop bit
	UBRRL = BAUD_PRESCALE;									              // Set prescale low bits
	UBRRH = (BAUD_PRESCALE >> 8);					                // Set prescale high bits
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
	UDR = data;										
	while (!(UCSRA & (1<<UDRE)));					
}


/* Transmit string */
void USART_SendString(char *str)							//Send string of USART data function 
{
	int i=0;
	while (str[i]!=0)
	{
		USART_TxChar(str[i]);								      // Send each char of string till the NULL 
		i++;
	}
}


/* Command car to move forward */
void moveForward()
{
	MOVEMENT_PORT = (1<<A1 | 1<<A2);
}

/* Command car to move backwards */
void moveReverse()
{
	MOVEMENT_PORT = (1<<B1 | 1<<B2);
}

/* Command car to move left */
void sharpLeft()
{
	MOVEMENT_PORT = (1<<B1 | 1<<A2);
}

/* Command car to move right */
void sharpRight()
{
	MOVEMENT_PORT = (1<<A1 | 1<<B2);
}
