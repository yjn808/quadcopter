#include "int_Motor.h"





/**
 * @brief 传入参数为比较值  最大为1000  默认值为200 speed=PWM占空比
 * @param Speed 电机速度，单位为RPM`
*/


void Int_Motor_setSpeed(Motor_Struct *motor)
{
    
    if(motor == NULL)
    {
        return;
    }

    if(motor->speed > 1000)
    {
        motor->speed = 1000;
    }

    __HAL_TIM_SET_COMPARE(motor->tim,motor->channel,motor->speed);
    
}


/***********************电机启动函数***********************/
/**
 * @brief 启动电机
 * @param motor 电机结构体指针
 */
void Int_Motor_start(Motor_Struct *motor)

{

    HAL_TIM_PWM_Start(motor->tim, motor->channel);

}







