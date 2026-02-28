#include "interrupts.h"
#include "stm32l476xx.h"
#include "LCD.h"
//#include "startup_stm321476xx.s"
//#include

#define pb8 (1<<8)
#define pb9 (1<<9)
#define pb10 (1<<10)

volatile int timerEnable = 0; //if 0, stop. if 1, go.
volatile int T; //tenths of a second
volatile int S1; //1s place of seconds
volatile int S2; //10s place of seconds
volatile int M1; //1s place of minutes
volatile int M2; //10s place of minutes
unsigned char time[7];

void SysTick_Init(uint32_t ticks) {
	SysTick->CTRL = 0; //disable systick irq and counter
	
	SysTick->LOAD = ticks - 1;
	
	NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1); //set sys tick to lowest priority
	
	//reset systick counter value
	SysTick ->VAL = 0;
	
	//select processor clock 
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk; //systick control register bit 2 goes high, for processor clock 
	
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; //counter counts down to 0 and makes excpetoin
	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //enable 
}

void EXTI_Init(void) {
	//enables clock to sysconfig
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	
	//connect exti 8 9 10 to gpiob 8 9 10
	SYSCFG->EXTICR[2] &= ~SYSCFG_EXTICR3_EXTI8;
	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PB;
	SYSCFG->EXTICR[2] &= ~SYSCFG_EXTICR3_EXTI9;
	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI9_PB;
	SYSCFG->EXTICR[2] &= ~SYSCFG_EXTICR3_EXTI10;
	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI10_PB;
	
//disable rising edge on exti 8 9 10
	EXTI->RTSR1 &= ~EXTI_RTSR1_RT8;
	EXTI->RTSR1 &= ~EXTI_RTSR1_RT9;
	EXTI->RTSR1 &= ~EXTI_RTSR1_RT10;
	
		//set exti 8 9 10 to falling edge trigger
	EXTI->FTSR1 |= EXTI_FTSR1_FT8;
	EXTI->FTSR1 |= EXTI_FTSR1_FT9;
	EXTI->FTSR1 |= EXTI_FTSR1_FT10;
	
	//enable exti interrupts on 8 9 10
	EXTI->IMR1 |= EXTI_IMR1_IM8;
	EXTI->IMR1 |= EXTI_IMR1_IM9;
	EXTI->IMR1 |= EXTI_IMR1_IM10;
	
	//set exti 8 9 10 priority to 1
	NVIC_SetPriority(EXTI9_5_IRQn, 2);
	NVIC_SetPriority(EXTI15_10_IRQn, 1);
	
	//enable interrups exti9 to 5 and exti 15 - 10
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void buttonGPIOInit(void) {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	GPIOB->MODER &= 0xFFC0FFFF; //clear pins 8 9 10, input mode
	GPIOB->PUPDR &= 0xFFC0FFFF; //clear pins 8 9 10
	GPIOB->PUPDR |= 0x00150000; //set pins 8 9 10 to pull up mode.
	
}

void SysTick_Handler(void) {

	if(timerEnable) {
		if(T == 9) {
			T = 0;
			if(S2 == 9) {
				S2 = 0;
				if(S1 == 5) {
					S1 = 0;
					if(M2 == 9) {
						M2 = 0;
						if(M1 == 9) {
							M1 = 0;
						} else M1++;
					} else M2++;
				} else S1++;
			} else S2++;
		} else T++;
		
	} //end if timer enable
	
	time[0] = M1 + 48; //convert integer to character
	time[1] = M2 + 48;
	time[2] = ':';
	time[3] = S1 + 48;
	time[4] = S2 + 48;
	time[5] = '.';
	time[6] = T + 48;
	LCD_DisplayString(0, time);
	
	
	
}
void EXTI9_5_IRQHandler(void)
{
	
	//if green button pressed, activate timer count
	if(!((GPIOB->IDR & pb8) == pb8))
		timerEnable = 1;
	
	//if yellow button pressed, stop timer count
	if(!((GPIOB->IDR & pb9) == pb9))
		timerEnable = 0;
	EXTI->PR1 |= EXTI_PR1_PIF8;
	EXTI->PR1 |= EXTI_PR1_PIF9;
}
void EXTI15_10_IRQHandler(void) {
	//if red button pressed, reset timer
	M1 = 0;
	M2 = 0;
	S1 = 0;
	S2 = 0;
	T = 0;
	EXTI->PR1 |= EXTI_PR1_PIF10;
};