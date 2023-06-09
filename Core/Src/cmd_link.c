#include "cmd_link.h"
#include "usart.h"
#include "run.h"
#include "fan.h"
#include "esp8266.h"
#include "interrupt_manager.h"
#include "subscription.h"
#include "wifi_fun.h"
#include "buzzer.h"

#define MAX_BUFFER_SIZE  30

uint8_t  inputBuf[4];
uint8_t  inputCmd[2];
uint8_t  wifiInputBuf[1];
//uint8_t test_counter;
//uint8_t test_counter_usat1;

uint8_t rx_wifi_data[7];

//uint8_t wifi_rx_temp_data[25];


static uint8_t transferSize;
static uint8_t outputBuf[MAX_BUFFER_SIZE];

volatile uint8_t transOngoingFlag;
volatile uint8_t usart2_transOngoingFlag;

void USART1_ERROR_Callback(void);
void (*EUSART_TxDefaultInterruptHandler)(void);
void (*EUSART_RxDefaultInterruptHandler)(void);

//void (*EUSART_FramingErrorHandler)(void);
//void (*EUSART_OverrunErrorHandler)(void);
//void (*EUSART_ErrorHandler)(void);




    
/********************************************
	*
	*Function Name:void Decode_Function(void)
    *Function: receive dsipay panel of order
    *Input Ref:NO
    *Return Ref:NO

*********************************************/ 
void Decode_Function(void)
{
   if(run_t.decodeFlag==1){
   
      run_t.decodeFlag =0;
	  run_t.process_run_guarantee_flag=1;
      Decode_RunCmd();
       run_t.decodeFlag =0;
      
     }
}

void USART1_Cmd_Error_Handler(UART_HandleTypeDef *huart)
{

     uint32_t temp;

   if(huart==&huart1){


      if(run_t.gTimer_usart_error >39){
	  	run_t.gTimer_usart_error=0;
	      __HAL_UART_GET_FLAG(&huart1,UART_FLAG_ORE);//UART_FLAG_NE
         __HAL_UART_GET_FLAG(&huart1,UART_FLAG_NE); //USART_ISR_FE
         __HAL_UART_GET_FLAG(&huart1,USART_ISR_FE);
		 
		
         if(UART_FLAG_ORE==1 ){
           __HAL_UART_CLEAR_OREFLAG(&huart1);
              __HAL_UART_CLEAR_NEFLAG(&huart1);
               __HAL_UART_CLEAR_FEFLAG(&huart1);

        
          temp = USART1->RDR;

		  UART_Start_Receive_IT(&huart1,inputBuf,1);
		
		
         }
		  
          
        }
  	  
   	}
}

void USART2_Cmd_Error_Handler(UART_HandleTypeDef *huart)
{
       uint32_t temp;

	if(huart==&huart2){


      if(run_t.gTimer_usart2_error >5){
	  	run_t.gTimer_usart2_error=0;
//	      __HAL_UART_GET_FLAG(&huart2,UART_FLAG_ORE);//UART_FLAG_NE
//         __HAL_UART_GET_FLAG(&huart2,UART_FLAG_NE); //USART_ISR_FE
//         __HAL_UART_GET_FLAG(&huart2,USART_ISR_FE);
//		UART_Start_Receive_IT(&huart2,(uint8_t *)UART2_DATA.UART_DataBuf,1);
//		
//         if(UART_FLAG_ORE==1 ){
           __HAL_UART_CLEAR_OREFLAG(&huart2);
            //  __HAL_UART_CLEAR_NEFLAG(&huart2);
             //  __HAL_UART_CLEAR_FEFLAG(&huart2);

          
          
          temp=USART2->ISR;
          temp = USART2->RDR;

		   UART_Start_Receive_IT(&huart2,(uint8_t *)UART2_DATA.UART_DataBuf,1);
		
		
         	}
		  
        } 
       
  	  
   
}
/********************************************************************************
	**
	*Function Name:sendData_Real_TimeHum(uint8_t hum,uint8_t temp)
	*Function :
	*Input Ref: humidity value and temperature value
	*Return Ref:NO
	*
*******************************************************************************/
void sendData_Real_TimeHum(uint8_t hum,uint8_t temp)
{

	//crc=0x55;
	outputBuf[0]='M'; //master
	outputBuf[1]='A'; //41
	outputBuf[2]='D'; //44	// 'D' data
	outputBuf[3]=hum; //	// 'R' rotator motor for select filter
	outputBuf[4]=temp; // // one command parameter
	
	//for(i=3;i<6;i++) crc ^= outputBuf[i];
	//outputBuf[i]=crc;
	transferSize=5;
	if(transferSize)
	{
		while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}



}
void SendWifiData_To_PanelTime(uint8_t dat1)
{
   
	
		outputBuf[0]='M'; //4D
		outputBuf[1]='A'; //41
		outputBuf[2]='T'; //44	// 'T' time
		outputBuf[3]=dat1; //	
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}



}
/********************************************************************************
**
*Function Name:void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
*Function :UART callback function  for UART interrupt for transmit data
*Input Ref: structure UART_HandleTypeDef pointer
*Return Ref:NO
*
*******************************************************************************/
void SendWifiData_To_PanelTemp(uint8_t dat1)
{
   
	//crc=0x55;
		outputBuf[0]='M'; //4D
		outputBuf[1]='A'; //41
		outputBuf[2]='P'; // 'T' time
		outputBuf[3]=dat1; //	
	
		
		//for(i=3;i<6;i++) crc ^= outputBuf[i];
		//outputBuf[i]=crc;
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}

