#include "App_FreeRTOS_Task.h"



//内存管理=>C语言中的结构体通常保存在堆中  不会回收 =>一个结构体循环使用
static Motor_Struct Left_Top={
    .tim = &htim3,
    .channel = TIM_CHANNEL_1,
    .speed = 200
};

static Motor_Struct Left_Bottom={
    .tim = &htim4,
    .channel = TIM_CHANNEL_4,
    .speed = 200
};

static Motor_Struct Right_Top={
    .tim = &htim2,
    .channel = TIM_CHANNEL_2,
    .speed = 200
};

static Motor_Struct Right_Bottom={
    .tim = &htim1,
    .channel = TIM_CHANNEL_3,
    .speed = 200
};


//STM32F103C8T6的SRAM为20KB=>分配12K（configTOTAL_HEAP_SIZE）给操作系统

//最小推荐值128=>128*4=512字节
#define POWER_TASK_STACK_SIZE 128

//任务优先级，数值越大优先级越高（configMAX_PRIORITIES）=> max=4 => 不推荐使用最小优先级0，推荐使用1~3
#define POWER_TASK_PRIORITY 1

TaskHandle_t power_taskHandle;
void Power_Task(void *pvParameters);
#define POWER_TASK_PERIOD 10000


//飞行任务·
void Flight_Task(void *pvParameters);
#define FLIGHT_TASK_STACK_SIZE 128
#define FLIGHT_TASK_PRIORITY 3
TaskHandle_t flight_taskHandle;
//定义任务周期
#define FLIGHT_TASK_PERIOD 6




void App_FreeRTOS_Task_Init(void)
{
    //1.创建电源管理任务
    xTaskCreate(Power_Task,"power_task",POWER_TASK_STACK_SIZE,NULL,POWER_TASK_PRIORITY,&power_taskHandle);

    //2.创建飞行任务
    xTaskCreate(Flight_Task,"flight_task",FLIGHT_TASK_STACK_SIZE,NULL,FLIGHT_TASK_PRIORITY,&flight_taskHandle);




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
        vTaskDelayUntil(&xLastWakeTime,POWER_TASK_PERIOD);//延时10000ms=>释放CPU占用
        //启动电源
        Int_IP5305T_start();
        

    }
}

void Flight_Task(void *pvParameters)
{
    //Flight_Task任务启动之后 不断执行的内容
    //获取当前基准时间

    //1.设置电机转速
    Left_Top.speed = 400;



    //2.直接启动电机
//    Int_Motor_start(&Left_Top);

    TickType_t xLastWakeTime = xTaskGetTickCount();
}










