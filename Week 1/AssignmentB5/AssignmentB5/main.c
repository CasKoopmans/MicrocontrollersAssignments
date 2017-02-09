/*
 * AssignmentB5.c
 *
 * Created: 09/02/2017 10:26:17
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
		 int ar[] = {1, 2, 4, 8, 16, 32, 64, 128};
		 
		 while (1)
		 {
			 for(int i = 0; i < sizeof(ar); i++){
				 PORTD = ar[i];
				 wait(250);
				 if(i==7){
					i=-1;
				 }
			 }
		 }

		 return 1;
	 }
 }

