/*
 * Demo_Project.c
 *
 * Created: 20/10/2017 6:59:01 μμ
 * Authors : Argyriou Maria, Lykos Emmanouil, Vardaxis Ioannis
 *
 * This demo project uses some core AVR technologies and it's usage 
 * is destined for demonstration purposes.
 * 
 * This program's functionality is to read an external pin's value 
 * and time an output value for another pin of the same port accordingly.
 * PINB4 is used as an input for reading the value and PINB1 as the OUTPUT 
 * in which we use a LED. INT0 and INT1 interrupts are used as switches. 
 * When INT0 becomes grounded, PINB4 input value is read and if it's high
 * we time the output to be set after 5 seconds, or else if it's low, we 
 * time the output to be set after 1 second. INT1 is used to initialize 
 * the output when it get's grounded.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>


volatile uint8_t secondsFlag;			 //Global flag for delay selection


/* Interrupt service routine used to set the right flag for
 * the timer, so the timer can know when to time the output.
 * If low is read, flag (=int 1) is set and when high is
 * read flag (=int 2) is set. Also, counter is restarted.
 */ 
ISR(INT0_vect) {						 
	
	TCNT1 = 0;
	
	if (bit_is_clear(PINB, PINB4)) 
	{
		secondsFlag=1;		
	}
	else if (bit_is_set(PINB,PINB4))
	{
		secondsFlag=2;
	}
	
}

/* Interrupt service routine used to initalize the output 
 * when INT0 is pressed.
 */
ISR(INT1_vect){
	
	secondsFlag = 0;
	TCNT1 = 0;
	PORTB &= ~(1<<PORTB1);			//Clear output
	
}

/* Internal interrupt service triggered by the timer on 1sec
 * where if the flag is set, we set the output.
 */
ISR(TIMER1_COMPA_vect) {
	
	if(secondsFlag==1)
	{
		PORTB |= (1<<PORTB1);
		secondsFlag = 0;
	}
	
}

/* Internal interrupt service triggered by the timer on 5secs
 * where if the flag is set, we set the output.
 */
ISR(TIMER1_COMPB_vect) {
	
	if(secondsFlag==2)
	{
		PORTB |= (1<<PORTB1);
		secondsFlag = 0;
	}
	
}

/* Initalize the required interrupts */
void initInterrupts(void) {
	GICR  |= (1 << INT0 | 1<<INT1);         // Enable INT0 and INT1 
	MCUCR |= (1 << ISC01 | 1<<ISC11);		// Trigger on falling edge (we ground the pin on button press)	   
	sei();									// Set (global) interrupt enable bit 
}

/*Initialize the 16bit counter*/
void initTimer16(void) {
	
	TCCR1B |= (1<<CS12);					// Set prescaler value to 256 by changing the C(lock) S(ource) bit 
	TIMSK  |= (1<<OCIE1A | 1<<OCIE1B);		// Enable the OCR1A and OCR1B output compare registers by changing 
											// Output Compare Interrupt Enable bits on TIMSK register		   	
	TCNT1 = 0;								// Initialize counter value
	
	OCR1A = 3906;							// Set OCR1A to 1sec  (3906 counts on 256 prescale mode)		   
	OCR1B = 19531;							// Set OCR1A to 5secs (3906 counts on 256 prescale mode)		   
	
	sei();
	
}



int main(void) {
	
	DDRB = 0x0f;                            //Set PORTB0-3 as outputs and PORTB4-7 as inputs       
	PORTB = 0xf0;							//Enable pull-up resistors for PORTB4-7 inputs
	DDRD = 0x00;							//Configure PORTD as inputs
	PORTD = 0x00;							//Don't cares for the inputs we don't use
	PORTD = (1<<PORTD2 | 1<<PORTD3);		//Enable PORTD2 and PORTD3 pull-up resistors 
	
	secondsFlag = 0;
	
	initInterrupts();
	initTimer16();

	// ------ Event loop ------ //
	while (1) {
		

		}                                                  /* End event loop */
		return 0;                            /* This line is never reached */
	}

