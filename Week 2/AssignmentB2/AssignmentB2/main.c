/*
 * AssignmentB2.c
 *
 * Created: 09/02/2017 11:40:39
 * Author : Cas
 */ 

#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/interrupt.h>

void wait( int ms )
{
	for (int i=0; i<ms; i++)
	{
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

ISR( INT1_vect )
{
	if(PORTA != 0x80){
		PORTA <<= 1;
	}
	else{
		PORTA = 0x01;
	}
	wait(250);
}

	ISR( INT2_vect )
	{
		if(PORTA != 0x01){
			PORTA >>= 1;
		}
		else{
			PORTA = 0x80;
		}
		wait(250);
	}

int main( void )
{
	// Init I/O
	DDRD = 0xF9;			// PORTD 1 and PORTD 2 are input
	DDRA = 0xFF;			// PORTA is all output
	
	//start with 1 light on 0b00000001;
	PORTA = 0x01;

	// Init Interrupt hardware
	EICRA |= 0x0A;			// INT1 and INT2 falling edge	(00001010)
	EIMSK |= 0x06;			// Enable INT2 & INT1			(00000110)
	
	// Enable global interrupt system
	// SREG = 0x80;
	sei();

	while (1)
	{
		//Do nothing.....
	}

	return 1;
}

