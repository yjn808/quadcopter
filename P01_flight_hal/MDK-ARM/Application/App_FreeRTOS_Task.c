#include "App_FreeRTOS_Task.h"
#include "int_SI24R1.h"


//内存管理=>C语言中的结构体通常保存在堆中  不会回收 =>一个结构体循环使用
Motor_Struct Left_Top={
    .tim = &htim3,
    .channel = TIM_CHANNEL_1,
    .speed = 200
};

Motor_Struct Left_Bottom={
    .tim = &htim4,
    .channel = TIM_CHANNEL_4,
    .speed = 200
};

Motor_Struct Right_Top={
    .tim = &htim2,
    .channel = TIM_CHANNEL_2,
    .speed = 200
};

Motor_Struct Right_Bottom={
    .tim = &htim1,
    .channel = TIM_CHANNEL_3,
    .speed = 200
};



//LED灯结构体
LED_Struct left_top_led={.port=LED1_GPIO_Port,.pin=LED1_Pin};
LED_Struct right_top_led={.port=LED2_GPIO_Port,.pin=LED2_Pin};
LED_Struct right_bottom_led={.port=LED3_GPIO_Port,.pin=LED3_Pin};
LED_Struct left_bottom_led={.port=LED4_GPIO_Port,.pin=LED4_Pin};


//表示当前遥控状态
Remote_State remote_state = REMOTE_DISCONNECTED;

//表示当前飞行状态
Flight_State flight_state = IDLE;



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

//灯控任务
void LED_Task(void *pvParameters);
#define LED_TASK_STACK_SIZE 128
#define LED_TASK_PRIORITY 1
TaskHandle_t led_taskHandle;
#define LED_TASK_PERIOD 100

//通讯任务
void Communication_Task(void *pvParameters);
#define COMMUNICATION_TASK_STACK_SIZE 128
#define COMMUNICATION_TASK_PRIORITY 2
TaskHandle_t communication_taskHandle;
#define COMMUNICATION_TASK_PERIOD 6




void App_FreeRTOS_Task_Init(void)
{
    //1.创建电源管理任务
    xTaskCreate(Power_Task,"power_task",POWER_TASK_STACK_SIZE,NULL,POWER_TASK_PRIORITY,&power_taskHandle);

    //2.创建飞行任务
    xTaskCreate(Flight_Task,"flight_task",FLIGHT_TASK_STACK_SIZE,NULL,FLIGHT_TASK_PRIORITY,&flight_taskHandle);

    //3.创建LED灯控任务
    xTaskCreate(LED_Task,"LED_task",LED_TASK_STACK_SIZE,NULL,LED_TASK_PRIORITY,&led_taskHandle);

    //4.创建通讯任务
    xTaskCreate(Communication_Task,"communication_task",COMMUNICATION_TASK_STACK_SIZE,NULL,COMMUNICATION_TASK_PRIORITY,&communication_taskHandle);


    //5.启动调度器
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


void LED_Task(void *pvParameters)
{
    //LED_Task任务启动之后 不断执行的内容
    //获取当前基准时间
    TickType_t xLastWakeTime = xTaskGetTickCount();
    uint8_t count = 0;
    while(1)
    {

        count++;
        //1.判断当前连接状态:前两个灯
        if(remote_state == REMOTE_CONNECTED)
        {
            //连接状态=>点亮LED
            Int_LED_on(&left_top_led);
            Int_LED_on(&right_top_led);
        }
        else if(remote_state == REMOTE_DISCONNECTED)
        {
            //断开状态=>熄灭LED
            Int_LED_off(&left_top_led);
            Int_LED_off(&right_top_led);
        }

        //2.飞行状态：后两个灯
        //闪烁循环时间怎么设置符合要求？LED_Task任务的周期是100ms（LED_TASK_PERIOD），设置要符合慢闪和快闪的最大公约数 => 100ms => 慢闪500ms=>5次反转，快闪200ms=>2次反转
        if(flight_state == IDLE)
        {
            //空闲状态=>灯慢闪烁=>500ms反转一次
            if(count % 5 == 0)
            {
                //循环5次反转一次 一次是100ms => 5*100ms=500ms
                Int_LED_toggle(&left_bottom_led);
                Int_LED_toggle(&right_bottom_led);
            }
        }
        else if(flight_state == NORMAL_FLIGHT)
        {
            //飞行状态=>灯快闪烁=>200ms反转一次
            if(count % 2 == 0)
            {
                //循环2次反转一次 一次是100ms => 2*100ms=200ms
                Int_LED_toggle(&left_bottom_led);
                Int_LED_toggle(&right_bottom_led);
            }
        }
        
        else if(flight_state == FIX_FLIGHT){
            //固定飞行状态=>后两个灯常亮
            Int_LED_on(&left_bottom_led);
            Int_LED_on(&right_bottom_led);

        }
        else if(flight_state == FAILURE)
        {
            //故障状态=>后两个灯灭
            Int_LED_off(&left_bottom_led);
            Int_LED_off(&right_bottom_led);
        }



        //将count计数重置
        if(count >= 10)
        {
            count = 0;
        }


        vTaskDelayUntil(&xLastWakeTime,LED_TASK_PERIOD);//延时1000ms=>释放CPU占用  让当前任务按固定周期运行，并在等待期间释放 CPU 给其他任务
    }
}



uint8_t communication_buffer[TX_PLOAD_WIDTH]={0};
void Communication_Task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while(1)
    {
        
        //1.接收数据到缓冲区
        uint8_t res=Int_SI24R1_RxPacket(communication_buffer);
        if(res==0)
        {
            DEBUG_PRINTF("接收数据成功:%s\r\n",communication_buffer);
        }
        //6ms执行一次 接收数据的时间间隔=发送数据的时间间隔
        vTaskDelayUntil(&xLastWakeTime,COMMUNICATION_TASK_PERIOD);

    }

}




