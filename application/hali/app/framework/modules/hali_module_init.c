#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include "../../include/base/list.h"
#include "../../include/base/hali_base.h"
#include "../../include/base/hali_error.h"
#include "../../include/base/hali_thread.h"
#include "../../include/base/hali_modules.h"




/**
 *  功能描述: 业务模块处理消息
 *  @param param1
 *
 *  @return
 */

ulong hali_module_init(void)
{
    uint moduleid;
    ulong ulerr = ERROR_SUCCESS;

    for(moduleid = HALI_MODULE_BASIC; moduleid < HALI_MODULE_MAX; moduleid++)
    {
        if (NULL != g_moduleinit[moduleid])
        {
            ulerr |= g_moduleinit[moduleid]();
        }
    }

    return ulerr;
}

void hali_module_fini(void)
{

    uint moduleid;
    for(moduleid = HALI_MODULE_BASIC; moduleid < HALI_MODULE_MAX; moduleid++)
    {
        if (NULL != g_modulefini[moduleid])
        {
            (void)g_modulefini[moduleid]();
        }
    }

    return ;
}








#ifdef __cplusplus
}
#endif






