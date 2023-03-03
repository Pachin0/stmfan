#include "main.h"
#include "Legacy/stm32_hal_legacy.h"
#include "stm32f031x6.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_def.h"
#include "stm32f0xx_hal_tim.h"
#include "stm32f0xx_hal_gpio.h"
#include "stm32f0xx_hal_gpio_ex.h" 
#include "stm32f0xx_hal_dma.h" 
#include "stm32f0xx_hal_msp.c"
#include "stm32f0xx_hal_rcc.h"
#include "stm32f0xx_nucleo_32.h"
#include <stdio.h>
#include <string.h>

struct TimPwm {
  TIM_TypeDef instance;
  uint32_t channels[];

};







  

