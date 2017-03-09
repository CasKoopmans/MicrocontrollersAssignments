/*
 * AssignmentB2.c
 *
 * Created: 09/03/2017 11:46:44
 * Author : Cas
 */ 
#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BIT(x)	(1 << (x))

void wait( int ms )
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}

int main( void )
{
	DDRF = 0x00;
	DDRA = 0xFF;

	ADMUX = 0b11100001;
	ADCSRA = 0b10000110;

	while (1)
	{
		ADCSRA |= BIT(6);
		while ( ADCSRA & BIT(6) );
		PORTA = ADCH;
		wait(500);
	}
}

