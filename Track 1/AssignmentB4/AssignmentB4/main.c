/*
 * AssignmentB4.c
 *
 * Created: 09/02/2017 10:23:06
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
		 char standard = 0b00000001;
		 
		 while (1)
		 {
			 for(int i = 0; i < 8; i++){
				char light = standard << i;
				PORTD = light;
				wait(500);
			 }
			 for(int i = 6; i > 0; i--){
				 char light = standard << i;
				 PORTD = light;
				 wait(500);
			 }
		 }

		 return 1;
	 }
 }

