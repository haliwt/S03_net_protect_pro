#include "wifi_fun.h"
#include "cmd_link.h"
#include "run.h"
#include "fan.h"
#include "tim.h"
#include "special_power.h"
#include "esp8266.h"
#include "publish.h"
#include "subscription.h"
#include "dht11.h"
#include "usart.h"
#include "mqtt_iot.h"
#include "flash.h"


WIFI_FUN   wifi_t;
uint8_t sub_send_power_on_times;


void (*PowerOn)(void);
void (*PowerOff)(void);
void (*Ai_Fun)(uint8_t sig);


void (*SetTimes)(void);
void (*SetTemperature)(void);
uint8_t get_rx_beijing_time_enable;


static void AutoReconnect_Wifi_Neware_Function(void);


void PowerOn_Host(void (* poweronHandler)(void))
{
    PowerOn = poweronHandler;  

}


void PowerOff_Host(void(*poweroffHandler)(void))
{
   PowerOff = poweroffHandler;

}

uint8_t first_connect;
/****************************************************************
     * 
     * Function Name:void AI_Function_Host(void(*AIhandler)(uint8_t sig))
     * Function: take with reference of function pointer
     * 
     * 
****************************************************************/
void AI_Function_Host(void(*AIhandler)(uint8_t sig))
{
    Ai_Fun=AIhandler;
}

void SetTimeHost(void(*timesHandler)(void))
{
    SetTimes = timesHandler;

}


void SetTemperatureHost(void(*temperatureHandler)(void))
{
    SetTemperature = temperatureHandler ;

}

