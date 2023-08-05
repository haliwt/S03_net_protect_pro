#include "flash.h"
#include "run.h"



/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FLASH_USER_START_ADDR   (FLASH_BASE + (16 * FLASH_PAGE_SIZE))   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     (FLASH_USER_START_ADDR + FLASH_PAGE_SIZE)   /* End @ of user Flash area */

#define DATA_64                 ((uint64_t)0x1234567812345678)
#define DATA_32                 ((uint32_t)0x12345678)


uint32_t FirstPage = 0, NbOfPages = 0;
uint32_t Address = 0, PageError = 0;
__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;

/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;


static uint32_t GetPage(uint32_t Address);


void Flash_Erase_Data(void)
{
   /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();
  
  /* Erase the user Flash area
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/


  /* Get the 1st page to erase */
  FirstPage = GetPage(FLASH_USER_START_ADDR);

  /* Get the number of pages to erase from 1st page */
  NbOfPages = 1;//GetPage(FLASH_USER_END_ADDR) - FirstPage + 1;

  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Page        = FirstPage;
  EraseInitStruct.NbPages     = NbOfPages;

  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
  {
    /*
      Error occurred while page erase.
      User can add here some code to deal with this error.
      PageError will contain the faulty page and then to know the code error on this page,
      user can call function 'HAL_FLASH_GetError()'
    */
    /* Infinite loop */
    while (1)
    {
      /* Make LED0 blink (100ms on, 2s off) to indicate error in Erase operation */
       HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
    }
  }
}

void Flash_Write_Data(void)
{
	 HAL_FLASH_Unlock();
	/* Program the user Flash area word by word
		(area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/
	
	  Address = FLASH_USER_START_ADDR;
	
	  while (Address < FLASH_USER_END_ADDR)
	  {
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, DATA_64) == HAL_OK)
		{
		  Address = Address + 8;
		  run_t.flash_write_data_error = 0;
		}
	   else
		{
		  /* Error occurred while writing data in Flash memory.
			 User can add here some code to deal with this error */
		 // while (1)
		  {
			/* Make LED0 blink (100ms on, 2s off) to indicate error in Write operation */
		     //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
		     run_t.flash_write_data_error = 1;
		     
		  }
		}
	  }
	
	  /* Lock the Flash to disable the flash control register access (recommended
		 to protect the FLASH memory against possible unwanted operation) *********/
	  HAL_FLASH_Lock();

}


uint8_t  Flash_Read_Data(void)
{
   /* Check if the programmed data is OK
      MemoryProgramStatus = 0: data programmed correctly
      MemoryProgramStatus != 0: number of words not programmed correctly ******/
  Address = FLASH_USER_START_ADDR;
  MemoryProgramStatus = 0x0;

  while (Address < FLASH_USER_END_ADDR)
  {
    data32 = *(__IO uint32_t *)Address;

    if (data32 != DATA_32)
    {
      MemoryProgramStatus++;
    }
    Address = Address + 4;
  }

  /*Check if there is an issue to program data*/
  if (MemoryProgramStatus == 0)
  {
    /* No error detected. Switch on LED0*/
  	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
  	//run_t.flash_read_data_success = 1;
	//run_t.flash_read_data_error = 0;
	return 1;
  }
  else
  {
    /* Error detected. LED0 will blink with 1s period */
    //while (1)
    {
      //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
      //HAL_Delay(1000);
     // run_t.flash_read_data_error = 1;
	  //run_t.flash_read_data_success = 0;
	  return 0;
    }
  }



}

/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t Addr)
{
  return (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;;
}


