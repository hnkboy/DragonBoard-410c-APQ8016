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


HALI_THREAD_MGR_S g_authrdmgr[HALI_THREADTYPE_MAX];


/*线程管理节点的初始化*/
void hali_threadmgr_init()
{
    int icont;
    /*遍历创建链表*/
    for (icont = 0; HALI_THREADTYPE_MAX > icont; icont++)
    {
        sl_init(&g_authrdmgr[icont].sthrdnodehead);
        g_authrdmgr[icont].uitypethrdnum = 0;
    }
}

/*线程管理节点的去初始化*/
void hali_threadmgr_fini()
{


}


HALI_THREAD_NODE_S *hali_threadmgr_creat(uint uithreadseqnum)
{
	HALI_THREAD_NODE_S *pstthreadnode = NULL;
    uint uithrdtype = hali_thread_typecacl(uithreadseqnum);
    if (HALI_THREADTYPE_MAX <= uithrdtype)
    {
        return pstthreadnode;
    }
    pstthreadnode = (HALI_THREAD_NODE_S *)malloc(sizeof(HALI_THREAD_NODE_S));
    if (NULL != pstthreadnode)
    {
        memset(pstthreadnode,0,sizeof(HALI_THREAD_NODE_S));
    }
    return pstthreadnode;
}
void hali_threadmgr_destroy(HALI_THREAD_NODE_S *psthreadinfo)
{
    if (NULL != psthreadinfo)
    {
        free(psthreadinfo);
    }
}

ulong  hali_threadmgr_add2list(HALI_THREAD_NODE_S *psthreadinfo)
{

    ulong ulerrcode = ERROR_FAILED;
    uint uithrdtype = HALI_THREADTYPE_MAX;

    HALI_THREAD_NODE_S *pstthreadnode = NULL;

    if (NULL == psthreadinfo)
    {
        return ulerrcode;
    }
    uithrdtype = hali_thread_typecacl(psthreadinfo->uithreadseqnum);
    if (HALI_THREADTYPE_MAX <= uithrdtype)
    {
        return ulerrcode;
    }
    //printf("addfirst  %d  %d  ",g_authrdmgr[uithrdtype].sthrdnodehead.pfirst, uithrdtype);

   pstthreadnode = SL_ENTRY(g_authrdmgr[uithrdtype].sthrdnodehead.pfirst,HALI_THREAD_NODE_S, stnode);
   if (NULL != pstthreadnode)
   {
       //printf("213get %s  ,%d",pstthreadnode->szthrdname,pstthreadnode->imqfd);

   }

    if (NULL != psthreadinfo)
    {
        sl_addhead(&g_authrdmgr[uithrdtype].sthrdnodehead,&psthreadinfo->stnode);
        g_authrdmgr[uithrdtype].uitypethrdnum ++;
        ulerrcode = ERROR_SUCCESS;
    }
    // printf("addfirst  %d  %d  ",g_authrdmgr[uithrdtype].sthrdnodehead.pfirst, uithrdtype);

    pstthreadnode = SL_ENTRY(g_authrdmgr[uithrdtype].sthrdnodehead.pfirst,HALI_THREAD_NODE_S, stnode);
    if (NULL != pstthreadnode)
    {
    //    printf("213get %s  ,%d",pstthreadnode->szthrdname,pstthreadnode->imqfd);

    }

    return ulerrcode;
}

void hali_threadmgr_del4list(uint uithreadseqnum)
{
    HALI_THREAD_NODE_S *pstthreadnode = NULL;
	SL_NODE_S *pstnode = NULL;
	SL_NODE_S *pstnext = NULL;
    uint uithrdtype = HALI_THREADTYPE_MAX;
    uithrdtype = hali_thread_typecacl(uithreadseqnum);
    if (HALI_THREADTYPE_MAX <= uithrdtype)
    {
        return;
    }

    SL_FOREACH_SAFE(&g_authrdmgr[uithrdtype].sthrdnodehead,pstnode,pstnext)
	{
        pstthreadnode = SL_ENTRY(pstnode,HALI_THREAD_NODE_S,stnode);
		if(uithreadseqnum == pstthreadnode->uithreadseqnum)
		{
			sl_del(&g_authrdmgr[uithrdtype].sthrdnodehead,pstnode);
            g_authrdmgr[uithrdtype].uitypethrdnum --;
			break;
		}
	}
    return;
}



