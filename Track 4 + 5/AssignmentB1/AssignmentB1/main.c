/*
 * AssignmentB1.c
 *
 * Author : Cas Koopmans, Berend Vet
 */ 
#include <avr/io.h>

int main( void )
{
	DDRF = 0x00;				// set PORTF for input (ADC)
	DDRA = 0xFF;				// set PORTA for output
	DDRB = 0xFF;				// set PORTB for output

	ADMUX = 0b01100001;
	ADCSRA = 0b11100110;

	while (1)
	{
		PORTA = ADCH;
		PORTB = ADCL;
	}
}