#include "publish.h"
#include "esp8266.h"
#include "mqtt_iot.h"
#include "run.h"
#include "dht11.h"


static void property_report_ptc_temp_wanrning(uint8_t warning);

static void property_report_plasma_warning(uint8_t warning);

static void property_report_ultrasonic_warning(uint8_t warning);

static void property_report_fan_warning(uint8_t warning);

static void Publish_Data_Ptc_Temp_Warning_Handler(uint8_t warning);
static void Publish_Data_Fan_Warning_Handler(uint8_t warning);
static void Publish_Data_Plasma_Warning_Handler(uint8_t warning);
static void Publish_Data_Ultrasnoic_Warning_Handler(uint8_t warning);






/****************************************************************************************
	*
	*Function Name:static void property_report_ptc_temp_wanrning(uint8_t warning)
	*Function:F
	*Input Ref: 0->no warning ,1->warning
	*Return Ref:
	*
****************************************************************************************/

static void property_report_ptc_temp_wanrning(uint8_t warning)
{
     char	message[128]    = {0};
	 int	message_len	  = 0;
	
	
	 message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up04\\\"\\,\\\"params\\\":{\\\"ptc_temp_wanrning\\\":%d}}\"\r\n",warning);
								  
	at_send_data((uint8_t *)message, message_len);

}

static void property_report_fan_warning(uint8_t warning)
{
     char	message[128]    = {0};
	 int	message_len	  = 0;
	
	
	 message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up04\\\"\\,\\\"params\\\":{\\\"fan_warning\\\":%d}}\"\r\n",warning);
								  
	at_send_data((uint8_t *)message, message_len);

}


static void property_report_plasma_warning(uint8_t warning)
{
     char	message[128]    = {0};
	 int	message_len	  = 0;
	
	
	 message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up04\\\"\\,\\\"params\\\":{\\\"plasma_warning\\\":%d}}\"\r\n",warning);
								  
	at_send_data((uint8_t *)message, message_len);

}

static void property_report_ultrasonic_warning(uint8_t warning)

{
     char	message[128]    = {0};
	 int	message_len	  = 0;
	
	
	 message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"up04\\\"\\,\\\"params\\\":{\\\"ultrasonic_warning\\\":%d}}\"\r\n",warning);
								  
	at_send_data((uint8_t *)message, message_len);

}




/*******************************************************************************
**
*Function Name:void Publish_Data_ToCloud(void)
*Function: dy
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Publish_Data_ToTencent_Initial_Data(void)
{
    
          
      MqttData_Publish_Init();
	         
	

}
	

void Publish_Data_ToTencent_Update_Data(void)
{
      MqttData_Publish_Update_Data();
  

}
/*******************************************************************************
**
*Function Name:static void Publish_Data_Ptc_Temp_Warning_Handler(uint8_t warning)
*Function: dy
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
static void Publish_Data_Ptc_Temp_Warning_Handler(uint8_t warning)
{
    property_topic_publish();
	property_report_ptc_temp_wanrning(warning);
	
}
static void Publish_Data_Fan_Warning_Handler(uint8_t warning)
{
	property_topic_publish();
	property_report_fan_warning(warning);


}

static void Publish_Data_Plasma_Warning_Handler(uint8_t warning)
{
    property_topic_publish();
	property_report_plasma_warning(warning);

}
static void Publish_Data_Ultrasnoic_Warning_Handler(uint8_t warning)
{
	  property_topic_publish();
	  property_report_ultrasonic_warning(warning);

}



void Publish_Data_Warning(warning_item whichone,uint8_t warning)
{
     switch(whichone){

	   case ptc_temp_warning:

	    Publish_Data_Ptc_Temp_Warning_Handler(warning);

	   break;

       case fan_warning:
		 Publish_Data_Fan_Warning_Handler(warning);
	   break;

	   case plasma_warning:
		Publish_Data_Plasma_Warning_Handler(warning);
	   break;

	   case ultrasonic_warning:
	   	Publish_Data_Ultrasnoic_Warning_Handler(warning);

	   break;

	   default:
	   break;

      }

}






