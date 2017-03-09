/*
 * AssignmentB2.c
 *
 * Created: 23/02/2017 10:33:39
 * Author : Cas
 */ 

#define F_CPU 8000000

#include "lcd.h"
#include <stdio.h>
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
	int count = 0;
	// Init I/O
	DDRD = 0xF8;			// PORTD(7) output, PORTD(6:0) input

	// Init LCD
	init_4bits_mode();
	char s[15];
	sprintf(s, "%d", count);
	lcd_write_string(s);

	// Loop forever
	while (1)
	{
		if(PIND & 1)		//If PD0 is pressed, decrement the counter
		{
			count--;
			char str[15];
			sprintf(str, "%d", count);
			lcd_write_string(str);
			wait( 250 );
		}
		else if(PIND & 4)	//If PD2 is pressed, increment the counter
		{
			count++;
			char str[15];
			sprintf(str, "%d", count);
			lcd_write_string(str);
			wait( 250 );
		}
		// Write sample string
		PORTD ^= (1<<7);	// Toggle PORTD.7
		wait( 250 );
	}

	return 1;
}