/********************************************************************************
**
*Function Name:void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
*Function :UART callback function  for UART interrupt for transmit data
*Input Ref: structure UART_HandleTypeDef pointer
*Return Ref:NO
*
*******************************************************************************/
void SendWifiData_To_WifiSetTemp(uint8_t dat1)
{
   
	//crc=0x55;
		outputBuf[0]='M'; //4D
		outputBuf[1]='A'; //41
		outputBuf[2]='E'; // 'T' time
		outputBuf[3]=dat1; //	
	
		
		//for(i=3;i<6;i++) crc ^= outputBuf[i];
		//outputBuf[i]=crc;
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}


void SendWifiData_To_PanelWindSpeed(uint8_t dat1)
{
   
	//crc=0x55;
		outputBuf[0]='M'; //
		outputBuf[1]='A'; //
		outputBuf[2]='S'; // wind speed
		outputBuf[3]=dat1; //	
	
		
		//for(i=3;i<6;i++) crc ^= outputBuf[i];
		//outputBuf[i]=crc;
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}


void SendWifiData_To_Cmd(uint8_t wdata)
{
  
	//crc=0x55;
			outputBuf[0]='M'; //4D
			outputBuf[1]='A'; //41
			outputBuf[2]='W'; //44	// wifi ->infomation link wifi 	
			outputBuf[3]=wdata; //
			//for(i=3;i<6;i++) crc ^= outputBuf[i];
			//outputBuf[i]=crc;
			transferSize=4;
			if(transferSize)
			{
				while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
				transOngoingFlag=1;
				HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
			}
}
/***************************************************************
 * 
 * Function: panel power on and special function
***************************************************************/
void SendWifiCmd_To_Order(uint8_t odata)
{
  

			outputBuf[0]='M'; //mainboard
			outputBuf[1]='A'; //A ->
			outputBuf[2]='C'; //'C' ->control 
			outputBuf[3]=odata; //	
		
			transferSize=4;
			if(transferSize)
			{
				while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
				transOngoingFlag=1;
				HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
			}



}

void SendData_Real_GMT(uint8_t hdata)
{
   

	outputBuf[0]='M'; //4D
	outputBuf[1]='A'; //41
	outputBuf[2]='B'; //44	// 'C' ->control 
	outputBuf[3]=hdata; //	
	//outputBuf[4]=mdata; //

	//for(i=3;i<6;i++) crc ^= outputBuf[i];
	//outputBuf[i]=crc;
	transferSize=4;
	if(transferSize)
	{
		while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}

}

void SendData_Real_GMT_Minute(uint8_t hdata)
{
   

	outputBuf[0]='M'; //4D
	outputBuf[1]='A'; //41
	outputBuf[2]='M'; //44	// 'C' ->control 
	outputBuf[3]=hdata; //	
	

	//for(i=3;i<6;i++) crc ^= outputBuf[i];
	//outputBuf[i]=crc;
	transferSize=4;
	if(transferSize)
	{
		while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}

}


void SendData_Real_GMT_Second(uint8_t hdata)
{
   

	outputBuf[0]='M'; //4D
	outputBuf[1]='A'; //41
	outputBuf[2]='N'; //44	// 'C' ->control 
	outputBuf[3]=hdata; //	
	

	//for(i=3;i<6;i++) crc ^= outputBuf[i];
	//outputBuf[i]=crc;
	transferSize=4;
	if(transferSize)
	{
		while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}

}


void EUSART_SetTxInterruptHandler(void (* interruptHandler)(void))
{
    EUSART_TxDefaultInterruptHandler = interruptHandler;
}


/********************************************************************************
**
*Function Name:void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
*Function :UART callback function  for UART interrupt for transmit data
*Input Ref: structure UART_HandleTypeDef pointer
*Return Ref:NO
*
*******************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart==&huart1)
	{
		transOngoingFlag=0; //UART Transmit interrupt flag =0 ,RUN
	}

	if(huart== &huart2){

       usart2_transOngoingFlag =0;

	}

}





