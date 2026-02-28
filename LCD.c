#include "LCD.h"
#include "stm32l476xx.h"

#define RS (1<<0)
#define E  (1<<1)
#define DB4 (1<<4)
#define DB5 (1<<5)
#define DB6 (1<<6) 
#define DB7 (1<<7)


void delay_ms(unsigned int ms) {
	//16000 cycles is 1 ms (hypothetically)
	//16 MHz clock frequency
	volatile unsigned int clkdiv = 0;
	while(clkdiv<(4000 * ms)) {
		clkdiv++;
	}
	
}

void LCD_WriteCom(unsigned char com) {
	GPIOB->ODR &= ~RS; //command mode
	
	PutNibble((com >> 4) & 0x0F); //send upper 4 bits of c
	LCD_Pulse();
	
	PutNibble(com & 0x0F); //clear upper 4 bits of c and send lower 4 bits
	LCD_Pulse();
	
	GPIOB->ODR |= RS; // return to data mode
}

void LCD_WriteData(unsigned char data) {
	GPIOB->ODR |= RS;
	
	PutNibble((data >> 4) & 0x0F); //send upper 4 bits of data
	LCD_Pulse();
	
	PutNibble(data & 0x0F); //clear upper 4 bits of data\ and send lower 4 bits
	LCD_Pulse();
	
}

void LCD_Init(void){
	//delay_ms(30);
	
	//enable gpio clocks 
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	//configure gpio to control 4 bit bus and E,RW,RS lines. 
	GPIOB->MODER &= ~(0xFFFF); //clear 16 least significant bits
	GPIOB->MODER |= (0x5555); //set pins 0-7 to output mode
	
	delay_ms(20);
//for loop that changes program to 8 bit data mode. 3 times  
	for(int idx = 0; idx <3; idx++){
		PutNibble(0x03);
		LCD_Pulse();
		delay_ms(5);		//Pause
	}
	
	//cursor starts at starting position. DDRAM address sets to 00H.
	PutNibble(0x02);
	LCD_Pulse();
	//pluse
	delay_ms(5);
	
	//send 4bit bus commands
	//5x8 dot matrix
	LCD_WriteCom(0x28);//2 line display mode
		//delay_ms(5);
	
		//clear display
	LCD_WriteCom(0x01);
		//delay_ms(5);
	
	//cursor off
	//cursor blink off
	LCD_WriteCom(0x0C);//display on
		//delay_ms(5);
	
	//cursor moves left to right,
	// no display scrolling
	LCD_WriteCom(0x06);
	//delay_ms(30);
	
	
	

	
	//CGRAM address starts at 0
	//LCD_WriteCom(0x40);
	//delay_ms(5);
}

void LCD_Clear(void){
  //clear LCD screen
	LCD_WriteCom(0x01);
	delay_ms(2);
}

void LCD_DisplayString(unsigned int line, unsigned char *ptr) {
	if(line == 0) {
		LCD_WriteCom(0x80); //write on first line
	} else {
		LCD_WriteCom(0xC0); //write on second line
	}
	
	for(int i = 0; i<16 && ptr[i] != '\0'; i++) {
		LCD_WriteData(ptr[i]);
	}
}

void PutNibble(unsigned char c){
	if(c & 0x8)
		GPIOB->ODR |= DB7;       
	else                       
		GPIOB->ODR &= ~DB7;      
														 
	if(c & 0x4)
		GPIOB->ODR |= DB6;
	else 
		GPIOB->ODR &= ~DB6;
	
	if(c & 0x2)
		GPIOB->ODR |= DB5;
	else 
		GPIOB->ODR &= ~DB5;
	
	if(c & 0x1)
		GPIOB->ODR |= DB4;
	else 
		GPIOB->ODR &= ~DB4; 
}

void LCD_Pulse(void) {

	GPIOB->ODR |= E;
	delay_ms(1);
	
	GPIOB->ODR &= ~E;
	delay_ms(1);

}

	

void System_Clock_Init(void){
	RCC->CR |= RCC_CR_HSION; //activate HSI clock 
	while((RCC->CR & RCC_CR_HSIRDY) == 0);
}