/***********************************************
   *
   *Function Name: void Wifi_RunCmd(void)
   *Funciton : separately update value 
   *
   *
***********************************************/
void RunWifi_Command_Handler(void)
{
    uint8_t i;
    static uint8_t first_sub,sub_to_tencent_flag;

	static uint8_t  get_bj_times=0;

     switch(wifi_t.wifi_runCommand_order_lable){

         
	    case wifi_has_been_connected:
		  first_sub=0;
		  first_connect=0;
           run_t.gTimer_ptc_adc_times=0;
		   run_t.gTimer_fan_adc_times=0;
		  wifi_t.get_rx_beijing_time_enable=0;
		 run_t.wifi_config_net_lable=0;
		 
		 
	     if(esp8266_t.esp8266_login_cloud_success==1){
		  	esp8266_t.linking_tencent_cloud_doing=0;
	
	       
			wifi_t.has_been_login_flag = 1;
			wifi_t.get_rx_beijing_time_enable=0;
			wifi_t.wifi_runCommand_order_lable = wifi_tencent_publish_init_data;//wifi_tencent_subscription_data;
			
				
		  }

		break;

        case wifi_link_tencent_cloud: //02

		   if(run_t.gPower_flag == POWER_ON){
			 run_t.gTimer_ptc_adc_times=0;
			  run_t.gTimer_fan_adc_times=0;
			Wifi_SoftAP_Config_Handler();
	        SmartPhone_LinkTencent_Cloud();
	     
	      if(esp8266_t.esp8266_login_cloud_success==1){
			
				 SendWifiData_To_Cmd(0x01) ;	//WT.EDIT 2023.03.02
				run_t.first_link_tencent_cloud_flag =1;
				wifi_t.get_rx_beijing_time_enable=0;
			    wifi_t.wifi_runCommand_order_lable = wifi_tencent_publish_init_data;
                run_t.wifi_run_set_restart_flag =0;
                run_t.rx_command_tag=RUN_COMMAND ;// WT.EDIT 2023.08.10
                run_t.RunCommand_Label = POWER_ON;
                
				
			}
		   }
           
           if(esp8266_t.esp8266_login_cloud_success==0){
             if(run_t.gTimer_linking_tencen_counter < 166){
                 wifi_t.wifi_runCommand_order_lable = wifi_link_tencent_cloud;
             }
             else{
              wifi_t.wifi_runCommand_order_lable = wifi_null;
              run_t.wifi_run_set_restart_flag =0;
              }
           
           }
	    break;

	  	
	  	case wifi_tencent_publish_init_data://03
		  
				do {
			     run_t.wifi_run_set_restart_flag =0;
				 MqttData_Publish_SetOpen(0x01);
		         HAL_Delay(200);
		         Publish_Data_ToTencent_Initial_Data();
				 HAL_Delay(350);

				Subscriber_Data_FromCloud_Handler();
				HAL_Delay(350);
	           

			   
			     sub_to_tencent_flag=0;
				
				wifi_t.wifi_runCommand_order_lable= wifi_tencent_subscription_data;
	           
				}while(sub_to_tencent_flag);
		
		 
       	break;

		
		case wifi_tencent_subscription_data://04
           
		
				 
				 SendWifiData_To_Cmd(0x01) ;	//WT.EDIT 2023.03.02
				 HAL_Delay(2);
		
		
			 	
				wifi_t.gTimer_get_beijing_time=0;
				 run_t.gTimer_ptc_adc_times=0;
				 run_t.gTimer_fan_adc_times=0;

				sub_to_tencent_flag=0;
				 run_t.gTimer_auto_detected_net_link_state=0;
				 run_t.gTimer_publish_dht11=0;
				 run_t.beijing_time_flag=0;

				 wifi_t.wifi_runCommand_order_lable= wifi_publish_update_tencent_cloud_data;
					
			
		break;

	   	case wifi_publish_update_tencent_cloud_data://05
            
		      while(run_t.beijing_time_flag == 1 && run_t.gPower_flag ==POWER_ON){
				run_t.beijing_time_flag ++;
				wifi_t.gTimer_get_beijing_time=0;
				wifi_t.get_rx_beijing_time_enable=0;//disenable get beijing timing

				if(get_bj_times < 2){
				get_bj_times++;
				wifi_t.wifi_runCommand_order_lable= wifi_get_beijing_time;
				run_t.set_beijing_time_flag =1;

				}

				if(wifi_t.real_hours < 25 && wifi_t.real_minutes < 61){

				SendData_Real_GMT(wifi_t.real_hours);
				HAL_Delay(10);
				SendData_Real_GMT_Minute(wifi_t.real_minutes);
				HAL_Delay(10);
				SendData_Real_GMT_Second(wifi_t.real_seconds);
				HAL_Delay(10);
				}

			  }

              if(run_t.gTimer_publish_dht11 >138){
				   run_t.gTimer_publish_dht11=0;

				wifi_t.wifi_runCommand_order_lable= wifi_tencent_publish_dht11_data;
			}

			if(run_t.gTimer_auto_detected_net_link_state > 9){		

				  run_t.gTimer_auto_detected_net_link_state=0;

			      AutoReconnect_Wifi_Neware_Function();
					
			 
			}
			 

	   break;



	   case wifi_tencent_publish_dht11_data://6
	   	     wifi_t.get_rx_beijing_time_enable=0;

		
			
    
			if(run_t.gPower_flag == POWER_ON){
			 if(run_t.app_timer_power_off_flag == 0 && run_t.gPower_On ==POWER_ON && (run_t.app_timer_power_on_flag==0||run_t.app_timer_power_on_flag==3)){
				Update_Dht11_Totencent_Value();
				HAL_Delay(300);
			
			 	}
			
			
			if(wifi_t.gTimer_get_beijing_time > 423 && run_t.gPower_On ==POWER_ON){ //&& run_t.power_on_send_bejing_times==0){ //
			   wifi_t.gTimer_get_beijing_time=0;
			  
			   run_t.set_beijing_time_flag=1; //set beijing times .
			   wifi_t.wifi_runCommand_order_lable= wifi_get_beijing_time;
			   
			    
			}
			else{

                wifi_t.wifi_runCommand_order_lable= wifi_publish_update_tencent_cloud_data;
               
			}

			

			
			}
			else{

                wifi_t.wifi_runCommand_order_lable= wifi_publish_update_tencent_cloud_data;
               
			}

		  
	  

	   break;

	   case wifi_get_beijing_time://7

	    if(run_t.gPower_On==POWER_ON ){
	 
		 esp8266_t.linking_tencent_cloud_doing =0;
	   	 if(run_t.set_beijing_time_flag ==1){   //&& wifi_t.gTimer_beijing_time>1){
			 run_t.set_beijing_time_flag ++;
		     wifi_t.get_rx_beijing_time_enable=1; //enable beijing times
		     UART2_DATA.UART_Cnt=0;
             Get_BeiJing_Time_Cmd();
			 HAL_Delay(1000); //200
			 run_t.gTimer_read_beijing_time=0;
	   	 	}

		 
	       if(run_t.gTimer_read_beijing_time > 2 && run_t.gTimer_read_beijing_time < 4){
				run_t.gTimer_read_beijing_time=0;
			   	Get_Beijing_Time();
				HAL_Delay(300);
			    run_t.beijing_time_flag = 1;
				
			  	wifi_t.real_hours = (UART2_DATA.UART_Data[134]-0x30)*10 + UART2_DATA.UART_Data[135]-0x30;
				wifi_t.real_minutes =(UART2_DATA.UART_Data[137]-0x30)*10 + UART2_DATA.UART_Data[138]-0x30;
			     wifi_t.real_seconds = (UART2_DATA.UART_Data[140]-0x30)*10 + UART2_DATA.UART_Data[141]-0x30;
			     wifi_t.get_rx_beijing_time_enable=0; //enable beijing times
				
		         wifi_t.wifi_runCommand_order_lable=wifi_publish_update_tencent_cloud_data;
	        }
		   if(run_t.gTimer_read_beijing_time > 3){

		       wifi_t.wifi_runCommand_order_lable=wifi_publish_update_tencent_cloud_data;

		   }

			
		  }
		 else{

		     wifi_t.wifi_runCommand_order_lable=wifi_publish_update_tencent_cloud_data;
			 wifi_t.get_rx_beijing_time_enable=0; //enable beijing times
			
		}
    
	    break;

	   
	  default:

	   break;
	 
     }
  
     if(run_t.beijing_time_flag ==2){
	     run_t.beijing_time_flag++; 
         for(i=0;i<150;i++){
	      UART2_DATA.UART_Data[i]=0;

         }

	 }

	
  }
 
	 	


