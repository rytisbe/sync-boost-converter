/*  ------------------------------------------------------------------------------------------------------------------------------
    *** PREPROCESSOR GUARDS ***
    ------------------------------------------------------------------------------------------------------------------------------ */

#ifndef MAIN_H
#define MAIN_H

/*  ------------------------------------------------------------------------------------------------------------------------------
    *** DEPENDANCIES ***
    ------------------------------------------------------------------------------------------------------------------------------ */

#include "stm32g0xx.h"

/*  ------------------------------------------------------------------------------------------------------------------------------
    *** FUNCTION PROTOTYPES ***
    ------------------------------------------------------------------------------------------------------------------------------ */

/**
*   @brief  Iinitialize system clock to 64 MHz using PLL
*   @note   function robustness can be improved (PLL reconfig option)
*/
void InitSystemClock_PLL64M(void);

/**
*   @brief  Initialize TIM1 for two complementary outputs with a DT insertion  
*   @note   Outputs are connected to PA7 and PA8
*/
void InitTIM1_compl(void);

/**
*   @brief  No description
*   @note   No notes 
*/
void InitTIM2_trig(void);

/**
*   @brief  No description
*   @note   No notes 
*/
void ErrorHandler(void);


#endif
