/**
  ******************************************************************************
  * @file    Templates/Src/stm32f0xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/** @addtogroup STM32F0xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the Global MSP.
  * @param  None
  * @retval None
  */
void HAL_MspInit(void)
{
 
}

/**
  * @brief  DeInitializes the Global MSP.
  * @param  None  
  * @retval None
  */
void HAL_MspDeInit(void)
{

}

/**
  * @brief  Initializes the PPP MSP.
  * @param  None
  * @retval None

  */

// void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim) {
//   if (tim->Instance == TIM1)
//   __HAL_RCC_TIM1_CLK_ENABLE();
// }
// 
// void HAL_TIM_MspPostInit() {
//   //Initialize GPIO Resources 
//     GPIO_InitTypeDef sus;
//     __HAL_RCC_GPIOA_CLK_ENABLE();
//     sus.Pin = GPIO_PIN_8;
//     sus.Mode = GPIO_MODE_AF_PP;
//     sus.Speed = GPIO_SPEED_HIGH;
//     sus.Alternate = GPIO_AF2_TIM1;
//     sus.Pull = GPIO_PULLUP;
//     HAL_GPIO_Init(GPIOA, &sus);
// }


/*}*/

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


// Before PWM Init 
