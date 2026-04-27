#ifndef __COM_CONFIG_H__
#define __COM_CONFIG_H__

#include "main.h"



//连接状态枚举
typedef enum
{
    REMOTE_CONNECTED,
    REMOTE_DISCONNECTED,

}Remote_State;


//飞行状态枚举
typedef enum
{
    IDLE = 0,//空闲状态
    NORMAL_FLIGHT,//正常飞行状态
    FIX_FLIGHT,//固定飞行状态
    FAILURE,//故障状态
    

}Flight_State;


#endif /* __COM_DEBUG_H__ */
