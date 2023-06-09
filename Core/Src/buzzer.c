#include "buzzer.h"
#include "delay.h"


IWDG_HandleTypeDef IWDG_Handler; //�������Ź����

void Buzzer_KeySound(void)
{

  unsigned int m=40;//40;//70;//70;//80;//80
	while(m--){

    HAL_GPIO_TogglePin(BUZZER_GPIO,BUZZER_PIN);//BEEP=!BEEP;
    //delay_us(600);
    //delay_us(500);//500//
     delay_us(500);
    }
       
}

void IWDG_Init(uint8_t prer,uint16_t rlr)
{
    IWDG_Handler.Instance=IWDG;
    IWDG_Handler.Init.Prescaler=prer;	//����IWDG��Ƶϵ��
      IWDG_Handler.Init.Window = rlr;
    IWDG_Handler.Init.Reload=rlr;		//��װ��ֵ
  		//��ʼ��IWDG,Ĭ�ϻῪ���������Ź�	
  if (HAL_IWDG_Init(&IWDG_Handler) != HAL_OK)
  {
    Error_Handler();
  }
}
  

//ι�������Ź�
void IWDG_Feed(void)
{   
    HAL_IWDG_Refresh(&IWDG_Handler); 	//ι��
}



