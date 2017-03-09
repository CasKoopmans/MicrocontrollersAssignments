/*
 * timer.c
 *
 *  Author: Cas Koopmans en Berend Vet
 */ 
  #define F_CPU 8000000

  #include <string.h>
  #include "timer.h"
  #include <avr/io.h>
  #include <util/delay.h>
  #include <avr/interrupt.h>

  #define COMPVALUE1 117
  #define COMPVALUE2 195

  void timer2initB3()
  {
	  //TIMSK |= (1<<TOIE2);
	  TIMSK |= 0b11111111;
	  TCNT2 = 0;
	  OCR2 = COMPVALUE1;
	  TCCR2 = 0b00011101;
  }

  ISR(TIMER2_COMP_vect)
  {
	  if(OCR2 == COMPVALUE1)
	  {
		  OCR2 = COMPVALUE2;
		  PORTC = 0b10000000;
	  }
	  else if(OCR2 == COMPVALUE2)
	  {
		  OCR2 = COMPVALUE1;
		  PORTC = 0b00000000;
	  }
  }