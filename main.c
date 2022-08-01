#include "stm32g0xx.h"

void SystemClockInit(void);

int main()
{
	while (1)
	{
		SystemClockInit();
		// do nothing
	}

}

void SystemClockInit(void)
{
	// upon system reset, the HSISYS clock derived from HSI16 oscillator is selected as system clock

	if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_1)	
	{
		return;																								// PLL is already used as system clock
	}	
	
	RCC->CR &= (uint32_t)(~RCC_CR_PLLON);										// disable PLL
	while((RCC->CR & RCC_CR_PLLRDY) != 0){}									// wait till PLLRDY is cleared				
	RCC->PLLCFGR 	 = (uint32_t)(0x00000112);								// PLL input clock source = HSI16 | M = *2 |  N = /8 --> 16*2/8 = 64 MHz
	RCC->CR 			|= (uint32_t)(RCC_CR_PLLON);							// enable the PLL 
	RCC->PLLCFGR 	|= (uint32_t)(0x11010000);								// enable PLLRCLK  | PLLQCLK  | PLLPCLK clock output
	RCC->CFGR			|= (uint32_t)(RCC_CFGR_SW_1);							// use PLLRCLK as system clock
}

