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




/**
 *  功能描述:
 *  @param param1
 *
 *  @return
 */
ulong hali_threadmsg_sendmsg(uint uithrType,
                                      uint uiindex,
                                      uint uimsgtype,
                                      void *pszsendbuf,
                                      uint uilenth)
{
    HALI_THREAD_NODE_S *pstthrdnode = NULL;
    uint uithreadseqnum;
    ulong ulerrcode = ERROR_FAILED;
    int iret = -1;
    HALI_THRDMSG_DATA_S sthrdmsgdata;
    char *pbuf = NULL;

    if (NULL == pszsendbuf)
    {
        return ulerrcode;
    }
    if (HALI_THREADTYPE_MAX <= uithrType)
    {
        return ulerrcode;
    }
    memset(&sthrdmsgdata,0,sizeof(sthrdmsgdata));

    /*换算线程索引         */
    uithreadseqnum = hali_thread_idcacl(uithrType,uiindex);
    /*查找线程列表是否已经创建*/
    pstthrdnode = hali_threadmgr_getnode(uithreadseqnum);
    if ((NULL != pstthrdnode) && (-1 != pstthrdnode->imqfd))
    {

        pbuf = malloc(uilenth);
        if (NULL != pbuf)
        {
            memcpy(pbuf, pszsendbuf, uilenth);

            sthrdmsgdata.uimsgType = uimsgtype;
            sthrdmsgdata.uilenth = uilenth;
            sthrdmsgdata.pbuf = (void *)pbuf;

            iret = mq_send(pstthrdnode->imqfd, &sthrdmsgdata, sizeof(HALI_THRDMSG_DATA_S), 2);
            if (-1 == iret)
            {
                free(pbuf);
            }
            else
            {
                ulerrcode = ERROR_SUCCESS;
            }
        }

    }
    return ulerrcode;
}

#ifdef __cplusplus
}
#endif



