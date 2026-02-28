#include "stm32l476xx.h"
#include "LCD.h"
#include "system_stm32l4xx.h"
#include "interrupts.h"
#include "core_cm4.h"
#include "health_logic.h"

int main(void){

	System_Clock_Init();
	LCD_Init();
	LCD_Clear();
	buttonGPIOInit();
	SysTick_Init(400000);
	EXTI_Init();

	
	while(1);
}


