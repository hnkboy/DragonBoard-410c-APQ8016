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
#include "../../include/hali_zigbee.h"



/**
 *  功能描述: 业务模块处理消息
 *  @param param1
 *
 *  @return
 */

void hali_hanging_init(void)
{
    hali_module_regmainthrmsgproc(HALI_MODULE_BASIC, hali_mainthread_msgproc);
    hali_module_regsvrthrmsgproc(HALI_MODULE_BASIC, hali_svrthread_msgproc);
    hali_module_regworkthrmsgproc(HALI_MODULE_BASIC, hali_workthread_msgproc);

    hali_module_regmodinit(HALI_MODULE_ZIGBEE, zigbee_init);
    hali_module_regmodfini(HALI_MODULE_ZIGBEE, zigbee_fini);


    return ;
}

void hali_hanging_fini(void)
{

    hali_module_unregmainthrmsgproc(HALI_MODULE_BASIC);
    hali_module_unregsvrthrmsgproc(HALI_MODULE_BASIC);
    hali_module_unregworkthrmsgproc(HALI_MODULE_BASIC);

    hali_module_unregmodinit(HALI_MODULE_ZIGBEE);
    hali_module_unregmodfini(HALI_MODULE_ZIGBEE);
    return ;
}








#ifdef __cplusplus
}
#endif







