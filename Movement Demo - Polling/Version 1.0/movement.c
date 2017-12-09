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