#ifndef __CMD_LINK_H_
#define __CMD_LINK_H_
#include "main.h"


extern uint8_t rx_wifi_data[7];

extern uint8_t inputBuf[4];

extern uint8_t  inputCmd[2];
extern uint8_t wifiInputBuf[1];



extern void (*EUSART_TxDefaultInterruptHandler)(void);
extern void (*EUSART_RxDefaultInterruptHandler)(void);

void EUSART_SetRxInterruptHandler(void (* interruptHandler)(void));

void Decode_Function(void);

void sendData_Real_TimeHum(uint8_t hum,uint8_t temp);

void SendWifiData_To_Cmd(uint8_t wdata);

void SendWifiData_To_PanelWindSpeed(uint8_t dat1);


void SendWifiData_To_PanelTime(uint8_t dat1);
void SendWifiData_To_PanelTemp(uint8_t dat1);
void SendWifiCmd_To_Order(uint8_t odata);
void SendData_Real_GMT_Second(uint8_t sdata);
void Eesp8266_TxData_ToSmartPhone(void);

void SendWifiData_To_WifiSetTemp(uint8_t dat1);
void SendData_Real_GMT(uint8_t hdata);
void SendData_Real_GMT_Minute(uint8_t hdata);



void USART1_Cmd_Error_Handler(UART_HandleTypeDef *huart);
void USART2_Cmd_Error_Handler(UART_HandleTypeDef *huart);


#endif 
