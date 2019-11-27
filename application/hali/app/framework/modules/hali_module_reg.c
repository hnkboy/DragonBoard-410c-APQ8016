#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include "../../include/base/list.h"
#include "../../include/base/hali_base.h"
#include "../../include/base/hali_error.h"
#include "../../include/base/hali_thread.h"
#include "../../include/base/hali_modules.h"

HALI_MODULE_MSGPROC_PF g_svrmsgproc[HALI_MODULE_MAX];
HALI_MODULE_MSGPROC_PF g_workmsgproc[HALI_MODULE_MAX];
HALI_MODULE_MSGPROC_PF g_mainmsgproc[HALI_MODULE_MAX];

HALI_MODULE_REGINIT_PF g_moduleinit[HALI_MODULE_MAX];

HALI_MODULE_REGFINI_PF g_modulefini[HALI_MODULE_MAX];

HALI_MODULE_MQTTPROC_PF g_mqttmsgproc[HALI_MODULE_MAX];
HALI_MODULE_MQTTSUB_PF  g_mqttsub[HALI_MODULE_MAX];



/**
 *  功能描述: 业务模块处理消息
 *  @param param1
 *
 *  @return
 */

void hali_module_regsvrthrmsgproc(uint moduletype, HALI_MODULE_MSGPROC_PF pfmsgcallback)
{


    g_svrmsgproc[moduletype] = pfmsgcallback;

    return ;
}

void hali_module_unregsvrthrmsgproc(uint moduletype)
{


    g_svrmsgproc[moduletype] = NULL;

    return ;
}

/**
 *  功能描述: 业务模块处理消息
 *  @param param1
 *
 *  @return
 */

void hali_module_regworkthrmsgproc(uint moduletype, HALI_MODULE_MSGPROC_PF pfmsgcallback)
{


    g_workmsgproc[moduletype] = pfmsgcallback;

    return ;
}

void hali_module_unregworkthrmsgproc(uint moduletype)
{


    g_workmsgproc[moduletype] = NULL;

    return ;
}



/**
 *  功能描述: 业务模块处理消息
 *  @param param1
 *
 *  @return
 */

void hali_module_regmainthrmsgproc(uint moduletype, HALI_MODULE_MSGPROC_PF pfmsgcallback)
{


    g_mainmsgproc[moduletype] = pfmsgcallback;

    return ;
}

void hali_module_unregmainthrmsgproc(uint moduletype)
{


    g_mainmsgproc[moduletype] = NULL;

    return ;
}



/**
 *  功能描述: 业务模块处理消息
 *  @param param1
 *
 *  @return
 */

void hali_module_regmodinit(uint moduletype, HALI_MODULE_REGINIT_PF pfunc)
{


    g_moduleinit[moduletype] = pfunc;

    return ;
}

void hali_module_unregmodinit(uint moduletype)
{


    g_moduleinit[moduletype] = NULL;

    return ;
}
void hali_module_regmodfini(uint moduletype, HALI_MODULE_REGFINI_PF pfunc)
{
    g_modulefini[moduletype] = pfunc;
    return ;
}

void hali_module_unregmodfini(uint moduletype)
{
    g_modulefini[moduletype] = NULL;
    return ;
}

void hali_module_regmqttmsgproc(uint moduletype, HALI_MODULE_MQTTPROC_PF pfunc)
{
    g_mqttmsgproc[moduletype] = pfunc;
    return ;
}

void hali_module_unregmqttmsgproc(uint moduletype)
{
    g_mqttmsgproc[moduletype] = NULL;
    return ;
}

void hali_module_regmqttsubproc(uint moduletype, HALI_MODULE_MQTTSUB_PF pfunc)
{
    g_mqttsub[moduletype] = pfunc;
    return ;
}

void hali_module_unregmqttsubproc(uint moduletype)
{
    g_mqttsub[moduletype] = NULL;
    return ;
}


#ifdef __cplusplus
}
#endif





