/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BTN_Pin GPIO_PIN_13
#define BTN_GPIO_Port GPIOC
#define RCC_OSC32_IN_Pin GPIO_PIN_14
#define RCC_OSC32_IN_GPIO_Port GPIOC
#define RCC_OSC32_OUT_Pin GPIO_PIN_15
#define RCC_OSC32_OUT_GPIO_Port GPIOC
#define RCC_OSC_IN_Pin GPIO_PIN_0
#define RCC_OSC_IN_GPIO_Port GPIOF
#define RCC_OSC_OUT_Pin GPIO_PIN_1
#define RCC_OSC_OUT_GPIO_Port GPIOF
#define LIM_Z_NEG_Pin GPIO_PIN_0
#define LIM_Z_NEG_GPIO_Port GPIOC
#define X_CS_Pin GPIO_PIN_1
#define X_CS_GPIO_Port GPIOC
#define CARY_A_Pin GPIO_PIN_2
#define CARY_A_GPIO_Port GPIOC
#define CARY_B_Pin GPIO_PIN_3
#define CARY_B_GPIO_Port GPIOC
#define X_STEP_Pin GPIO_PIN_0
#define X_STEP_GPIO_Port GPIOA
#define Y_STEP_Pin GPIO_PIN_1
#define Y_STEP_GPIO_Port GPIOA
#define X_DIR_Pin GPIO_PIN_4
#define X_DIR_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_5
#define LED_GPIO_Port GPIOA
#define LIM_Y_POS_Pin GPIO_PIN_6
#define LIM_Y_POS_GPIO_Port GPIOA
#define LIM_X_NEG_Pin GPIO_PIN_7
#define LIM_X_NEG_GPIO_Port GPIOA
#define X_AWO_Pin GPIO_PIN_0
#define X_AWO_GPIO_Port GPIOB
#define Z_STEP_Pin GPIO_PIN_10
#define Z_STEP_GPIO_Port GPIOB
#define ESTOP_Pin GPIO_PIN_7
#define ESTOP_GPIO_Port GPIOC
#define LIM_Z_POS_Pin GPIO_PIN_8
#define LIM_Z_POS_GPIO_Port GPIOC
#define Z_DIR_Pin GPIO_PIN_8
#define Z_DIR_GPIO_Port GPIOA
#define Z_ENA_Pin GPIO_PIN_9
#define Z_ENA_GPIO_Port GPIOA
#define Y_DIR_Pin GPIO_PIN_10
#define Y_DIR_GPIO_Port GPIOA
#define T_SWDIO_Pin GPIO_PIN_13
#define T_SWDIO_GPIO_Port GPIOA
#define T_SWCLK_Pin GPIO_PIN_14
#define T_SWCLK_GPIO_Port GPIOA
#define T_SWO_Pin GPIO_PIN_3
#define T_SWO_GPIO_Port GPIOB
#define Y_CS_Pin GPIO_PIN_4
#define Y_CS_GPIO_Port GPIOB
#define Y_AWO_Pin GPIO_PIN_5
#define Y_AWO_GPIO_Port GPIOB
#define LIM_X_POS_Pin GPIO_PIN_6
#define LIM_X_POS_GPIO_Port GPIOB
#define LIM_Y_NEG_Pin GPIO_PIN_9
#define LIM_Y_NEG_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
