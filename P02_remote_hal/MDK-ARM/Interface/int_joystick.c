#include "int_joystick.h"






uint16_t adc_buf[4]={0};
/**
 * @brief 初始化ADC摇杆
 */
void Int_Joystick_Init(void)
{
    //开启ADC
    //16位数据的地址值  其实是32位 32位数据的地址值也是32位
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buf, 4);
}




/**
 * @brief 获取摇杆数据
 */
void Int_Joystick_Get(Joystick_t *joystick)
{
    //DMA不依赖于CPU计算  所以读取的数据是实时保存到ADC_BUFF中
    //顺序是自定义的RAN234  要对齐
    joystick->thr=adc_buf[0];

    joystick->yaw=adc_buf[1];

    joystick->pitch=adc_buf[2];

    joystick->roll=adc_buf[3];
}

