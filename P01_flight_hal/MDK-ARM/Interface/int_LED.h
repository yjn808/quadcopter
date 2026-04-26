#ifndef _INT_LED_H_
#define _INT_LED_H_



#include "main.h"

typedef struct
{
    GPIO_TypeDef * port;
    uint16_t pin;

}LED_Struct;


/**
 * @brief 打开LED
 */
void Int_LED_on(LED_Struct *Led);


/**
 * @brief 关闭LED
 */

void Int_LED_off(LED_Struct *Led);

/**
 * @brief 反转引脚
 */

void Int_LED_toggle(LED_Struct *Led);


#endif /* _INT_LED_H_ */
