/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
#define WIFI_TX_Pin                 GPIO_PIN_2
#define WIFI_TX_GPIO_Port           GPIOA

#define WIFI_RX_Pin                 GPIO_PIN_3
#define WIFI_RX_GPIO_Port           GPIOA

#define PLASMA_Pin                  GPIO_PIN_5
#define PLASMA_GPIO_Port            GPIOA

#define FAN_CW_Pin                  GPIO_PIN_6
#define FAN_CW_GPIO_Port            GPIOA

#define FAN_CCW_Pin                 GPIO_PIN_7
#define FAN_CCW_GPIO_Port           GPIOA

#define RELAY_Pin                   GPIO_PIN_0
#define RELAY_GPIO_Port             GPIOB

#define TEMP_Pin                    GPIO_PIN_13
#define TEMP_GPIO_Port              GPIOB

#define ULTRASONIC_Pin              GPIO_PIN_8
#define ULTRASONIC_GPIO_Port        GPIOA

#define WIFI_EN_Pin                 GPIO_PIN_12
#define WIFI_EN_GPIO_Port           GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
