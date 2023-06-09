#include "delay.h"


uint32_t fac_us;	

/* USER CODE BEGIN 2 */
//³õÊ¼»¯ÑÓ³Ùº¯Êý
//µ±Ê¹ÓÃucosµÄÊ±ºò,
//´Ëº¯Êý»á³õÊ¼»¯ucosµÄÊ±ÖÓ½ÚÅÄ
//SYSTICKµÄÊ±ÖÓ¹Ì¶¨ÎªAHBÊ±ÖÓ
//SYSCLK:ÏµÍ³Ê±ÖÓÆµÂÊ, SYSCLK = 24MHz
void delay_init(uint8_t SYSCLK)
{

    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTickÆµÂÊÎªHCLK
	fac_us=SYSCLK;						//²»ÂÛÊÇ·ñÊ¹ÓÃOS,fac_us¶¼ÐèÒªÊ¹ÓÃ
#if SYSTEM_SUPPORT_OS 						//Èç¹ûÐèÒªÖ§³ÖOS.
	reload=SYSCLK;					    //Ã¿ÃëÖÓµÄ¼ÆÊý´ÎÊý µ¥Î»ÎªK	   
	reload*=1000000/delay_ostickspersec;	//
¸ù¾Ýdelay_ostickspersecÉè¶¨Òç³öÊ±¼ä
											//reloadÎª24Î»¼Ä´æÆ÷,×î´óÖµ:16777216,ÔÚ72MÏÂ,Ô¼ºÏ0.233s×óÓÒ	
	fac_ms=1000/delay_ostickspersec;		//´ú±íOS¿ÉÒÔÑÓÊ±µÄ×îÉÙµ¥Î»	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//¿ªÆôSYSTICKÖÐ¶Ï
	SysTick->LOAD=reload; 					//Ã¿1/OS_TICKS_PER_SECÃëÖÐ¶ÏÒ»´Î	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //¿ªÆôSYSTICK
#else
#endif
}								    


//ÑÓÊ±nus
//nus:ÒªÑÓÊ±µÄusÊý.	
//nus:0~190887435(×î´óÖµ¼´2^32/fac_us@fac_us=22.5)	    								   
void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOADµÄÖµ	    	 
	ticks=nus*fac_us; 						//ÐèÒªµÄ½ÚÅÄÊý 
	//delay_osschedlock();					//×èÖ¹OSµ÷¶È£¬·ÀÖ¹´ò¶ÏusÑÓÊ±
	told=SysTick->VAL;        				//¸Õ½øÈëÊ±µÄ¼ÆÊýÆ÷Öµ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//ÕâÀï×¢ÒâÒ»ÏÂSYSTICKÊÇÒ»¸öµÝ¼õµÄ¼ÆÊýÆ÷¾Í¿ÉÒÔÁË.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//Ê±¼ä³¬¹ý/µÈÓÚÒªÑÓ³ÙµÄÊ±¼ä,ÔòÍË³ö.
		}  
	};
	//delay_osschedunlock();					//»Ö¸´OSµ÷¶È											    
}  
//ÑÓÊ±nms
//nms:ÒªÑÓÊ±µÄmsÊý
//nms:0~65535
//void delay_ms(uint16_t nms)
//{	
////	if(delay_osrunning&&delay_osintnesting==0)//Èç¹ûOSÒÑ¾­ÔÚÅÜÁË,²¢ÇÒ²»ÊÇÔÚÖÐ¶ÏÀïÃæ(ÖÐ¶ÏÀïÃæ²»ÄÜÈÎÎñµ÷¶È	    
////	{		 
////		if(nms>=fac_ms)						//ÑÓÊ±µÄÊ±¼ä´óÓÚOSµÄ×îÉÙÊ±¼äÖÜÆÚ 
////		{ 
////   			delay_ostimedly(nms/fac_ms);	//OSÑÓÊ±
////		}
////		nms%=fac_ms;						//OSÒÑ¾­ÎÞ·¨Ìá¹©ÕâÃ´Ð¡µÄÑÓÊ±ÁË,²ÉÓÃÆÕÍ¨·½Ê½ÑÓÊ±    
////	}
//	delay_us((uint32_t)(nms*1000));				//ÆÕÍ¨·½Ê½ÑÓÊ±
//}


//ÑÓÊ±nus
//nusÎªÒªÑÓÊ±µÄusÊý.	
//nus:0~190887435(×î´óÖµ¼´2^32/fac_us@fac_us=22.5)	 
//void delay_us(uint32_t nus)
//{		
//	uint32_t ticks;
//	uint32_t told,tnow,tcnt=0;
//	uint32_t reload=SysTick->LOAD;				//LOADµÄÖµ	    	 
//	ticks=nus*fac_us; 						//ÐèÒªµÄ½ÚÅÄÊý 
//	told=SysTick->VAL;        				//¸Õ½øÈëÊ±µÄ¼ÆÊýÆ÷Öµ
//	while(1)
//	{
//		tnow=SysTick->VAL;	
//		if(tnow!=told)
//		{	    
//			if(tnow<told)tcnt+=told-tnow;	//ÕâÀï×¢ÒâÒ»ÏÂSYSTICKÊÇÒ»¸öµÝ¼õµÄ¼ÆÊýÆ÷¾Í¿ÉÒÔÁË.
//			else tcnt+=reload-tnow+told;	    
//			told=tnow;
//			if(tcnt>=ticks)break;			//Ê±¼ä³¬¹ý/µÈÓÚÒªÑÓ³ÙµÄÊ±¼ä,ÔòÍË³ö.
//		}  
//	};
//}

//ÑÓÊ±nms
//nms:ÒªÑÓÊ±µÄmsÊý
void delay_ms(uint16_t nms)
{
	uint32_t i;
	for(i=0;i<nms;i++) delay_us(1000);
}

