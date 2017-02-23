/*
 * lcd.h
 *
 * Created: 23/02/2017 09:52:47
 *  Author: Cas
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LCD_E 	3
#define LCD_RS	2

int cursorpos;

void lcd_strobe_lcd_e(void);
void init_4bits_mode(void);
void lcd_write_string(char *str);
void lcd_write_data(unsigned char byte);
void lcd_write_cmd(unsigned char byte);
void display_text(char *str);
void set_cursor(int position);