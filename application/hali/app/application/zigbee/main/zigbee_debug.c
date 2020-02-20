#ifdef __cplusplus
extern "C"{
#endif


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <signal.h>
#include <pthread.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <sys/stat.h>        /* For mode constants */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <mosquitto.h>

#include <stdbool.h>


#include "../../../include/base/hali_base.h"
#include "../../../include/base/list.h"
#include "../../../include/base/hali_error.h"
#include "../../../include/base/hali_thread.h"
#include "../../../include/base/hali_epoll.h"
#include "../../../include/base/hali_timer.h"
#include "../../../include/base/hali_tlv.h"
#include "../../../scenarios/include/hali_mosquitto.h"


#include "../include/zigbee.h"



uint g_debugswich = (1 << ZIGBEE_DEBUG_ERROR);

char *g_debugstr[ZIGBEE_DEBUG_MAX]=
{

	[ZIGBEE_DEBUG_PKT]      = "[zibgee][pkt]\n",
    [ZIGBEE_DEBUG_ERROR]    = "[zibgee][error]\n",
    [ZIGBEE_DEBUG_EVENT]    = "[zibgee][event]\n",

};



API void zigbee_setdebugsw(ushort type, BOOL_T bswitch)
{
    if (BOOL_TRUE == bswitch)
    {
        g_debugswich |=  (1 << type);
    }
    else
    {
        g_debugswich &=  ~(1 << type);
    }
}
API void zigbee_setdebug(char *pstr)
{

    uchar *cmpstr = NULL;


    if (NULL != (cmpstr = strstr(pstr, "pkt")))
    {
        if (NULL != strstr(cmpstr, "on"))
        {
           zigbee_setdebugsw(ZIGBEE_DEBUG_PKT, BOOL_TRUE);
        }
        else if(NULL != strstr(cmpstr, "off"))
        {
            zigbee_setdebugsw(ZIGBEE_DEBUG_PKT, BOOL_FALSE);

        }
    }
    else if (NULL != (cmpstr = strstr(pstr, "event")))
    {
        if (NULL != strstr(cmpstr, "on"))
        {
           zigbee_setdebugsw(ZIGBEE_DEBUG_EVENT, BOOL_TRUE);
        }
        else if(NULL != strstr(cmpstr, "off"))
        {
            zigbee_setdebugsw(ZIGBEE_DEBUG_EVENT, BOOL_FALSE);

        }
    }
    else if (NULL != (cmpstr = strstr(pstr, "error")))
    {
        if (NULL != strstr(cmpstr, "on"))
        {
           zigbee_setdebugsw(ZIGBEE_DEBUG_ERROR, BOOL_TRUE);
        }
        else if(NULL != strstr(cmpstr, "off"))
        {
            zigbee_setdebugsw(ZIGBEE_DEBUG_ERROR, BOOL_FALSE);

        }
    }
    #if 0
    if ((g_debugswich) & (1 << ZIGBEE_DEBUG_PKT))
    {
        zigbee_setdebugsw(ZIGBEE_DEBUG_PKT, BOOL_FALSE);
    }
    else
    {
        zigbee_setdebugsw(ZIGBEE_DEBUG_PKT, BOOL_TRUE);
    }
    #endif

    return;
}


API void zigbee_debug(ushort type,const char *cmd, ...)
{
    if ((g_debugswich) & (1 << type))
    {
        printf("%s %s ", __DATE__, __TIME__);
        printf("%s",g_debugstr[type]);
        va_list args;       //定义一个va_list类型的变量，用来储存单个参数
        va_start(args,cmd); //使args指向可变参数的第一个参数
        vprintf(cmd,args);  //必须用vprintf等带V的
        va_end(args);       //结束可变参数的获取
        printf("\n");
    }
}







#ifdef __cplusplus
}
#endif





