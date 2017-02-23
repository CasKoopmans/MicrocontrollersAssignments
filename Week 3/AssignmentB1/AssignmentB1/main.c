/*
 * AssignmentB1.c
 *
 * Created: 23/02/2017 09:51:22
 * Author : Cas
 */ 

#define F_CPU 8000000

#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void wait( int ms )
{
	for (int i=0; i<ms; i++)
	{
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}


int main(void)
{
	//Init I/O
	DDRD = 0xF8;			// PORTD(7) output, PORTD(6:0) input

	// Init LCD
	init_4bits_mode();

	// Write sample string
	lcd_write_string("Kevin is Gay");

	return 1;
}

