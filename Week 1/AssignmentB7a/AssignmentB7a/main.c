/*
 * AssignmentB7a.c
 *
 * Created: 09/02/2017 10:30:52
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
		 DDRD = 0b11111111;			// All pins PORTD are set to output
		 DDRC = 0b00000000;			// All pins PORTC are set to input
		 
		 while (1)
		 {
			 if((PINC & 0b00000011) == 0b00000011){
				PORTD = 0b11000011;
				wait(500);
				PORTD = 0b00111100;
				wait(500);
			 }
			 else if(PINC & 0b00000001){
				PORTD = 0b11111111;
				wait(500);
				PORTD = 0b00000000;
				wait(500);
			 }
			 else if(PINC & 0b00000010){
				PORTD = 0b10101010;
				wait(500);
				PORTD = 0b01010101;
				wait(500);
			 }
			 else{
				PORTD = 0b11111111;
				wait(500);
				PORTD = 0b00000000;
				wait(500);
			 }
		 }

		 return 1;
	 }
 }

