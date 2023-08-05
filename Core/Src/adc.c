/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */
#include "buzzer.h"
#include "run.h"
#include "fan.h"
#include "mqtt_iot.h"
#include "cmd_link.h"
#include "publish.h"
#include "delay.h"
#include "publish.h"
#include "subscription.h"

#define MODIFICATION_VALUE              110



static  uint16_t Get_Adc_Average(uint32_t ch,uint8_t times);
static uint16_t Get_Adc_Channel(uint32_t ch)  ;
static void Judge_PTC_Temperature_Value(void);

uint8_t detect_error_times=0;
volatile uint16_t  fan_detect_voltage;


/* USER CODE END 0 */
uint32_t temp_value ;

uint16_t adcx;

ADC_HandleTypeDef hadc1;


/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.LowPowerAutoPowerOff = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_1CYCLE_5;
  hadc1.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_1CYCLE_5;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
}

//{
//  /** Configure Regular Channel
//  */
//  sConfig.Channel = run_t.ADC_channel_No; //ADC_CHANNEL_0;
//  sConfig.Rank = ADC_REGULAR_RANK_1;
//  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
//  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN ADC1_Init 2 */
//
//  /* USER CODE END ADC1_Init 2 */
//
//}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* ADC1 clock enable */
    __HAL_RCC_ADC_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA0     ------> ADC1_IN0
    PA1     ------> ADC1_IN1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PA0     ------> ADC1_IN0
    PA1     ------> ADC1_IN1
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_1);

  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/*****************************************************************
*
	*Function Name: static uint16_t Get_Adc(uint32_t ch)  
	*Function ADC input channel be selected "which one channe"
	*Input Ref: which one ? AC_Channel_?
	*Return Ref: No
	*
	*
*****************************************************************/
static uint16_t Get_Adc_Channel(uint32_t ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;

	ADC1_ChanConf.Channel=ch;                                   //Í¨µÀ
    ADC1_ChanConf.Rank= ADC_REGULAR_RANK_1;                                    //第一个序列
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_1CYCLE_5;//ADC_SAMPLETIME_239CYCLES_5;      //²ÉÑùÊ±¼ä               


	HAL_ADC_ConfigChannel(&hadc1,&ADC1_ChanConf);        //Í¨µÀÅäÖÃ
	
    HAL_ADC_Start(&hadc1);                               //start ADC transmit
	
    HAL_ADC_PollForConversion(&hadc1,10);                //轮询转换
 
	return (uint16_t)HAL_ADC_GetValue(&hadc1);	        	//·µ»Ø×î½üÒ»´ÎADC1¹æÔò×éµÄ×ª»»½á¹û
}
/*****************************************************************
*
	*Function Name: static uint16_t Get_Adc(uint32_t ch)  
	*Function ADC input channel be selected "which one channe"
	*Input Ref: which one ? AC_Channel_?
	*Return Ref: No
	*
	*
*****************************************************************/
//static uint16_t Get_Adc_Average(uint8_t times)
//{
//	
//	HAL_ADC_Start(&hadc1);								
//    
//    temp_value= ADC1->DR;
//    return temp_value;
//}

