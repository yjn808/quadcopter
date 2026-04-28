#include "int_key.h"



/**
 * @brief 获取当前按键是否被按下
 * @return Key_Type Key_NONE 没有按键被按下，其他值都是对应被按下的标记
 */


Key_Type Int_Key_get(void)
{
    if(HAL_GPIO_ReadPin(KEY_UP_GPIO_Port,KEY_UP_Pin)==GPIO_PIN_RESET)
    {
        //1.电弧抖动 =>需要进行消抖
        vTaskDelay(5);
        if(HAL_GPIO_ReadPin(KEY_UP_GPIO_Port,KEY_UP_Pin)==GPIO_PIN_RESET)
        {
            //2.人按下的时间通常比较长 => 为了不被多次判断 =>等待抬起按键才返回
            while(HAL_GPIO_ReadPin(KEY_UP_GPIO_Port,KEY_UP_Pin)==GPIO_PIN_RESET)
            {
                vTaskDelay(1);
            }
            //KEY_UP被按下
            return KEY_UP;
        }
        
    }
    else if(HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port,KEY_DOWN_Pin)==GPIO_PIN_RESET)
    {
        vTaskDelay(5);
        if(HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port,KEY_DOWN_Pin)==GPIO_PIN_RESET)
        {   
            while(HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port,KEY_DOWN_Pin)==GPIO_PIN_RESET)
            {
                vTaskDelay(1);
            }
            return KEY_DOWN;

        }
    }
    else if(HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port,KEY_LEFT_Pin)==GPIO_PIN_RESET)
    {
        vTaskDelay(5);
        if(HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port,KEY_LEFT_Pin)==GPIO_PIN_RESET)
        {   
            while(HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port,KEY_LEFT_Pin)==GPIO_PIN_RESET)
            {
                vTaskDelay(1);
            }
            return KEY_LEFT;

        }
    }
    else if(HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port,KEY_RIGHT_Pin)==GPIO_PIN_RESET)
    {
        vTaskDelay(5);
        if(HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port,KEY_RIGHT_Pin)==GPIO_PIN_RESET)
        {   
            while(HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port,KEY_RIGHT_Pin)==GPIO_PIN_RESET)
            {
                vTaskDelay(1);
            }
            return KEY_RIGHT;

        }
    }

    //左上按键
    else if(HAL_GPIO_ReadPin(KEY_LEFT_X_GPIO_Port,KEY_LEFT_X_Pin) == GPIO_PIN_RESET)
    {
        vTaskDelay(5);
        if(HAL_GPIO_ReadPin(KEY_LEFT_X_GPIO_Port,KEY_LEFT_X_Pin) == GPIO_PIN_RESET)
        {
            while(HAL_GPIO_ReadPin(KEY_LEFT_X_GPIO_Port,KEY_LEFT_X_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(1);
            }

            return KEY_LEFT_X;
        }
    }


    //右上按键
    else if(HAL_GPIO_ReadPin(KEY_RIGHT_X_GPIO_Port,KEY_RIGHT_X_Pin) == GPIO_PIN_RESET)
    {
        //此时开始计时 => 长按为超时1s
        TickType_t count1=xTaskGetTickCount();
        vTaskDelay(5);
        if(HAL_GPIO_ReadPin(KEY_RIGHT_X_GPIO_Port,KEY_RIGHT_X_Pin) == GPIO_PIN_RESET)
        {
            while(HAL_GPIO_ReadPin(KEY_RIGHT_X_GPIO_Port,KEY_RIGHT_X_Pin) == GPIO_PIN_RESET);
            {
                //1ms的时间 cpu是释放的
                vTaskDelay(1);
            }
            TickType_t count2=xTaskGetTickCount();
            if(count2-count1 > 1000)
            {
                return KEY_RIGHT_X_LONG;
            }
            else
            {
                return KEY_RIGHT_X;
            }
        }
    }

    return KEY_NONE;
}


















