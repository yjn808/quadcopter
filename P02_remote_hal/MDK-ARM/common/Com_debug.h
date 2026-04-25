#ifndef __COM_DEBUG_H__
#define __COM_DEBUG_H__

#include "usart.h"
#include "stdio.h"
#include <string.h>

//使用宏定义的方式  只打印文件名称，不打印路径名称
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define __FILE__NAME__ (strrchr(__FILENAME__, '/') ? strrchr(__FILENAME__, '/') + 1 : __FILENAME__)

//日志输出打印非常占用CPU资源，使用宏定义的方式控制日志输出=>通过比特率计算得出 打印10字节需要1ms，影响飞行器飞行
#define LOG_PRINTF_ON 1

#ifdef LOG_PRINTF_ON

//使用宏定义的方式重定向printf函数，增加文件名和行号
#define DEBUG_PRINTF(format, ...)  printf("[%s:%d]" format, __FILE__NAME__, __LINE__, ##__VA_ARGS__)

#else

//如果不需要日志输出，定义一个空的宏,相同名称但是空
#define DEBUG_PRINTF(format, ...)

#endif /* LOG_PRINTF_ON */

#endif /* __COM_DEBUG_H__ */
