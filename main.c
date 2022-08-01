#include "stm32g0xx.h"

void SystemClockInit(void);

int main(){
	SystemClockInit();
	// do nothing
}

void SystemClockInit(void){
	/*
	- upon system reset, the HSISYS clock derived from HSI16 oscillator is selected as system clock
	
	*/
	
	RCC->CR &= (uint32_t)(~RCC_CR_PLLON);								// disable PLL
	while((RCC->CR & RCC_CR_PLLRDY) != 0){}							// wait till PLLRDY is cleared				
	RCC->CFGR |= (uint32_t)(RCC_PLLCFGR_PLLM_0);				// PLL division factor M = 2
	RCC->CFGR |= (uint32_t)(RCC_PLLCFGR_PLLN_0);				// PLL multiplication factor N = 8	
	RCC->CR 	|= (uint32_t)(RCC_CR_PLLON);							// enable the PLL 
	RCC->PLLCFGR |= (uint32_t)(RCC_PLLCFGR_PLLREN);			// enable PLLRCLK
	RCC->CFGR	|= (uint32_t)(RCC_CFGR_SW_1);							// use PLLRCLK as system clock
		
}

