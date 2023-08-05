#ifndef __FLASH_H_
#define __FLASH_H_
#include "main.h"

typedef enum{
   error,
   success

}flash_state_t;


void Flash_Erase_Data(void);
void Flash_Write_Data(void);
uint8_t Flash_Read_Data(void);


#endif 


