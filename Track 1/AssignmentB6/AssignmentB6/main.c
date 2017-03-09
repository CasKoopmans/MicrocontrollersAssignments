/*
 * AssignmentB6.c
 *
 * Created: 09/02/2017 10:16:53
 * Author : Cas
 */ 

 #include <avr/io.h>
 #define F_CPU 8000000
 #include <util/delay.h>


void wait( int ms )
{
	for (int i=0; i<ms; i++)
	{
		_delay_ms( 1 );
	}

	int main(void)
	{
		int changed = 0;			// Tracks the status if frequency has been changed
		int freq = 1;				// Blinking frequency
		DDRD = 0b11111111;			// All pins PORTD are set to output
		DDRC = 0b00000000;			// All pins PORTC are set to input
		
		while (1)
		{
			if(PINC & 1){
				if(changed == 0 && freq == 1){
					changed = 1;
					freq = 4;
				}
				else if (changed == 0 && freq == 4){
					changed = 1;
					freq = 1;
				}
			}
			else{
				changed = 0;
			}
			PORTD = 0x80;
			wait(500/freq);
			PORTD = 0x0;
			wait(500/freq);
		}

		return 1;
	}
}

