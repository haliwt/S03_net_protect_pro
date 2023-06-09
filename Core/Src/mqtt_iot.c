#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mqtt_iot.h"
#include "esp8266.h"
#include "run.h"
#include "usart.h"


#define TOKEN_ID      "123"


//char *tx_data={"AT+TCMQTTPUB=\"$thing/up/property/EHQB1P53IH/UYIJIA01-a0005\",0,\"{\"method\":\"report\"\,\"clientToken\":\"up01\"\,\"params\":\"{\"open\":1\,\"temperature\":26}}\"\r\n"};
//char *tx_data={"AT+TCMQTTPUB=\"$thing/up/property/EHQB1P53IH/UYIJIA01-a0005\",0,\"{\\\"method\\\":\\\"report\\\"\\\,\\\"clientToken\\\":\\\"up01\\\"\\,\\\"params\\\":{\\\"open\\\":1\\,\\\"temperature\\\":26}}\"\r\n"};

/* MAX size of client ID */


static void Mqtt_power_off_Value(void);

static void property_report_SetOpen(uint8_t open);
static void Mqtt_Value_update_data(void);



static void property_report_state(void);


static void property_report_ReadTempHum(uint8_t tempvalue,uint8_t humvalue);

static void property_report_SetTemp(uint8_t temp);
static void property_report_SetFan(uint8_t fan);
static void property_report_SetTime(uint8_t time);
static void property_report_SetState(uint8_t dat);
static void property_report_power_off_state(void);



typedef struct {
    
    bool     open;
	bool     sonic;
	
	bool     ptc;
	bool     anion;

	uint8_t     state ; //
    
	uint8_t  find;
	uint8_t  set_temperature;

} serviceInfo;


static serviceInfo    sg_info;


// led attributes, corresponding to struct LedInfo
//static char *sg_property_name[] = {"opne", "sonic", "find", "nowtemperature","state","ptc","Anion","temperature","Humidity"};
void Mqtt_Value_Init(void)
{
    run_t.set_wind_speed_value=100;
    run_t.set_temperature_value=40 ;
   	sg_info.open=1;
    sg_info.state=1;
    sg_info.ptc=1; 
    sg_info.anion=1;  //灭菌
	sg_info.sonic =1;  //驱虫
    sg_info.find=run_t.set_wind_speed_value;
	//if(run_t.set_temperature_value <20)run_t.set_temperature_value = 20;
	//else if(run_t.set_temperature_value > 40 )run_t.set_temperature_value = 40;
	sg_info.set_temperature =  run_t.set_temperature_value ;
	
}
static void Mqtt_Value_update_data(void)
{
    
    sg_info.open = 1;
	if(run_t.gModel==0)run_t.gModel =1;
	sg_info.state = run_t.gModel;
	sg_info.ptc  = run_t.gDry;
	sg_info.anion = run_t.gPlasma;
	sg_info.sonic = run_t.gUlransonic ;
    sg_info.find = run_t.set_wind_speed_value;
    if(run_t.set_temperature_value <20)run_t.set_temperature_value = 20;
	else if(run_t.set_temperature_value > 40)run_t.set_temperature_value = 40;
	sg_info.set_temperature = run_t.set_temperature_value;

}

static void Mqtt_power_off_Value(void)
{
    run_t.set_wind_speed_value=10;
   	sg_info.open=0;
    sg_info.state=1;
    sg_info.ptc=0; 
    sg_info.anion=0;  //灭菌
	sg_info.sonic =0;  //驱虫
    sg_info.find=run_t.set_wind_speed_value;
	if(run_t.set_temperature_value <20)run_t.set_temperature_value = 20;
	else if(run_t.set_temperature_value > 40 )run_t.set_temperature_value = 40;
	sg_info.set_temperature =  run_t.set_temperature_value ;
	
}


/********************************************************************************
	*
	*Function Name:static void property_report_Temp_Humidity(void)
	*Function :send data to tencent cloud only read temperature and humidity of data
	*Input Ref: only read temperature value and humidiy value
	*           
	*Return Ref:
	*
********************************************************************************/
void property_topic_publish(void)
{
    char topic[128] = {0};
    int  size;

    run_t.randomName[0]=HAL_GetUIDw0();
    size = snprintf(topic, sizeof(topic), "AT+TCMQTTPUB=\"$thing/up/property/%s/UYIJIA01-%d\",0,", PRODUCT_ID,run_t.randomName[0]);
    at_send_data((uint8_t *)topic, size);
 
   
}
/********************************************************************************
	*
	*Function Name:static void property_report_state(void)
	*Function : publis to tencent cloud data 
	*Input Ref: 
	*           
	*Return Ref:
	*
********************************************************************************/
static void property_report_state(void)
{
    char       message[256]    = {0};
    int        message_len     = 0;

    Mqtt_Value_Init();
   message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up01\\\"\\,\\\"params\\\":{\\\"open\\\":%d\\,\\\"Anion\\\":%d\\,\\\"ptc\\\":%d\\,\\\"sonic\\\":%d\\,\\\"state\\\":%d\\,\\\"find\\\":%d\\,\\\"temperature\\\":%d}}\"\r\n",
                             sg_info.open,sg_info.anion,sg_info.ptc,sg_info.sonic,sg_info.state,sg_info.find,sg_info.set_temperature);
                               
 
	at_send_data((uint8_t *)message, message_len);
   
}