HALI_THREAD_NODE_S *hali_threadmgr_getnode(uint uithreadseqnum)
{

    HALI_THREAD_NODE_S *pstthreadnode = NULL;
	SL_NODE_S *pstnode = NULL;
	SL_NODE_S *pstnext = NULL;
    uint uithrdtype = hali_thread_typecacl(uithreadseqnum);
    if (HALI_THREADTYPE_MAX <= uithrdtype)
    {
        return pstthreadnode;
    }
    //printf("first  %d  %d  ",g_authrdmgr[uithrdtype].sthrdnodehead.pfirst, uithrdtype);

    pstthreadnode = SL_ENTRY(g_authrdmgr[uithrdtype].sthrdnodehead.pfirst,HALI_THREAD_NODE_S, stnode);
    if (NULL != pstthreadnode)
    {
        //printf("213get %s  ,%d",pstthreadnode->szthrdname,pstthreadnode->imqfd);

    }

    SL_FOREACH_SAFE(&g_authrdmgr[uithrdtype].sthrdnodehead,pstnode,pstnext)
	{

		pstthreadnode = SL_ENTRY(pstnode,HALI_THREAD_NODE_S, stnode);
        //printf("get %s  ,%d",pstthreadnode->szthrdname,pstthreadnode->imqfd);

		if(uithreadseqnum == pstthreadnode->uithreadseqnum)
		{

			return pstthreadnode;
		}
	}
    pstthreadnode = NULL;
    return pstthreadnode;
}



void hali_threadmgr_delall(uint uithrdtype)
{
    HALI_THREAD_NODE_S *pstthreadnode = NULL;
	SL_NODE_S *pstnode = NULL;
	SL_NODE_S *pstnext = NULL;

    if (HALI_THREADTYPE_MAX <= uithrdtype)
    {
        return;
    }
    SL_FOREACH_SAFE(&g_authrdmgr[uithrdtype].sthrdnodehead,pstnode,pstnext)
	{
        pstthreadnode = SL_ENTRY(pstnode,HALI_THREAD_NODE_S,stnode);
        sl_del(&g_authrdmgr[uithrdtype].sthrdnodehead,pstnode);
	    hali_threadmgr_destroy(pstnode);
	}
    return;
}


HALI_THREAD_NODE_S *hali_threadmgr_findbythrdid(pthread_t tpthreadid)
{

    HALI_THREAD_NODE_S *pstthreadnode = NULL;
	SL_NODE_S *pstnode = NULL;
	SL_NODE_S *pstnext = NULL;
    int icmpresult;
    uint uitemp;
    for (uitemp = 0; HALI_THREADTYPE_MAX > uitemp; uitemp++)
    {
        SL_FOREACH_SAFE(&g_authrdmgr[uitemp].sthrdnodehead,pstnode,pstnext)
        {


            pstthreadnode = SL_ENTRY(pstnode,HALI_THREAD_NODE_S, stnode);
            icmpresult = pthread_equal(pstthreadnode->tpthreadid,tpthreadid);
            //printf("threadid,%llu  %llu ",tpthreadid,pstthreadnode->tpthreadid);
            if(0 != icmpresult)
            {
                return pstthreadnode;
            }
        }
    }

    pstthreadnode = NULL;
    return pstthreadnode;
}

HALI_THREAD_NODE_S *hali_threadmgr_getcurthrdnode(void)
{

    HALI_THREAD_NODE_S *pstthreadnode = NULL;
    pthread_t tpthreadid = pthread_self();

    pstthreadnode = hali_threadmgr_findbythrdid(tpthreadid);

    return pstthreadnode;
}
int hali_threadmgr_getcurepfd(void)
{

    HALI_THREAD_NODE_S *pstthreadnode = NULL;
    pthread_t tpthreadid = pthread_self();
    int epfd = -1;

    pstthreadnode = hali_threadmgr_findbythrdid(tpthreadid);
    if (NULL != pstthreadnode)
    {
        epfd = pstthreadnode->stthrdep.iepollfd;
    }
    return epfd;
}

#ifdef __cplusplus
}
#endif


