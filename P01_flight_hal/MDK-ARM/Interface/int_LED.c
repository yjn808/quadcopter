#include "int_LED.h"




void Int_LED_on(LED_Struct *Led)
{
    //直接修改引脚电平为低电平
    HAL_GPIO_WritePin(Led->port,Led->pin,GPIO_PIN_RESET);



}


/**
 * @brief 关闭LED
 */

void Int_LED_off(LED_Struct *Led)
{
    HAL_GPIO_WritePin(Led->port,Led->pin,GPIO_PIN_SET);



}


/**
 * @brief 反转引脚
 */

void Int_LED_toggle(LED_Struct *Led)
{
    HAL_GPIO_TogglePin(Led->port,Led->pin);
    
}

