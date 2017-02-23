/*
 * lcd.c
 *
 * Created: 23/02/2017 09:52:30
 *  Author: Cas
 */ 

#define F_CPU 8000000

#include "lcd.h"

cursorpos = 0;

void lcd_strobe_lcd_e(void)
{
	PORTC |= (1<<LCD_E);	// E high
	_delay_ms(1);
	PORTC &= ~(1<<LCD_E);  	// E low
	_delay_ms(1);
}

void init_4bits_mode(void)
{
	// PORTC output mode and all low (also E and RS pin)
	DDRC = 0xFF;
	PORTC = 0x00;

	// Step 2 (table 12)
	PORTC = 0x20;	// function set
	lcd_strobe_lcd_e();

	// Step 3 (table 12)
	PORTC = 0x20;   // function set
	lcd_strobe_lcd_e();
	PORTC = 0x80;
	lcd_strobe_lcd_e();

	// Step 4 (table 12)
	PORTC = 0x00;   // Display on/off control
	lcd_strobe_lcd_e();
	PORTC = 0xF0;
	lcd_strobe_lcd_e();

	// Step 4 (table 12)
	PORTC = 0x00;   // Entry mode set
	lcd_strobe_lcd_e();
	PORTC = 0x60;
	lcd_strobe_lcd_e();
}

void lcd_write_string(char *str)
{
	lcd_write_command(0x01);
	lcd_write_command(0x02);
	while(*str)
	{
		lcd_write_data(*str++);
		cursorpos++;
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

void display_text(char *str)
{
	lcd_write_command(0x01);
	lcd_write_command(0x02);
	while(*str)
	{
		lcd_write_data(*str++);
	}
}

void set_cursor(int position)
{
	if(cursorpos==position)
	{
		
	}
	else if(cursorpos>position)
	{
		while (cursorpos>position)
		{
			lcd_write_command(0x10);
			cursorpos--;
		}
	}
	else if(cursorpos<position)
	{
		while (cursorpos<position)
		{
			lcd_write_command(0x14);
			cursorpos++;
		}
	}
}