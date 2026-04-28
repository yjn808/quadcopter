#ifndef __INT_KEY__
#define __INT_KEY__

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"


typedef enum
{
	KEY_NONE = 0,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_LEFT_X,
	KEY_RIGHT_X,
	KEY_RIGHT_X_LONG,
}Key_Type;

/**
 * @brief 获取当前按键是否被按下
 * @return Key_Type Key_NONE 没有按键被按下，其他值都是对应被按下的标记
 */


Key_Type Int_Key_get(void);



#endif

