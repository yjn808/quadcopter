#include "App_FreeRTOS_Task.h"

//STM32F103C8T6的SRAM为20KB=>分配12K（configTOTAL_HEAP_SIZE）给操作系统

//最小推荐值128=>128*4=512字节
#define POWER_TASK_STACK_SIZE 128

//任务优先级，数值越大优先级越高（configMAX_PRIORITIES）=> max=4 => 不推荐使用最小优先级0，推荐使用1~3
#define POWER_TASK_PRIORITY 1

TaskHandle_t power_taskHandle;
void Power_Task(void *pvParameters);



void App_FreeRTOS_Task_Init(void)
{
    //1.创建电源管理任务
    xTaskCreate(Power_Task,"power_task",POWER_TASK_STACK_SIZE,NULL,POWER_TASK_PRIORITY,&power_taskHandle);




    //2.启动调度器
    vTaskStartScheduler();



}

void Power_Task(void *pvParameters)
{
    //Power_Task任务启动之后 不断执行的内容
    //获取当前基准时间
    TickType_t xLastWakeTime = xTaskGetTickCount();


    while(1)
    {
        //每10s执行一次 => 启动电源  避免自动关机
        vTaskDelayUntil(&xLastWakeTime,10000);//延时10000ms=>释放CPU占用
        //启动电源
        Int_IP5305T_start();
        

    }
}










