#ifndef __STM32L476R_NUCLEO_INTERRUPTS_H
#define __STM32L476R_NUCLEO_INTERRUPTS_H
#include <stdint.h>


void SysTick_Handler(void);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);

void buttonGPIOInit(void);
void SysTick_Init(uint32_t ticks);
void EXTI_Init(void);

extern volatile uint8_t current_hp;
#endif