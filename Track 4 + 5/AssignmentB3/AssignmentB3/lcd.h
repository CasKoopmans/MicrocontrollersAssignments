/*
 * lcd.h
 *
 * Created: 16/02/2017 11:42:31
 *  Author: Duc
 */ 


#ifndef LCD_H_
#define LCD_H_
#define LCD_RS 2
#define LCD_E 3

void init_lcd_4bit();
void display_text(char *str);
void set_cursor(int position);
void lcd_write_data(unsigned char byte);
void lcd_write_command(unsigned char byte);
void lcd_strobe_lcd_e();
void clearScreen(void);



#endif /* LCD_H_ */