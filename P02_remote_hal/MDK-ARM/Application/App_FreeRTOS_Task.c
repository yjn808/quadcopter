#include "App_FreeRTOS_Task.h"
#include "int_SI24R1.h"
#include "int_key.h"


//遥杆数据结构体
Joystick_t joystick={0,0,0,0}; 



//STM32F103C8T6的SRAM为20KB=>分配12K（configTOTAL_HEAP_SIZE）给操作系统

//最小推荐值128=>128*4=512字节
#define POWER_TASK_STACK_SIZE 128

//任务优先级，数值越大优先级越高（configMAX_PRIORITIES）=> max=4 => 不推荐使用最小优先级0，推荐使用1~3
#define POWER_TASK_PRIORITY 4

TaskHandle_t power_taskHandle;
void Power_Task(void *pvParameters);
#define POWER_TASK_PERIOD 10000




//按键任务、
void key_Task(void *pvParameters);
#define KEY_TASK_STACK_SIZE 128
#define KEY_TASK_PRIORITY 2
TaskHandle_t key_task_handle;
#define KEY_TASK_PERIOD 20

//摇杆任务
void joystick_Task(void *pvParameters);
#define JOYSTICK_TASK_STACK_SIZE 128
#define JOYSTICK_TASK_PRIORITY 2
TaskHandle_t joystick_task_handle;
#define JOYSTICK_TASK_PERIOD 20



//通讯任务
void Com_Task(void *pvParameters);
#define COM_TASK_STACK_SIZE 128
#define COM_TASK_PRIORITY 3
TaskHandle_t com_task_handle;
//任务周期
#define COM_TASK_PERIOD 6







void App_FreeRTOS_Task_Init(void)
{
    //1.创建电源管理任务
    xTaskCreate(Power_Task,"power_task",POWER_TASK_STACK_SIZE,NULL,POWER_TASK_PRIORITY,&power_taskHandle);

    //2.创建通讯任务
    xTaskCreate(Com_Task,"Com_task",COM_TASK_STACK_SIZE,NULL,COM_TASK_PRIORITY,&com_task_handle);

    //按键任务
    xTaskCreate(key_Task,"key_task",KEY_TASK_STACK_SIZE,NULL,KEY_TASK_PRIORITY,&key_task_handle);

    //摇杆任务
    xTaskCreate(joystick_Task,"joystick_task",JOYSTICK_TASK_STACK_SIZE,NULL,JOYSTICK_TASK_PRIORITY,&joystick_task_handle);

    //3.启动调度器
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


void key_Task(void *pvParameters)
{
    TickType_t xLastWakeTime=xTaskGetTickCount();
    while(1)
    {
        Key_Type key=Int_Key_get();
        if(key != KEY_NONE)
        {
            DEBUG_PRINTF("key=%d\n",key);
        }


        vTaskDelayUntil(&xLastWakeTime,KEY_TASK_PERIOD);
    }
}


void joystick_Task(void *pvParameters)
{
    TickType_t xLastWakeTime=xTaskGetTickCount();
    //1.初始化摇杆
    Int_Joystick_Init();
    while(1)
    {
        //1.获取摇杆的值
        Int_Joystick_Get(&joystick);
        //2.打印摇杆的值
        DEBUG_PRINTF("joystick=%d,%d,%d,%d\n",joystick.thr,joystick.yaw,joystick.pitch,joystick.roll);



        vTaskDelayUntil(&xLastWakeTime,KEY_TASK_PERIOD);
    }

}




uint8_t com_buff[TX_PLOAD_WIDTH]={0};
void Com_Task(void *pvParameters)
{
    //获取当前基准时间
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while(1)
    {
        //调用SI24R1的接口 发送数据
        //1.进入TX模式
        Int_SI24R1_TX_Mode();

        //2.发送数据
        Int_SI24R1_TxPacket(com_buff);

        //3.恢复到RX模式
        Int_SI24R1_RX_Mode();


        //每6s执行一次
        vTaskDelayUntil(&xLastWakeTime,COM_TASK_PERIOD);
    }
}