static void AutoReconnect_Wifi_Neware_Function(void)
{
  
     static uint8_t det_no_wifi_net=0;
			 
	if(wifi_t.wifi_reconnect_read_flag == 1 && det_no_wifi_net==0){

		det_no_wifi_net++;
		run_t.auto_link_cloud_flag=0;
		SendWifiData_To_Cmd(0x0) ;


		esp8266_t.esp8266_login_cloud_success=0;

	}

	if(wifi_t.wifi_reconnect_read_flag == 1 && run_t.auto_link_cloud_flag==0 ){

		AutoRepeate_Link_Tencent_Cloud();
		wifi_t.wifi_runCommand_order_lable= wifi_publish_update_tencent_cloud_data;
	}

	if(det_no_wifi_net==1){

		if(esp8266_t.esp8266_login_cloud_success==1){
			det_no_wifi_net=0;
			run_t.reconnect_tencent_cloud_flag=0;
			run_t.auto_link_cloud_flag=0xff;
			esp8266_t.linking_tencent_cloud_doing =0;

			SendWifiData_To_Cmd(0x01) ;
			HAL_Delay(30);


			Subscriber_Data_FromCloud_Handler();
			HAL_Delay(300);

			Publish_Data_ToTencent_Update_Data();
			HAL_Delay(300);
		}
	}

}



