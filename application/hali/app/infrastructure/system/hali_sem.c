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
#include <semaphore.h>
#include <errno.h>

#include "../../include/base/hali_base.h"
#include "../../include/base/hali_error.h"

#include "../../include/base/hali_sem.h"





sem_t g_thrdSyn;  /* 主线程控制*/

/**
 *  功能描述:
 *  @param param1
 *  @param param2
 *  @param param3
 *
 *  @return
 */

API ulong hali_sem_init()
{
    int iret;
    ulong ulret = ERROR_FAILED;
    iret = sem_init(&g_thrdSyn, 0, 0);
    if (0 == iret)
    {
        ulret = ERROR_SUCCESS;
    }
    return ulret;
}
/**
 *  功能描述:
 *  @param param1
 *  @param param2
 *  @param param3
 *
 *  @return
 */

API void hali_sem_fini()
{
    (void)sem_destroy(&g_thrdSyn);
    return;
}

API void hali_sem_post(IN HALI_SEMTYPE_E esemtype)
{
    sem_t *psemtemp = NULL;
    switch (esemtype)
    {
        case HALI_SEM_THRDSYN:
        {
            psemtemp = &g_thrdSyn;
            break;
        }
        default :break;
    }
    (void)sem_post(psemtemp);
    return ;
}
API void hali_sem_wait(IN HALI_SEMTYPE_E esemtype)
{
    sem_t *psemtemp = NULL;
    switch (esemtype)
    {
        case HALI_SEM_THRDSYN:
        {
            psemtemp = &g_thrdSyn;
            break;
        }
        default:break;
    }
    (void)sem_wait(psemtemp);
    return ;
}


#ifdef __cplusplus
}
#endif




