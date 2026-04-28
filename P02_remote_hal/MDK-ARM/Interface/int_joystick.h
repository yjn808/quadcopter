#ifndef __INT_JOYSTICK__	
#define __INT_JOYSTICK__


#include "adc.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"


typedef struct
{
	int16_t thr;//油门

	int16_t yaw;

	int16_t pitch;

	int16_t roll;

}Joystick_t;



/**
 * @brief 初始化ADC摇杆
 */
void Int_Joystick_Init(void);




/**
 * @brief 获取摇杆数据
 */
void Int_Joystick_Get(Joystick_t *joystick);





#endif

