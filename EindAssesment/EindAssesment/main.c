/*
 * EindAssesment.c
 *
 * Created: 23/03/2017
 * Author : Cas Koopmans en Berend Vet
 */ 
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

static char tempToDisplay[15];
static int temp, wantedTemp;
static int oldPINC = 0;

#pragma region source functions
void twi_init(void)
{
	TWSR = 0;
	TWBR = 32;	 // TWI clock set to 100kHz, prescaler = 0
}
void twi_startup(void){
	twi_start();
	twi_tx(0xE0);	// Display I2C addres + R/W bit
	twi_tx(0x21);	// Internal osc on (page 10 HT16K33)
	twi_stop();
	twi_start();
	twi_tx(0xE0);	// Display I2C address + R/W bit
	twi_tx(0xA0);	// HT16K33 pins all output
	twi_stop();
	twi_start();
	twi_tx(0xE0);	// Display I2C address + R/W bit
	twi_tx(0xE3);	// Display Dimming 4/16 duty cycle
	twi_stop();
	twi_start();
	twi_tx(0xE0);	// Display I2C address + R/W bit
	twi_tx(0x81);	// Display OFF - Blink On
	twi_stop();
}
void twi_start(void)
{
	TWCR = (0x80 | 0x20 | 0x04);
	while( 0x00 == (TWCR & 0x80) );
}
void twi_stop(void)
{
	TWCR = (0x80 | 0x10 | 0x04);
}
void twi_tx(unsigned char data)
{
	TWDR = data;
	TWCR = (0x80 | 0x04);
	while( 0 == (TWCR & 0x80) );
}
void wait( int ms )
{
	for (int i=0; i<ms; i++)
	{
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}
#pragma endregion

#pragma region Animations
static int smiley[8] =		{0b11110011,0b11110011,0b11110011,0b00000000,0b00000000,0b11100001,0b00111111,0b00000000};

static int wink[][8] ={		{0b11110011,0b11110011,0b11110011,0b00000000,0b00000000,0b11100001,0b00111111,0b00000000},
							{0b01110000,0b11110011,0b11110011,0b00000000,0b00000000,0b11100001,0b00111111,0b00000000},
							{0b01110000,0b01110000,0b11110011,0b00000000,0b00000000,0b11100001,0b00111111,0b00000000},
							{0b01110000,0b11110011,0b11110011,0b00000000,0b00000000,0b11100001,0b00111111,0b00000000},
							{0b11110011,0b11110011,0b11110011,0b00000000,0b00000000,0b11100001,0b00111111,0b00000000}};

static int blink[][8] = {	{0b11110011,0b11110011,0b11110011,0b00000000,0b00000000,0b11100001,0b00111111,0b00000000},
							{0b00000000,0b11110011,0b11110011,0b00000000,0b00000000,0b11100001,0b00111111,0b00000000},
							{0b00000000,0b00000000,0b11110011,0b00000000,0b00000000,0b11100001,0b00111111,0b00000000},
							{0b00000000,0b11110011,0b11110011,0b00000000,0b00000000,0b11100001,0b00111111,0b00000000},
							{0b11110011,0b11110011,0b11110011,0b00000000,0b00000000,0b11100001,0b00111111,0b00000000}};

static int kiss[][8] = {	{0b11110011,0b11110011,0b11110011,0b00000000,0b00000000,0b11100001,0b00111111,0b00000000},
							{0b00000000,0b11110011,0b11110011,0b00000000,0b00000000,0b00100001,0b00011110,0b00000000},
							{0b00000000,0b11110011,0b11110011,0b00000000,0b00001100,0b00010010,0b00001100,0b00000000},
							{0b00000000,0b11110011,0b11110011,0b00000000,0b00000000,0b00100001,0b00011110,0b00000000},
							{0b11110011,0b11110011,0b11110011,0b00000000,0b00000000,0b11100001,0b00111111,0b00000000}};

static int angry[][8] = {	{0b11110011,0b11110011,0b11110011,0b00000000,0b00000000,0b11100001,0b00111111,0b00000000},
							{0b11100001,0b11110011,0b11110011,0b00000000,0b00000000,0b00000000,0b00111111,0b00000000},
							{0b00000000,0b11100001,0b11110011,0b00000000,0b00000000,0b00111111,0b11100001,0b00000000}};

static int eat[][8] =	{	{0b11110011,0b11110011,0b11110011,0b00000000,0b00000000,0b11100001,0b00111111,0b00000000},
							{0b11110011,0b11110011,0b11110011,0b00000000,0b00000000,0b00110011,0b00011110,0b00000000},
							{0b11110011,0b11110011,0b11110011,0b00000000,0b00001100,0b00010010,0b00001100,0b00000000},
							{0b11100001,0b11100001,0b00000000,0b00011110,0b00100001,0b00100001,0b00100001,0b00011110},
							{0b11000000,0b00011110,0b00100001,0b11000000,0b11000000,0b11000000,0b00100001,0b00011110},
							{0b00011110,0b00100001,0b11000000,0b11000000,0b11000000,0b11000000,0b00100001,0b00011110},
							{0b11000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11000000}};

#pragma endregion

#pragma region Standard display functions
void clearDisplay(){
	for(int i = 0; i<=14; i+=2){
		twi_start();
		twi_tx(0b11100000);	//display address
		twi_tx(i);			//row address
		twi_tx(0b00000000);	//row data
		twi_stop();
	}
}
void turnOnAllLeds(){
	for(int i = 0; i<=14; i+=2){
		twi_start();
		twi_tx(0b11100000);	//display address
		twi_tx(i);			//row address
		twi_tx(0b11111111);	//row data
		twi_stop();
	}
}
#pragma endregion

#pragma region Smiley functions
void DrawSmiley(){
	int i = 0;
	for(int j = 0; j<=14; j+=2){
		twi_start();
		twi_tx(0b11100000);	//display address
		twi_tx(j);			//row address
		twi_tx(smiley[i]);	//row data
		twi_stop();
		i++;
	}
}
void Wink(){
	for(int c = 0; c<5; c++){
		int i = 0;
		for(int j = 0; j<=14; j+=2){
			twi_start();
			twi_tx(0b11100000);	//display address
			twi_tx(j);			//row address
			twi_tx(wink[c][i]);	//row data
			twi_stop();
			i++;
		}
		wait(35);
	}
}
void Blink(){
	for(int c = 0; c<5; c++){
		int i = 0;
		for(int j = 0; j<=14; j+=2){
			twi_start();
			twi_tx(0b11100000);	//display address
			twi_tx(j);			//row address
			twi_tx(blink[c][i]);	//row data
			twi_stop();
			i++;
		}
		wait(35);
	}
}
void MakeAngry(){
	for(int c = 0; c<3; c++){
		int i = 0;
		for(int j = 0; j<=14; j+=2){
			twi_start();
			twi_tx(0b11100000);	//display address
			twi_tx(j);			//row address
			twi_tx(angry[c][i]);	//row data
			twi_stop();
			i++;
		}
		wait(35);
	}
	wait(700);
	for(int c = 2; c>=0; c--){
		int i = 0;
		for(int j = 0; j<=14; j+=2){
			twi_start();
			twi_tx(0b11100000);	//display address
			twi_tx(j);			//row address
			twi_tx(angry[c][i]);	//row data
			twi_stop();
			i++;
		}
		wait(35);
	}
}
void Kiss(){
	for(int c = 0; c<5; c++){
		int i = 0;
		for(int j = 0; j<=14; j+=2){
			twi_start();
			twi_tx(0b11100000);	//display address
			twi_tx(j);			//row address
			twi_tx(kiss[c][i]);	//row data
			twi_stop();
			i++;
		}
		wait(40);
	}
}
void Eat(){
	for(int c = 0; c<7; c++){
		int i = 0;
		for(int j = 0; j<=14; j+=2){
			twi_start();
			twi_tx(0b11100000);	//display address
			twi_tx(j);			//row address
			twi_tx(eat[c][i]);	//row data
			twi_stop();
			i++;
		}
		wait(50);
	}
	clearDisplay();
	wait(500);
	DrawSmiley();
}
#pragma endregion

#pragma region Events
int winkEvent(){
	return checkButton(1);
}
int blinkEvent(){
	return checkButton(2);
}
int makeAngryEvent(){
	return (checkButton(64) || temp>wantedTemp);
}
int kissEvent(){
	return checkButton(8);
}
int eatEvent(){
	return checkButton(128);
}
#pragma endregion

#pragma region Button functions
void initButtons(){
	DDRC = 0x00;			//Set all pins of PORTC as input
}

int checkButton(int button){
	int result = 0;
	if(PINC & button && !(oldPINC & button)){
		result = 1;
	}
	return result;
}
#pragma endregion

#pragma region Temperature functions
void initTemperature(){
	DDRF = 0x00;
	DDRA = 0xFF;

	ADMUX =  0b11100001;
	ADCSRA = 0b11100110;
	
	init_lcd_4bit();
	wait(300);
	
	clearScreen();
}

void displayTemperature(){
	clearScreen();
	temp = ADCH;
	sprintf(tempToDisplay, "%i°C", temp);
	wait(5);
	
	display_text(tempToDisplay);
	wait(5);
	memset(tempToDisplay, 0, 15);
	PORTA = ADCH;
}

#pragma endregion

#pragma region Demo function
void Demo(){
	Wink();
	wait(1000);
	Blink();
	wait(1000);
	MakeAngry();
	wait(1000);
	Kiss();
	wait(1000);
	Eat();
	wait(1000);
}
#pragma endregion

int main( void )
{
	//standard initialization
	twi_init();
	twi_startup();
	initButtons();
	initTemperature();

	//clearing the display and draw the standard emotion
	clearDisplay();
	DrawSmiley();

	//setting the temperature for angry to the current temperature +3
	wantedTemp = ADCH+3;

	while (1)
	{
		if(winkEvent()){
			Wink();
		}
		if(blinkEvent()){
			Blink();
		}
		if(makeAngryEvent()){
			MakeAngry();
		}
		if(kissEvent()){
			Kiss();
		}
		if(eatEvent()){
			Eat();
		}
		
		displayTemperature();
		oldPINC = PINC;
		wait(250);
	}
	return 1;
}

