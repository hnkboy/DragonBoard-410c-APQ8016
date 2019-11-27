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
#include "../../include/base/hali_threadmsg.h"
#include "../../include/base/hali_sem.h"


/*work线程去初始*/
void hali_svrthread_fini(void *pthreadnode)
{
    HALI_THREAD_NODE_S *pstthrdnode = (HALI_THREAD_NODE_S *)pthreadnode;
    hali_threadmgr_del4list(pstthrdnode->uithreadseqnum);
    hali_threadmgr_destroy(pstthrdnode);
    return;
}
/*work线程初始化*/
void *hali_svrthread_init(void *pthreadnode)
{
    ulong ulerrcode = ERROR_FAILED;
    HALI_THREAD_NODE_S *pstthrdnode = (HALI_THREAD_NODE_S *)pthreadnode;
    pstthrdnode->tpthreadid = pthread_self();
    (void)hali_threadmgr_add2list(pstthrdnode);

    printf("%s is running\n",pstthrdnode->szthrdname);

    hali_sem_post(HALI_SEM_THRDSYN);


    /*epoll调度*/
    hali_epoll_schedule(pstthrdnode->stthrdep.iepollfd);

    hali_svrthread_fini(pthreadnode);
    return (void *)ulerrcode;

}
/*svr线程消息处理*/
ulong hali_svrthread_msgproc(char *pucbuf, int ilen)
    {
        ulong ulerrcode = ERROR_FAILED;
        /*消息处理*/
        HALI_THRDMSG_DATA_S *pstthrdmsgdata  = NULL;

        pstthrdmsgdata = (HALI_THRDMSG_DATA_S *)pucbuf;

        if (NULL == pstthrdmsgdata)
        {
            return ulerrcode;
        }
        switch (pstthrdmsgdata->uimsgType)
        {
            case HALI_THRDMSGTYPE_PRINT:
            {
                printf("type = %d rcv mqueue msg %s\n",pstthrdmsgdata->uimsgType,
                                                       (char *)pstthrdmsgdata->pbuf);
                ulerrcode = ERROR_SUCCESS;
                break;
            }
            default:
            {
                ulerrcode = ERROR_SUCCESS;
                break;
            }
        }
        free(pstthrdmsgdata->pbuf);
        return ulerrcode;
    }



#ifdef __cplusplus
}
#endif