void property_report_update_data(void)
{
	char  message[256]    = {0};
	int   message_len	   = 0;
	
	 Mqtt_Value_update_data();
	 message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up02\\\"\\,\\\"params\\\":{\\\"open\\\":%d\\,\\\"Anion\\\":%d\\,\\\"ptc\\\":%d\\,\\\"sonic\\\":%d\\,\\\"state\\\":%d\\,\\\"find\\\":%d\\,\\\"temperature\\\":%d}}\"\r\n",
								 sg_info.open,sg_info.anion,sg_info.ptc,sg_info.sonic,sg_info.state,sg_info.find,sg_info.set_temperature);
								   
	 
	at_send_data((uint8_t *)message, message_len);


}

static void property_report_power_off_state(void)
{

	char       message[256]    = {0};
    int        message_len     = 0;

   Mqtt_power_off_Value();
   message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up01\\\"\\,\\\"params\\\":{\\\"open\\\":%d\\,\\\"Anion\\\":%d\\,\\\"ptc\\\":%d\\,\\\"sonic\\\":%d\\,\\\"state\\\":%d\\,\\\"find\\\":%d\\,\\\"temperature\\\":%d}}\"\r\n",
                             sg_info.open,sg_info.anion,sg_info.ptc,sg_info.sonic,sg_info.state,sg_info.find,sg_info.set_temperature);
                               
 
  at_send_data((uint8_t *)message, message_len);


}


/********************************************************************************
	*
	*Function Name:static void property_report_Temp_Humidity(void)
	*Function :send data to tencent cloud only read temperature and humidity of data
	*Input Ref: only read temperature value and humidiy value
	*           
	*Return Ref:
	*
********************************************************************************/
static void property_report_ReadTempHum(uint8_t tempvalue,uint8_t humvalue)
{

	   char	message[128]    = {0};
	   int	message_len	  = 0;
	   message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up00\\\"\\,\\\"params\\\":{\\\"nowtemperature\\\":%d\\,\\\"Humidity\\\":%d}}\"\r\n"
								,tempvalue,humvalue);
								  
		at_send_data((uint8_t *)message, message_len);
}

static void property_report_SetState(uint8_t dat)
{
     char	message[128]    = {0};
	 int	message_len	  = 0;
	
	
	 message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up04\\\"\\,\\\"params\\\":{\\\"state\\\":%d}}\"\r\n",dat);
								  
	at_send_data((uint8_t *)message, message_len);

}
/********************************************************************************
	*
	*Function Name:static void property_report_SetTempFan(void)
	*Function : sensor of data to tencent cloud  temperature and humidity of data
	*Input Ref: write temperature value          
	*Return Ref:
	*
********************************************************************************/
static void property_report_SetTemp(uint8_t temp)
{
     char	message[128]    = {0};
	 int	message_len	  = 0;
	
	
	 message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up03\\\"\\,\\\"params\\\":{\\\"temperature\\\":%d}}\"\r\n",temp);
								  
	at_send_data((uint8_t *)message, message_len);

}
static void property_report_SetOpen(uint8_t open)
{
     char	message[128]    = {0};
	 int	message_len	  = 0;
	
	
	 message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up04\\\"\\,\\\"params\\\":{\\\"open\\\":%d}}\"\r\n",open);
								  
	at_send_data((uint8_t *)message, message_len);

}
/********************************************************************************
	*
	*Function Name:static void property_report_SetSonic(uint8_t datsonic)
	*Function : "驱虫"
	*Input Ref: bool-> 1 ->on ,0 -> off
	*Return Ref:
	*
********************************************************************************/
static void property_report_SetSonic(uint8_t datsonic)
{
     char	message[128]    = {0};
	 int	message_len	  = 0;
	
	
	message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up02\\\"\\,\\\"params\\\":{\\\"sonic\\\":%d}}\"\r\n"
								,datsonic);
								  
	at_send_data((uint8_t *)message, message_len);

}
/********************************************************************************
	*
	*Function Name:static void property_report_SetAnion(uint8_t datsonic)
	*Function : "灭菌"
	*Input Ref: bool-> 1 ->on ,0 -> off
	*Return Ref:
	*
********************************************************************************/
static void property_report_SetAnion(uint8_t datanion)
{
     char	message[128]    = {0};
	 int	message_len	  = 0;
	
	
	 message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up03\\\"\\,\\\"params\\\":{\\\"Anion\\\":%d}}\"\r\n"
								,datanion);
								  
	at_send_data((uint8_t *)message, message_len);

}
/********************************************************************************
	*
	*Function Name:static void property_report_SetPtc(uint8_t datsonic)
	*Function : "驱虫"
	*Input Ref: bool-> 1 ->on ,0 -> off
	*Return Ref:
	*
********************************************************************************/
static void property_report_SetPtc(uint8_t datptc)
{
     char	message[128]    = {0};
	 int	message_len	  = 0;
	
	
	 message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up04\\\"\\,\\\"params\\\":{\\\"ptc\\\":%d}}\"\r\n"
								,datptc);
								  
	at_send_data((uint8_t *)message, message_len);

}


