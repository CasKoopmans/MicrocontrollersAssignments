/*
 * Week3B1.c
 *
 * Created: 16/02/2017 11:40:02
 * Author : Duc
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

#define F_CPU 80000000;

//int main(void)
//{
	//init_lcd_4bit();
//
	//set_cursor(3);
	//display_text("Dit is");
    //while (1) 
    //{
    //}
//}

void init_lcd_4bit()
{
	// PORTC output mode and all low (also E and RS pin)
	DDRC = 0xFF;
	PORTC = 0x00;	// PORTC is on output mode.

	PORTC = 0x20;	// Sets the mode to 4 bits.
	lcd_strobe_lcd_e();

	PORTC = 0x20;   // Sets the mode to 4 bits.
	lcd_strobe_lcd_e();
	PORTC = 0x80;	
	lcd_strobe_lcd_e();

	PORTC = 0x00;   // Display on/off control
	lcd_strobe_lcd_e();
	PORTC = 0xF0;
	lcd_strobe_lcd_e();

	
	PORTC = 0x00;   // Entry mode set
	lcd_strobe_lcd_e();
	PORTC = 0x60;	// Sets the DDRAM adress to send and receive.
	lcd_strobe_lcd_e();

}

void display_text(char *str)
{

	while(*str)
	{
		lcd_write_data(*str++);
	}
}

void clearScreen()
{
	for(int i = 0; i < 16; i++)
	{
		display_text(" ");
	}
	lcd_write_command(0x80);
}

void set_cursor(int position)
{
	lcd_write_command(0x80);
	int currentLocation = 0;
	if(position != 0)
	{
		for(int i = 0; i < position; i++)
		{
			if(currentLocation != 15)
			{
					lcd_write_command(0x14);
					_delay_ms(5);
			}
			else if(currentLocation == 16)
			{
				lcd_write_command(0xA8);
				_delay_ms(5);
			}
			else if(currentLocation == 32)
			{
				lcd_write_command(0x80);
				currentLocation = 0;
				_delay_ms(5);
			}
			currentLocation++;
			_delay_ms(5);
		}
	}
}

void lcd_write_data(unsigned char byte)
{
	// First nibble.
	PORTC = byte;
	PORTC |= (1<<LCD_RS);
	lcd_strobe_lcd_e();

	// Second nibble
	PORTC = (byte<<4);
	PORTC |= (1<<LCD_RS);
	lcd_strobe_lcd_e();
}

void lcd_write_command(unsigned char byte)
{
	// First nibble.
	PORTC = byte;
	PORTC &= ~(1<<LCD_RS);
	lcd_strobe_lcd_e();

	// Second nibble
	PORTC = (byte<<4);
	PORTC &= ~(1<<LCD_RS);
	lcd_strobe_lcd_e();
}

void lcd_strobe_lcd_e()
{
	PORTC |= (1<<LCD_E);	// E high
	_delay_ms(1);			
	PORTC &= ~(1<<LCD_E);  	// E low
	_delay_ms(1);			
}




