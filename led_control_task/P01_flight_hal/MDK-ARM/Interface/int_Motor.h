#ifndef _INT_MOTOR_H_
#define _INT_MOTOR_H_

#include "tim.h"
#include "Com_debug.h"

typedef struct
{
    TIM_HandleTypeDef *tim;
    uint16_t channel;
    uint16_t speed;

}Motor_Struct;

/**
 * @brief 传入参数为比较值  最大为1000  默认值为200 speed=PWM占空比
 * @param Speed 电机速度，单位为RPM`
*/


void Int_Motor_setSpeed(Motor_Struct *motor);


/**
 * @brief 启动电机
 * @param motor 电机结构体指针
 */
void Int_Motor_Start(Motor_Struct *motor);





#endif /* _INT_MOTOR_H_ */
