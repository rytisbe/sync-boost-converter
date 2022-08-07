#include "stm32g0xx.h"

char error = 0;

void InitSystemClock_PLL64M(void);
void ErrorHandler(void);

int main()
{
	InitSystemClock_PLL64M();
	
	while (1)
	{
		__nop();
		__nop();
	}
}

void InitSystemClock_PLL64M(void)
{
	// upon system reset, the HSISYS clock derived from HSI16 oscillator is selected as system clock

	if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_1)	      // PLL is already used as system clock
	{                                                       // if PLL reconfig is required this statement must be modified 
		return;                                               
	}
	if (!(PWR->CR1 & PWR_CR1_VOS_0))                        // range 1 (high-performance) must be used for clock frequency of 64 MHz  
	{
		ErrorHandler();                                       // sequence to go from range 2 to range 1 could be implemented instead 
	}  
	RCC->CR |= (RCC_CR_HSION);                              // enable HSI16 clock 
	while (!(RCC->CR & RCC_CR_HSIRDY));                     // wait until HSI16 oscillator is stable

	FLASH->ACR &= ~(FLASH_ACR_LATENCY);                     // reset flash memory access latency
	FLASH->ACR |= FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_1;   // enable CPU prefetch | flash memory access latency = 2 WS (64 MHz)  	                  
	
	RCC->PLLCFGR = (uint32_t)(0x00000802);							    // PLL input clock source = HSI16 | M = *2 |  N = /8 --> 16*2/8 = 64 MHz
	RCC->CR |= RCC_CR_PLLON;                                // enable the PLL 
	RCC->PLLCFGR |= (uint32_t)(0x11010000);                 // enable PLLRCLK  | PLLQCLK  | PLLPCLK clock output
	while(!(RCC->CR & RCC_CR_PLLRDY));                      // wait until PLL ready flag is set  
	RCC->CFGR &= ~(RCC_CFGR_SW);                            // clear clock switch selection
	RCC->CFGR |= (RCC_CFGR_SW_1);			                      // switch to PLLRCLK
	while(!(RCC->CFGR & RCC_CFGR_SWS_1));                   // wait until PLLRCLK is switched on
	
	SystemCoreClockUpdate();											          // update system clock
	SysTick_Config(SystemCoreClock);							          // configure system timer		
}

void ErrorHandler(void)
{
	while (1)
	{
		/*
		- freeze the program execution due to an error
		- an appropriate handler could be implemented in a future
		*/
	}
}