/********************************************************************************
	*
	*Function Name:static void property_report_SetTempFan(void)
	*Function : sensor of data to tencent cloud  temperature and humidity of data
	*Input Ref: only read temperature value and humidiy value
	*           
	*Return Ref:
	*
********************************************************************************/
static void property_report_SetFan(uint8_t fan)
{
     char	message[128]    = {0};
	 int	message_len	  = 0;
	
	
	 message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up05\\\"\\,\\\"params\\\":{\\\"find\\\":%d}}\"\r\n",fan);
	at_send_data((uint8_t *)message, message_len);

}
/********************************************************************************
	*
	*Function Name:static void property_report_SetTempFan(void)
	*Function : sensor of data to tencent cloud  temperature and humidity of data
	*Input Ref: only read temperature value and humidiy value
	*           
	*Return Ref:
	*
********************************************************************************/
static void property_report_SetTime(uint8_t time)
{

	char   message[128]    = {0};
	int    message_len	 = 0;
	   
	   
	message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up05\\\"\\,\\\"params\\\":{\\\"find\\\":%d}}\"\r\n",
								   time);
	  at_send_data((uint8_t *)message, message_len);
}

/********************************************************************************
	*
	*Function Name:void IOT_MQTT_Publish(pClient, topic, &pubParams)
	*Function :send data to tencent cloud 
	*Input Ref: pClient ->client data array ,topic->tencent register topic name,
	*           &pubParams -> topic of reference
	*Return Ref:
	*
********************************************************************************/
void MqttData_Publish_Init(void)
{
	 property_topic_publish();

	 property_report_state();

}

void MqttData_Publish_Update_Data(void)
{
    property_topic_publish();
	property_report_update_data();

}

void MqttData_Publish_SetOpen(uint8_t dop)
{
   property_topic_publish();
   property_report_SetOpen(dop);
}

void MqttData_Publish_SetPtc(uint8_t dptc)
{
   property_topic_publish();
    property_report_SetPtc(dptc);

}

void MqttData_Publish_SetPlasma(uint8_t pla) //杀菌
{
   property_topic_publish();
   property_report_SetAnion(pla);

}

void MqttData_Publish_SetUltrasonic(uint8_t datsonic) //超声波
{
   property_topic_publish();
   property_report_SetSonic(datsonic);

}

void MqttData_Publish_SetState(uint8_t sdat) //Ai model
{
   property_topic_publish();
   property_report_SetState(sdat);

}


void MqttData_Publish_PowerOff_Ref(void) //
{
   property_topic_publish();

   property_report_power_off_state();

}

/********************************************************************************
	*
	*Function Name:void MqttData_Publis_TempHumidity(void)
	*Function : publis data to tencent cloud 
	*Input Ref: pClient ->client data array ,topic->tencent register topic name,
	*           &pubParams -> topic of reference
	*Return Ref:
	*
********************************************************************************/
void MqttData_Publis_ReadTempHum(uint8_t tempvalue,uint8_t humvalue)
{

	property_topic_publish(); 
	property_report_ReadTempHum(tempvalue,humvalue);


}

void MqttData_Publis_SetFan(uint8_t fan)
{
	property_topic_publish(); 
    property_report_SetFan(fan);

}
void MqttData_Publis_SetTemp(uint8_t temp)
{
	property_topic_publish(); 
    property_report_SetTemp(temp);


}


void MqttData_Publis_SetTime(uint8_t time)
{
	property_topic_publish(); 
    property_report_SetTime(time);


}



