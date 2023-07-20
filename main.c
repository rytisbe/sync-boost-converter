/*  ------------------------------------------------------------------------------------------------------------------------------
    *** DEPENDANCIES ***
    ------------------------------------------------------------------------------------------------------------------------------ */
#include "stm32g0xx.h"
#include "main.h"

/*  ------------------------------------------------------------------------------------------------------------------------------
    *** OBJECT-LIKE MACROS ***
    ------------------------------------------------------------------------------------------------------------------------------ */
#define ON 1
#define OFF 0
/*  ------------------------------------------------------------------------------------------------------------------------------
    *** FUNCTION-LIKE MACROS ***
    ------------------------------------------------------------------------------------------------------------------------------ */

#define UPDATE_PWM        \
{                         \
  TIM1->CCR1 = gDutyCycle;\
}

/*  ------------------------------------------------------------------------------------------------------------------------------
    *** GLOBAL CONSTANTS ***
    ------------------------------------------------------------------------------------------------------------------------------ */

/*  ------------------------------------------------------------------------------------------------------------------------------
    *** LOOK-UP TABLES ***
    ------------------------------------------------------------------------------------------------------------------------------ */

/*  ------------------------------------------------------------------------------------------------------------------------------
    *** GLOBAL VARIABLES ***
    ------------------------------------------------------------------------------------------------------------------------------ */

/*  ------------------------------------------------------------------------------------------------------------------------------
    *** GENERAL PURPOSE REGISTERS [R5 - R11] for spped critical data operation ***
    ------------------------------------------------------------------------------------------------------------------------------ */
register int32_t gDutyCycle   __asm("r5");

char gError = 0;
uint8_t toggleVar = 0;

int main()
{
	InitSystemClock_PLL64M();
  InitTIM1_compl();
	
  gDutyCycle = 590; // 288 --> 50%
  
  UPDATE_PWM;
  
  GPIOA->MODER &= (uint32_t)0xFFFFFBFF; 
  GPIOA->MODER |= (uint32_t)0x00001400;
  
  TIM1->CR1 |= TIM_CR1_CEN;

	while (1)
	{       
//		while (t < 1000)
//    {
//      __nop();            // delay of a single instruction cycle
//      t++;      
//    }
//    gDutyCycle++;         // incrament duty ratio
//    if (gDutyCycle > 500)
//    {
//      gDutyCycle = 300;    // reset duty ratio
//    }
//    UPDATE_PWM;     
//    t = 0;                // reset counter
//    
	}
}

/*  ------------------------------------------------------------------------------------------------------------------------------
    FUNCTION DEFINITIONS
    ------------------------------------------------------------------------------------------------------------------------------ */

void InitSystemClock_PLL64M(void)
{   
	// upon system reset, the HSISYS clock derived from HSI16 oscillator is selected as system clock

	if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_1)	                // PLL is already used as system clock
	{                                                                 // if PLL reconfig is required this statement must be modified 
		return;                                               
	}
	if (!(PWR->CR1 & PWR_CR1_VOS_0))                                  // range 1 (high-performance) must be used for clock frequency of 64 MHz  
	{
		ErrorHandler();                                                 // sequence to go from range 2 to range 1 could be implemented instead 
	}  
	RCC->CR |= (RCC_CR_HSION);                                        // enable HSI16 clock 
	while (!(RCC->CR & RCC_CR_HSIRDY));                               // wait until HSI16 oscillator is stable

	FLASH->ACR &= ~(FLASH_ACR_LATENCY);                               // reset flash memory access latency
	FLASH->ACR |= FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_1;             // enable CPU prefetch | flash memory access latency = 2 WS (64 MHz)  	                  
	
	RCC->PLLCFGR = (uint32_t)(0x00000802);							              // PLL input clock source = HSI16 | M = *2 |  N = /8 --> 16*2/8 = 64 MHz
	RCC->CR |= RCC_CR_PLLON;                                          // enable the PLL 
	RCC->PLLCFGR |= (uint32_t)(0x11010000);                           // enable PLLRCLK  | PLLQCLK  | PLLPCLK clock output
	while(!(RCC->CR & RCC_CR_PLLRDY));                                // wait until PLL ready flag is set  
	RCC->CFGR &= ~(RCC_CFGR_SW);                                      // clear clock switch selection
	RCC->CFGR |= (RCC_CFGR_SW_1);			                                // switch to PLLRCLK
	while(!(RCC->CFGR & RCC_CFGR_SWS_1));                             // wait until PLLRCLK is switched on
	
	SystemCoreClockUpdate();											                    // update system clock
	SysTick_Config(SystemCoreClock);							                    // configure system timer		
  
}

void InitTIM1_compl(void)
{
  RCC->IOPENR   |= RCC_IOPENR_GPIOAEN;                              // enable clock for GPIOA port
  RCC->CCIPR    |= RCC_CCIPR_TIM1SEL;                               // PLLQCLK as clock source to TIM1
  RCC->APBENR2  |= RCC_APBENR2_TIM1EN;                              // enable clock for TIM1 peripheral
  
  RCC->APBRSTR2 |= RCC_APBRSTR2_TIM1RST;                 
  RCC->APBRSTR2 &= ~(RCC_APBRSTR2_TIM1RST);                         // reset TIM1 (RCC_APBRSTR2)
  
  TIM1->CR1     |= TIM_CR1_ARPE;                                    // TIM1_ARR register is buffered
  TIM1->CCER    |= TIM_CCER_CC1E | TIM_CCER_CC1NE;
  TIM1->CCMR1   |= TIM_CCMR1_OC1PE | (3U<<5);           // CC2 channel is configured as input, IC2 is mapped on TI2
  TIM1->ARR      = 640;
  TIM1->CCR1     = 320;
  TIM1->EGR      = TIM_EGR_UG;
  TIM1->BDTR     = (uint32_t)0x0200A020;                            // dead time = 32 PCLKs = 500ns
  TIM1->AF1     |= TIM1_AF1_BKINE;                                  // ??????????
  TIM1->AF2     |= TIM1_AF2_BK2INE;                                 // ??????????
  
  // configure PA7 = TIM1_CH1, PA8 = TIM1_CH1N
  GPIOA->MODER     &= (uint32_t)0xFFF00FFF;
  GPIOA->MODER     |= (uint32_t)0x00028000;                         // mode = AF
  //GPIOA->OSPEEDR  |= (uint32_t)0x0003C000;                        // speed = high speed
  GPIOA->AFR[0]   |= (uint32_t)0x20000000;                          // PA7 = AF2
  GPIOA->AFR[1]   |= (uint32_t)0x00000002;                          // PA8 = AF2
  
  //TIM1->CR1 |= TIM_CR1_CEN;                                       // enable TIM1 counter
}

void InitTIM2_trig(void)
{
  
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

