/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

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
#define h1_Pin GPIO_PIN_1
#define h1_GPIO_Port GPIOA
#define h2_Pin GPIO_PIN_2
#define h2_GPIO_Port GPIOA
#define h3_Pin GPIO_PIN_3
#define h3_GPIO_Port GPIOA
#define h4_Pin GPIO_PIN_4
#define h4_GPIO_Port GPIOA
#define h5_Pin GPIO_PIN_5
#define h5_GPIO_Port GPIOA
#define h6_Pin GPIO_PIN_6
#define h6_GPIO_Port GPIOA
#define h7_Pin GPIO_PIN_7
#define h7_GPIO_Port GPIOA
#define h8_Pin GPIO_PIN_8
#define h8_GPIO_Port GPIOA
#define h9_Pin GPIO_PIN_9
#define h9_GPIO_Port GPIOA
#define h10_Pin GPIO_PIN_10
#define h10_GPIO_Port GPIOA
#define h11_Pin GPIO_PIN_11
#define h11_GPIO_Port GPIOA
#define h12_Pin GPIO_PIN_12
#define h12_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
