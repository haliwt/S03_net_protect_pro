#ifndef __PUBLISH_H_
#define __PUBLISH_H_
#include "main.h"

enum pub_Signal {                   /* enumeration for CParser signals */
  tencent_cloud_init,
  tencent_cloud_real_times,
};

typedef enum{

   ptc_temp_warning =0x01,
   fan_warning,
   plasma_warning,
   ultrasonic_warning,


}warning_item;


typedef enum{

    no_warning,
	warning


}warning_state;




void Publish_Data_ToTencent_Initial_Data(void);
void Publish_Data_ToTencent_Update_Data(void);

void Publish_Data_Warning(warning_item whichone,uint8_t warning);

#endif 
