#include "special_power.h"
#include "dht11.h"
#include "fan.h"
#include "tim.h"
#include "cmd_link.h"
#include "run.h"
#include "wifi_fun.h"
#include "esp8266.h"
#include "buzzer.h"
#include "subscription.h"
#include "publish.h"

#include "mqtt_iot.h"


uint8_t power_on_doing_step;

void (*Single_Usart_ReceiveData)(uint8_t cmd);

void SetPowerOn_ForDoing(void)
{
    

    run_t.gPower_flag = POWER_ON;
   
    run_t.gPower_On=POWER_ON;
	run_t.gmt_time_flag=0;


	if(run_t.app_timer_power_on_flag==0){
	
        run_t.gModel=1;
	    run_t.gFan = 1;
		run_t.gDry = 1;
		run_t.gPlasma =1;       //"杀菌"
		run_t.gUlransonic = 1; // "驱虫"
	    run_t.gFan_counter=0;

       }
  

	
 }

void Tencen_Cloud_Timer_Power_On(void)
{
    
               run_t.gModel =1;
            
    
              Parse_Json_Statement();
    
        
              if( run_t.gPlasma==1){ //Anion
                  run_t.gPlasma=1;
    
                  SendWifiCmd_To_Order(WIFI_KILL_ON);
                 // HAL_Delay(1);
              }
              else{
                  run_t.gPlasma =0;
                  SendWifiCmd_To_Order(WIFI_KILL_OFF);
                 /// HAL_Delay(1);
              }
    
    
              if(run_t.gUlransonic==1){
    
                      SendWifiCmd_To_Order(WIFI_SONIC_ON);
                     // HAL_Delay(1);
              }
              else {
                      run_t.gUlransonic=0;
                      SendWifiCmd_To_Order(WIFI_SONIC_OFF);
                    //  HAL_Delay(1);
              }
    
    
    
              if(run_t.gDry==1){
    
                  SendWifiCmd_To_Order(WIFI_PTC_ON);
                 /// HAL_Delay(1);
              }
              else{
                      run_t.gDry=0;
                      SendWifiCmd_To_Order(WIFI_PTC_OFF);
                     // HAL_Delay(1);
    
              }
           
        
               run_t.set_temperature_value=40;
               run_t.set_wind_speed_value =100;
           
              

}

void SetPowerOff_ForDoing(void)
{
   
	run_t.gPower_flag = 0; //bool 

	run_t.gPower_On = POWER_OFF;

    run_t.set_wind_speed_value =10;
 
    run_t.gFan = 0;
    run_t.gDry = 0;
	run_t.gPlasma =0;       //"杀菌"
	run_t.gUlransonic = 0; // "驱虫"
	run_t.gModel =1;
    power_on_doing_step=0;

    
	PLASMA_SetLow(); //
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic Off 
	PTC_SetLow();
	FAN_Stop();


}

void Single_Usart_RxData(void(*rxHandler)(uint8_t dat))
{

      Single_Usart_ReceiveData=  rxHandler;

}

void ActionEvent_Handler(void)
{
     

if(run_t.works_break_power_on==0) { 

    
	if(run_t.gDry == 1 && run_t.ptc_warning ==0){

	   PTC_SetHigh();

	}
	else{
		   PTC_SetLow();
		  
		   
	}
	//kill
	if(run_t.gPlasma == 1){
		
	     PLASMA_SetHigh();
	}
	else{

		PLASMA_SetLow();
	}
	//driver bug
	if(run_t.gUlransonic ==1){
	
	 
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
	}
	else{
	  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic off

	}

	if(run_t.gPlasma ==0 && run_t.gDry==0){

        run_t.gFan_counter=0;
		       

	}

	Fan_RunSpeed_Fun();
		
   }


}


