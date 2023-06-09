#include "fan.h"
#include "main.h"
#include "tim.h"
#include "run.h"
#include "delay.h"
#include "tim.h"
#include "gpio.h"


static void SetLevel_Fan_PWMA(uint8_t levelval);



void FAN_CCW_RUN(void)
{
  // FAN_CW_SetLow();
   SetLevel_Fan_PWMA(100);
  
}

void FAN_Stop(void)
{
   FAN_CW_SetLow(); //brake
  //  FAN_GPIO_Init();
  // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,GPIO_PIN_RESET);
   //HAL_TIM_PWM_Stop(&htim16,TIM_CHANNEL_1);
   SetLevel_Fan_PWMA(0);//SetLevel_Fan_PWMA(16);
}

void Fan_One_Power_Off_Speed(void)
{
	// SetLevel_Fan_PWMA(25);
	 FAN_CW_SetLow(); //brake
	 FAN_GPIO_Init();
     //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,GPIO_PIN_SET);
	


}

void Fan_One_Speed(void)
{

	SetLevel_Fan_PWMA(20);


}

void Fan_Two_Speed(void)
{
	SetLevel_Fan_PWMA(22);
	//FAN_CW_SetLow(); //brake
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,GPIO_PIN_SET);
}

 void Fan_Full_Speed(void)
{
    SetLevel_Fan_PWMA(25);
   // FAN_CW_SetLow(); //brake
	///HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,GPIO_PIN_SET);

}


#if 0
void Fan_Two_Speed(void)
{
   static uint16_t fan_speed;
	//static uint16_t  fan_dec;
	fan_speed ++ ;
	
	FAN_CW_SetLow();

	if(fan_speed ==1)  FAN_CCW_SetLow(); //brake//Run fan//brake/Run fan //Run fan//brake //Run fan //brake //Run fan
	
	if(fan_speed % 2 ==0  && fan_speed % 4 !=0 ){
	   FAN_CCW_SetHigh(); //Run fan
    }
    if( fan_speed % 4 ==0 && fan_speed % 2 !=0){
  
	  FAN_CCW_SetLow(); //brake
	  
	}

	if(fan_speed > 10000){
		fan_speed =0;
	    
	     FAN_CCW_SetLow(); //brake
	}

}
#endif 

#if 0
void Fan_One_Speed(void)
{
    static uint16_t fan_speed;
	//static uint16_t  fan_dec;
	fan_speed ++ ;
	
	FAN_CW_SetLow();

	if(fan_speed ==1)  FAN_CCW_SetLow(); //brake//Run fan//brake/Run fan //Run fan//brake //Run fan //brake //Run fan
	
	if(fan_speed % 2 ==0 ){
	   FAN_CCW_SetHigh(); //Run fan
    }
    if( fan_speed % 3 ==0 ){
  
	  FAN_CCW_SetLow(); //brake
	  
	}

	if(fan_speed > 10000){
		fan_speed =0;
	    
	     FAN_CCW_SetLow(); //brake
	}



}
#endif 

#if 0
void Fan_Slowly_Speed(void)
{
    static uint16_t fan_speed;
	fan_speed ++ ;
	FAN_CW_SetLow();
	
	if(fan_speed < 30){
	   FAN_CCW_SetHigh(); //Run fan
    }

	if(fan_speed > 29  && fan_speed < 101){
  
	  FAN_CCW_SetLow(); //brake
	  FAN_CCW_SetHigh(); //Run fan
	  FAN_CCW_SetLow(); //brake

	}

	if(fan_speed > 99){
		 FAN_CCW_SetHigh(); //Run fan
		fan_speed =0;
	 }
	
}
#endif


void ShutDown_AllFunction(void)
{
	
	PLASMA_SetLow(); //
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
	PTC_SetLow();
	FAN_Stop();



}
//"杀毒" 
void SterIlization(uint8_t sel)
{
    if(sel==0){//open 
		
		 PLASMA_SetHigh();
		 HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 

	}
	else{ //close

			
		PLASMA_SetLow();
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic off
	

	}



}

void Dry_Function(uint8_t sel)
{
   if(sel ==0){

  
		//FAN_CCW_RUN();
		PTC_SetHigh();

   }
   else{

       PTC_SetLow();

   }

}

void Fan_RunSpeed_Fun(void)
{
         if(run_t.set_wind_speed_value < 34 ){
              Fan_One_Speed();
		 }
		 else if(run_t.set_wind_speed_value > 33  && run_t.set_wind_speed_value < 67 ){

             Fan_Two_Speed();

		 }
		 else if(run_t.set_wind_speed_value > 66)
		 	Fan_Full_Speed();


}
/********************************************************
*
*Function Name:void SetLevel_Fan_PWMA(uint8_t levelval)
*Function: 
*
*
********************************************************/
static void SetLevel_Fan_PWMA(uint8_t levelval)
{
     run_t.gFan_pwm_duty_level = levelval;
     FAN_CW_SetLow();
	 MX_TIM16_Init();
	 HAL_TIM_PWM_Start(&htim16,TIM_CHANNEL_1);
}


