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
    SysTick_Init(400000); // Interrupt fires in the background
    EXTI_Init();

    Health_Init(); // 1. Start your software logic!

    char lcd_buffer[17] = {0}; // The scratchpad

    while(1) {
        // 2. Format the string based on current health
        Health_FormatDisplayString(lcd_buffer);
        
        // 3. Send it to the screen
        LCD_DisplayString(0, (unsigned char*)lcd_buffer);
        
        // 4. Give the screen a split-second to breathe before drawing again
        delay_ms(200); 
    }
}