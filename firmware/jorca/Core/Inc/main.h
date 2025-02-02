/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32h7xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define M1PM_Pin GPIO_PIN_2
#define M1PM_GPIO_Port GPIOE
#define M1DP_Pin GPIO_PIN_3
#define M1DP_GPIO_Port GPIOE
#define M1DM_Pin GPIO_PIN_4
#define M1DM_GPIO_Port GPIOE
#define M1EP_Pin GPIO_PIN_5
#define M1EP_GPIO_Port GPIOE
#define M1EM_Pin GPIO_PIN_6
#define M1EM_GPIO_Port GPIOE
#define M4PP_Pin GPIO_PIN_14
#define M4PP_GPIO_Port GPIOD
#define M4PM_Pin GPIO_PIN_15
#define M4PM_GPIO_Port GPIOD
#define M4DP_Pin GPIO_PIN_2
#define M4DP_GPIO_Port GPIOG
#define M4DM_Pin GPIO_PIN_3
#define M4DM_GPIO_Port GPIOG
#define M4EP_Pin GPIO_PIN_4
#define M4EP_GPIO_Port GPIOG
#define M4EM_Pin GPIO_PIN_5
#define M4EM_GPIO_Port GPIOG
#define LED_Pin GPIO_PIN_7
#define LED_GPIO_Port GPIOG
#define M3PP_Pin GPIO_PIN_3
#define M3PP_GPIO_Port GPIOD
#define M3PM_Pin GPIO_PIN_4
#define M3PM_GPIO_Port GPIOD
#define M3DP_Pin GPIO_PIN_5
#define M3DP_GPIO_Port GPIOD
#define M3DM_Pin GPIO_PIN_6
#define M3DM_GPIO_Port GPIOD
#define M3EP_Pin GPIO_PIN_7
#define M3EP_GPIO_Port GPIOD
#define M3EM_Pin GPIO_PIN_9
#define M3EM_GPIO_Port GPIOG
#define M2PP_Pin GPIO_PIN_10
#define M2PP_GPIO_Port GPIOG
#define M2PM_Pin GPIO_PIN_11
#define M2PM_GPIO_Port GPIOG
#define M2DP_Pin GPIO_PIN_12
#define M2DP_GPIO_Port GPIOG
#define M2DM_Pin GPIO_PIN_13
#define M2DM_GPIO_Port GPIOG
#define M2EP_Pin GPIO_PIN_14
#define M2EP_GPIO_Port GPIOG
#define M2EM_Pin GPIO_PIN_15
#define M2EM_GPIO_Port GPIOG
#define M1PP_Pin GPIO_PIN_8
#define M1PP_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
