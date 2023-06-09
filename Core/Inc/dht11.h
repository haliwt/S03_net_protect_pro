#ifndef __DHT11_H__
#define __DHT11_H__
#include "main.h"

//IO方向设置
#define DHT11_DATA_IO_IN()      {GPIOB->MODER&=0XFFFFFFFC;GPIOB->MODER|=0<<0;}  //0x00 input mode
#define DHT11_DATA_IO_OUT()     {GPIOB->MODER&=0XFFFFFFFC;GPIOB->MODER|=1<<0;}   //0x01 output  mode 

#define DHT11_DATA      GPIO_PIN_13
#define DHT11_GPIO      GPIOB

#define DHT11_DATA_SetHigh()            HAL_GPIO_WritePin(DHT11_GPIO,DHT11_DATA,GPIO_PIN_SET)    // output high level
#define DHT11_DATA_SetLow()             HAL_GPIO_WritePin(DHT11_GPIO,DHT11_DATA,GPIO_PIN_RESET)    // output low level

#define DHT11_ReadData()	            HAL_GPIO_ReadPin(DHT11_GPIO,DHT11_DATA)

/* 宏定义 -------------------------------------------------------------------*/
/***********************   DHT11 连接引脚定义  **************************/
#define DHT11_Dout_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOB_CLK_ENABLE()//__HAL_RCC_GPIOA_CLK_ENABLE()
#define DHT11_Dout_PORT                           GPIOB
#define DHT11_Dout_PIN                            GPIO_PIN_13

/***********************   DHT11 函数宏定义  ****************************/
#define DHT11_Dout_LOW()                          HAL_GPIO_WritePin(DHT11_Dout_PORT, DHT11_Dout_PIN, GPIO_PIN_RESET)
#define DHT11_Dout_HIGH()                         HAL_GPIO_WritePin(DHT11_Dout_PORT, DHT11_Dout_PIN, GPIO_PIN_SET)
#define DHT11_Data_IN()	                          HAL_GPIO_ReadPin(DHT11_Dout_PORT,DHT11_Dout_PIN)



/* 类型定义 ------------------------------------------------------------------*/
/************************ DHT11 数据类型定义******************************/
typedef struct
{
	uint8_t  humi_high8bit;		//原始数据：湿度高8位
	uint8_t  humi_low8bit;	 	//原始数据：湿度低8位
	uint8_t  temp_high8bit;	 	//原始数据：温度高8位
	uint8_t  temp_low8bit;	 	//原始数据：温度高8位
	uint8_t  check_sum;	 	    //校验和
  float    humidity;            //实际湿度
  float    temperature;        //实际温度  
} DHT11_Data_TypeDef;
extern DHT11_Data_TypeDef DHT11;


/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
//void DHT11_Init( void );
//uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef * DHT11_Data);
void Update_DHT11_Value(void);
void Update_Dht11_Totencent_Value(void);


#endif 






