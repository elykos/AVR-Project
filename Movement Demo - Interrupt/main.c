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
#include <avr/interrupt.h>
#include "movement.h"
#include "movement.c"
#include "UART.h"
#include "UART.c"


volatile int count=0;
volatile int delayFlag=0;


ISR(USART_RXC_vect)
{
	char ReceivedByte;
	ReceivedByte = UDR;
	
	if(ReceivedByte == 0xff && delayFlag==0)
	{
		if(count%4==0)
		{
			moveForward();
			//USART_SendString("ON");
		}
		else if(count%4==1)
		{
			moveReverse();
			//USART_SendString("ON");
		}
		else if(count%4==2)
		{
			sharpRight();
			//USART_SendString("ON");
		}
		else if(count%4==3)
		{
			sharpLeft();
			//USART_SendString("ON");
		}
		
		count ++;
		
		delayFlag=1;
		
		//USART_SendString("ON");
		
	}
	else if(ReceivedByte ==0x00 && delayFlag==1)
	{
		PORTB = 0x00;							       //Reset ports
		delayFlag = 0;
	}
	else
	UDR = 0x00;									  //Flush UDR register
}



int main(void)
{
	char Data_in;				// Variable that stores the received data
	DDRD &= ~(1<<PIND0);		// Set PORTD0 as input
	PORTD |= (1<<PIND0);		// Enable pull-up resistor for PORTD2
    DDRB = 0xff;				// Set entire PORTB as outputs			
	USART_Init(4800);			// Init UART as 9600 baud rate			
	PORTB = 0x00;				// Clear PORTB outputs
	
	OSCCAL = 0xB5;				// Set oscillator calibration value
	
	sei();						// Enable global interrupts
	
	
	while(1)
	{
		
	}		
	return 0;
	
	
}



