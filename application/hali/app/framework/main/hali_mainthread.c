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
#include "../../include/base/hali_threadmsg.h"
#include "../../include/base/hali_epoll.h"
#include "../../include/base/hali_timer.h"
#include "../../include/base/hali_modules.h"
#include "../../include/base/hali_cli.h"

int g_imainepollfd = -1;
int g_imainmqfd = -1;

int g_timerfd = -1;




/**
 *  功能描述: main线程收到的线程消息处理
 *  @param param1
 *
 *  @return
 */

ulong hali_mainthread_msgproc(char *pucbuf, int ilen)
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

/*main线程消息处理*/
ulong hali_mainthread_mainmsgproc(char *pucbuf, int ilen)
{
    ulong ulerrcode = ERROR_FAILED;

    uint uimodtype;

    for(uimodtype = HALI_MODULE_BASIC; uimodtype < HALI_MODULE_MAX; uimodtype++)
    {
        if (NULL != g_mainmsgproc[uimodtype])
        {
            ulerrcode |= g_mainmsgproc[uimodtype](pucbuf, ilen);
        }
    }
    return ulerrcode;
}



/**
 *  功能描述:
 *  @param param1
 *
 *  @return
 */
ulong hali_mainthread_timeout(int itimerfd)
{
    uint64_t ui64data;
    char szmbuf[20];
    read(itimerfd, &ui64data, sizeof(uint64_t));
    //printf("timerfd read data is %lu.\n",data);
    szmbuf[0] = 'w';
    szmbuf[1] = 'w';
    szmbuf[2] = 'w';
    szmbuf[3] = '\0';


    (void)hali_threadmsg_sendmsg(HALI_THREADTYPE_WORK,
                                 1,
                                 HALI_THRDMSGTYPE_PRINT,
                                 szmbuf,
                                 4);

    //printf("1s \n\r");
    return ERROR_SUCCESS;
}

void hali_mainthread_funinit(void)
{
    ulong ulerrcode;
    hali_timer_create(1, hali_mainthread_timeout, &g_timerfd);

    ulerrcode = hali_epoll_add(STDIN_FILENO, hali_cli_msgcallbackproc);
    if (ERROR_SUCCESS != ulerrcode)
    {
        printf("main thrad fun init failed\n");
    }
    return;
}
/*按照类型和数量创建线程*/
ulong hali_mainthread_init(void)
{
    HALI_THREAD_NODE_S *pstthrdnode = NULL;
    int iepollfd = -1;
    int imqfd = -1;
    uint uithreadseqnum;
    uchar szname[THRDNAMELEN_MAX +1];
    uchar szmqname[THRDNAMELEN_MAX +1];
    ulong ulerrcode = ERROR_FAILED;
    uint uiindex = 0;


    szname[0]='\0';
    szmqname[0]='\0';

    /*换算线程索引         */
    uithreadseqnum = hali_thread_idcacl(HALI_THREADTYPE_MAIN, uiindex);
    /*查找线程列表是否已经创建*/
    pstthrdnode = hali_threadmgr_getnode(uithreadseqnum);
    if(NULL == pstthrdnode)
    {
        pstthrdnode = hali_threadmgr_creat(uithreadseqnum);
    }

    if(NULL != pstthrdnode)
    {
        /*算出线程名字*/
        sprintf((char *)szname,"%s-%u",g_paucthrdname[HALI_THREADTYPE_MAIN],uiindex);
        sprintf((char *)szmqname,"/%s-%u",g_paucthrdname[HALI_THREADTYPE_MAIN],uiindex);
        strcpy((char *)pstthrdnode->szthrdname,(char *)szname);
        pstthrdnode->uithreadseqnum = uithreadseqnum;

        /*创建epoll挂回调     */
        iepollfd =  epoll_create(1);
        if (-1 != iepollfd)
        {
            pstthrdnode->stthrdep.iepollfd = iepollfd;
            sl_init(&pstthrdnode->stthrdep.stfdlist);

            g_imainepollfd = iepollfd;
            imqfd = mq_open((char *)szmqname, O_RDWR|O_CREAT, 0600, NULL);
            if (-1 != imqfd)
            {
                pstthrdnode->imqfd = imqfd;
                g_imainmqfd = imqfd;
                pstthrdnode->tpthreadid = pthread_self();
                /*mq消息处理*/
                pstthrdnode->pfmsgcallbackfun = hali_mainthread_mainmsgproc;
                (void)hali_threadmgr_add2list(pstthrdnode);

                ulerrcode = hali_epoll_add(imqfd, hali_thread_mqmsgcallbackproc);
                if(ERROR_SUCCESS != ulerrcode)
                {
                    hali_threadmgr_del4list(pstthrdnode->uithreadseqnum);
                }
                else
                {

                    hali_mainthread_funinit();

                }
            }
        }
        if (ERROR_SUCCESS != ulerrcode)
        {
            if (-1 != iepollfd)
            {
                (void)close(iepollfd);
            }
            if (-1 != imqfd)
            {
                (void)mq_close(imqfd);
            }
            hali_threadmgr_destroy(pstthrdnode);
        }
    }
    return ulerrcode;
}


/**
 *  功能描述: 线程节点去初始化
 *  @param pthreadnode
 *
 *  @return
 */
void hali_mainthread_fini(void)
{

    HALI_THREAD_NODE_S *pstthrdnode = NULL;
    uint uithreadseqnum;
    uint uiindex = 0;


    /*换算线程索引         */
    uithreadseqnum = hali_thread_idcacl(HALI_THREADTYPE_MAIN, uiindex);
    /*查找线程列表是否已经创建*/
    pstthrdnode = hali_threadmgr_getnode(uithreadseqnum);
    if (NULL == pstthrdnode)
    {
        return;
    }

    hali_timer_destroy(g_timerfd);

    if (-1 != pstthrdnode->imqfd)
    {
        (void)hali_epoll_del(pstthrdnode->imqfd);
        (void)mq_close(pstthrdnode->imqfd);
    }
    if (-1 != pstthrdnode->stthrdep.iepollfd)
    {
        (void)close(pstthrdnode->stthrdep.iepollfd);
    }


    hali_threadmgr_del4list(pstthrdnode->uithreadseqnum);
    hali_threadmgr_destroy(pstthrdnode);
    return;
}


#ifdef __cplusplus
}
#endif



