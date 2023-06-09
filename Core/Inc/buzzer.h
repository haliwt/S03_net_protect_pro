#ifndef __BUZZER_H_
#define __BUZZER_H_
#include "main.h"

#define BUZZER_PIN      GPIO_PIN_1
#define BUZZER_GPIO     GPIOB

extern IWDG_HandleTypeDef IWDG_Handler;


void Buzzer_KeySound(void);

void IWDG_Init(uint8_t prer,uint16_t rlr);
void IWDG_Feed(void);


#endif 
