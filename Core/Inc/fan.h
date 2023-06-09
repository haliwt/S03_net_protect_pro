#ifndef __FAN_H__
#define __FAN_H__
#include "main.h"


//#define FAN_CCW       GPIO_PIN_6
#define FAN_CW        GPIO_PIN_7
#define FAN_GPIO      GPIOA

//#define FAN_CCW_SetHigh()            HAL_GPIO_WritePin(FAN_GPIO,FAN_CCW,GPIO_PIN_SET)    // output high level
//#define FAN_CCW_SetLow()             HAL_GPIO_WritePin(FAN_GPIO,FAN_CCW,GPIO_PIN_RESET)    // output low level

#define FAN_CW_SetHigh()            HAL_GPIO_WritePin(FAN_GPIO,FAN_CW,GPIO_PIN_SET)    // output high level
#define FAN_CW_SetLow()             HAL_GPIO_WritePin(FAN_GPIO,FAN_CW,GPIO_PIN_RESET)    // output low level


#define PTC_PIN     GPIO_PIN_0
#define PTC_GPIO    GPIOB

#define PTC_SetHigh()            HAL_GPIO_WritePin(PTC_GPIO,PTC_PIN ,GPIO_PIN_SET)    // output high level
#define PTC_SetLow()             HAL_GPIO_WritePin(PTC_GPIO,PTC_PIN ,GPIO_PIN_RESET)    // output low level



#define BUZZER_SetHigh()            HAL_GPIO_WritePin(BUZZER_GPIO,BUZZER_PIN,GPIO_PIN_SET)    // output high level
#define BUZZER_SetLow()             HAL_GPIO_WritePin(BUZZER_GPIO,BUZZER_PIN,GPIO_PIN_RESET)    // output low level
#define BUZZER_TOGGLE()             HAL_GPIO_TogglePin(BUZZER_GPIO,BUZZER_PIN ) 




#define PLASMA_PIN      GPIO_PIN_5
#define PLASMA_GPIO     GPIOA

#define PLASMA_SetHigh()            HAL_GPIO_WritePin(PLASMA_GPIO,PLASMA_PIN,GPIO_PIN_SET)    // output high level
#define PLASMA_SetLow()             HAL_GPIO_WritePin(PLASMA_GPIO,PLASMA_PIN,GPIO_PIN_RESET)    // output low level





void FAN_Stop(void);
void SterIlization(uint8_t sel);





void ShutDown_AllFunction(void);
void Dry_Function(uint8_t sel);
void Fan_Slowly_Speed(void);
void Fan_One_Speed(void);


void Fan_Two_Speed(void);
void Fan_One_Power_Off_Speed(void);


void Fan_RunSpeed_Fun(void);
void Fan_Full_Speed(void);


#endif 
