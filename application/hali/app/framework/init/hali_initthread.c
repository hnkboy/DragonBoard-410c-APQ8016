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

#define HALI_WORKTHRDNUM_MAX       2
#define HALI_SVRTHRDNUM_MAX        2


uint g_uitollthrdnum = 0;




/*svr线程消息处理*/
ulong hali_inithread_svrmsgproc(char *pucbuf, int ilen)
{
    ulong ulerrcode = ERROR_FAILED;

    uint uimodtype;

    for(uimodtype = HALI_MODULE_BASIC; uimodtype < HALI_MODULE_MAX; uimodtype++)
    {
        if (NULL != g_svrmsgproc[uimodtype])
        {
            ulerrcode |= g_svrmsgproc[uimodtype](pucbuf, ilen);
        }
    }
    return ulerrcode;
}

/*svr线程消息处理*/
ulong hali_inithread_workmsgproc(char *pucbuf, int ilen)
{
    ulong ulerrcode = ERROR_FAILED;

    uint uimodtype;

    for(uimodtype = HALI_MODULE_BASIC; uimodtype < HALI_MODULE_MAX; uimodtype++)
    {
        if (NULL != g_workmsgproc[uimodtype])
        {
            ulerrcode |= g_workmsgproc[uimodtype](pucbuf, ilen);
        }
    }
    return ulerrcode;
}






/*线程的初始化*/
ulong hali_inithread_init()
{

    ulong ulerrcode = ERROR_SUCCESS;
    ulong ulindex = 0;

    /*启动工作线程*/
    for (ulindex = 0; ulindex < HALI_WORKTHRDNUM_MAX; ulindex++)
    {
        ulerrcode +=  hali_thread_create(HALI_THREADTYPE_WORK,
                                        ulindex,
                                        hali_workthread_init,
                                        hali_inithread_workmsgproc,
                                        hali_workthread_fini);
    }

    /*启动业务线程*/
    for (ulindex = 0; ulindex < HALI_SVRTHRDNUM_MAX; ulindex++)
    {
        ulerrcode |= hali_thread_create(HALI_THREADTYPE_SVR,
                                        ulindex,
                                        hali_svrthread_init,
                                        hali_inithread_svrmsgproc,
                                        hali_svrthread_fini);

    }

    if (ERROR_SUCCESS == ulerrcode)
    {
        g_uitollthrdnum = HALI_WORKTHRDNUM_MAX + HALI_SVRTHRDNUM_MAX;
    }

    return ulerrcode;
}

/*线程的初始化*/
void hali_inithread_fini()
{


}


#ifdef __cplusplus
}
#endif



