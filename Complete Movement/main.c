/*
 * MyApp5.c
 *
 * Authors : Manolis Lykos, Argyriou Maria
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


volatile int delayFlag=0;

volatile int arriveFlag=0;
volatile char ReceivedByte;

/*
 * Interrupt Service routine that handles
 * the incoming data and sets the global flag
 * alowing the main loop to set the correct
 * command on the car 
 */
ISR(USART_RXC_vect)
{
	ReceivedByte = UDR;	//Store UDR data
	arriveFlag = 1;		//Set global flag
}



int main(void)
{
	char Data_in;				// Variable that stores the received data
	DDRD &= ~(1<<PIND0);		// Set PORTD0 as input
	PORTD |= (1<<PIND0);		// Enable pull-up resistor for PORTD2
        DDRB = 0xff;				// Set entire PORTB as outputs			
	USART_Init(4800);			// Init UART at 4800(x2) baud rate			
	PORTB = 0x00;				// Clear PORTB outputs
	
	OSCCAL = 0xB5;				// Set oscillator calibration value
	
	sei();					// Enable global interrupts
	
	
	while(1)
	{
		if(arriveFlag==1)
		{
			
			if(ReceivedByte==0x0f)		  //Backwards
			{
				moveReverse();
				arriveFlag = 0;
			}
			
			else if(ReceivedByte==0x07)  	  //Left
			{
				sharpLeft();
				arriveFlag = 0;
			}
			
			else if(ReceivedByte==0x03)       //Right
			{
				sharpRight();
				arriveFlag = 0;
			}
			
			else if(ReceivedByte==0x01)       //Forward
			{
				moveForward();
				arriveFlag = 0;
			}
			
			else if(ReceivedByte==0x00)       //Pause movement
			{
				PORTB = 0x00;
				arriveFlag = 0;
			}
			
			else				   //Do nothing
			{
				PORTB = PORTB;	
				arriveFlag = 0;
			}
			
		}
		
		UDR = 0x00;	//Clear UDR buffer
	}		
	return 0;
	
	
}
