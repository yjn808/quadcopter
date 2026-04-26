#ifndef _APP_FREERTOS_TASK_H_
#define _APP_FREERTOS_TASK_H_

#include "FreeRTOS.h"
#include "task.h"
#include "Com_debug.h"
#include "int_IP5305T.h"
#include "int_Motor.h"
#include "int_LED.h"
#include "Com_config.h"

/**
 * @brief 初始化FreeRTOS任务
 */
void App_FreeRTOS_Task_Init(void);

#endif /* _APP_FREERTOS_TASK_H_ */