static uint16_t Get_Adc_Average(uint32_t ch,uint8_t times)
{
	uint32_t temp_val=0;
	uint8_t t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc_Channel(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 



void Get_PTC_Temperature_Voltage(uint32_t channel,uint8_t times)
{
    
	adcx = Get_Adc_Average(channel,times);

    run_t.ptc_temp_voltage  =(uint16_t)((adcx * 3300)/4096); //amplification 100 ,3.11V -> 311

	if(run_t.ptc_first_detected_times < 3){ //power on the voltage is small 
        run_t.ptc_first_detected_times ++ ;
         run_t.ptc_temp_voltage = 500;
    }
	else{
	  run_t.ptc_temp_voltage= run_t.ptc_temp_voltage - MODIFICATION_VALUE ;
	  Judge_PTC_Temperature_Value();

	}
   
}


/*****************************************************************
	*
	*Function Name: void Judge_PTC_Temperature_Value(void)
	*Function: PTC adc read voltage
	*Input Ref: NO
	*Return Ref: No
	*
	*
*****************************************************************/
static void Judge_PTC_Temperature_Value(void)
{
 
 
   //if(run_t.ptc_temp_voltage < 54 || run_t.ptc_temp_voltage ==54){ //75 degree
   
  //if(run_t.ptc_temp_voltage < 60 || run_t.ptc_temp_voltage ==60){ //70 degree
  #ifdef JINGPAI
	  if(run_t.ptc_temp_voltage < 373 || run_t.ptc_temp_voltage ==373){ //90 degree

  #else 
      if(run_t.ptc_temp_voltage < 400 || run_t.ptc_temp_voltage ==400){ //90 degree

  #endif 
		run_t.gDry =0 ;
		PTC_SetLow(); //turn off
        run_t.ptc_warning =1;
        
		MqttData_Publish_SetPtc(0);
		HAL_Delay(350);  
		
		Publish_Data_Warning(ptc_temp_warning,warning);
	    HAL_Delay(350);
		Buzzer_KeySound();
		HAL_Delay(100);
		Buzzer_KeySound();
		HAL_Delay(100);
		Buzzer_KeySound();
		HAL_Delay(100);
		Buzzer_KeySound();
		HAL_Delay(100);
		Buzzer_KeySound();	
		HAL_Delay(100);
		SendWifiCmd_To_Order(PTC_ERROR); //0xE0
		HAL_Delay(5);
		

		Publish_Data_Warning(ptc_temp_warning,warning);
	    HAL_Delay(350);
		
			  
				
	   	}
}
/*****************************************************************
	*
	*Function Name: void Get_Fan_Adc_Fun(uint8_t channel,uint8_t times)
	*Function ADC input channel be selected "which one channe"
	*Input Ref: which one ? AC_Channel_?, hexadecimal of average
	*Return Ref: No
	*
	*
*****************************************************************/
void Get_Fan_Adc_Fun(uint32_t channel,uint8_t times)
{
   uint16_t adc_fan_hex;
   Fan_Full_Speed();
   adc_fan_hex = Get_Adc_Average(channel,times);

    fan_detect_voltage  =(uint16_t)((adc_fan_hex * 3300)/4096); //amplification 1000 ,3.111V -> 3111
	//HAL_Delay(2);

//	if(fan_detect_voltage >500 && fan_detect_voltage < 1700){
//           detect_error_times=0;
//		  
////		   Publish_Data_Warning(fan_warning,no_warning); //don't has warning .
////		   HAL_Delay(350);
////		   SendWifiCmd_To_Order(FAN_REMOVE_ERROR); //0xE1,
////		    HAL_Delay(5);
////		  run_t.fan_warning = 0;
////		   
//    }
//    else{


  if(fan_detect_voltage < 350){ //500  now and then is bug false alarm rate  .
       detect_error_times++;
	   if(detect_error_times >2){
	   	
		 
		   run_t.fan_warning = 1;
		   
		
          MqttData_Publis_SetFan(0);
	      HAL_Delay(350);




	
		   Publish_Data_Warning(fan_warning,warning);
	       HAL_Delay(350);
		 
	    
	       Buzzer_KeySound();
	       HAL_Delay(100);
		   Buzzer_KeySound();
	       HAL_Delay(100);
		   Buzzer_KeySound();
	       HAL_Delay(100);
		   Buzzer_KeySound();
	       HAL_Delay(100);
		   Buzzer_KeySound();	
		   HAL_Delay(100);
           SendWifiCmd_To_Order(FAN_ERROR); //0xE1,
            HAL_Delay(5);

	

		   Publish_Data_Warning(fan_warning,warning);
	       HAL_Delay(350);
		}
	          

     }
}

    

