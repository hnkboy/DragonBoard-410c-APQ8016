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
#include "../../include/base/hali_epoll.h"
#include "../../include/base/hali_timer.h"
#include "../../include/base/hali_tlv.h"





API HALI_TLV_NODE_S *hali_tlv_creatnode(uint uitag,uchar *pvalue, uint uisize)
{

    HALI_TLV_NODE_S *pstlvnode = NULL;
    pstlvnode = (HALI_TLV_NODE_S *)malloc(sizeof(HALI_TLV_NODE_S) + uisize);
    if (NULL != pstlvnode)
    {
        memset(pstlvnode, 0, sizeof(HALI_TLV_NODE_S) + uisize);
        pstlvnode->uitag = uitag;
        pstlvnode->uilen = uisize;
        if (NULL != pvalue)
        {
            memcpy(pstlvnode->aucdata, pvalue, uisize);
        }
    }
    return pstlvnode;
}

void hali_tlv_destroynode(HALI_TLV_NODE_S *pstlvnode)
{
    if (NULL != pstlvnode)
    {
        free(pstlvnode);
    }
    return ;
}
API ulong hali_tlv_getvalue(SL_HEAD_S *phead, uint uitag, void *pvalue)
{
    ulong ulerr = ERROR_FAILED;
    HALI_TLV_NODE_S *pstlvnode = NULL;

	SL_NODE_S *pstnode = NULL;
	SL_NODE_S *pstnext = NULL;
	SL_FOREACH_SAFE(phead,pstnode,pstnext){
		pstlvnode = SL_ENTRY(pstnode, HALI_TLV_NODE_S,stnode);
        if (uitag == pstlvnode->uitag)
        {
            memcpy(pvalue, pstlvnode->aucdata, pstlvnode->uilen);
            ulerr = ERROR_SUCCESS;
            break;
        }
	}
    return ulerr;
}
API ulong hali_tlv_changevalue(SL_HEAD_S *phead, uint uitag, void *pvalue)
{
    ulong ulerr = ERROR_FAILED;
    HALI_TLV_NODE_S *pstlvnode = NULL;

	SL_NODE_S *pstnode = NULL;
	SL_NODE_S *pstnext = NULL;
	SL_FOREACH_SAFE(phead,pstnode,pstnext){
		pstlvnode = SL_ENTRY(pstnode, HALI_TLV_NODE_S,stnode);
        if (uitag == pstlvnode->uitag )
        {
            memcpy(pstlvnode->aucdata, pvalue, pstlvnode->uilen);
            ulerr = ERROR_SUCCESS;
            break;
        }
	}

    return ulerr;
}


/**
 *  功能描述:
 *  @param param1
 *
 *  @return
 */

API void hali_tlv_freenode(void *pstnode)
{
    HALI_TLV_NODE_S *pstlvnode;
    if (NULL != pstnode)
    {
        pstlvnode = SL_ENTRY(pstnode, HALI_TLV_NODE_S, stnode);
        free(pstlvnode);
    }
    return ;
}

API void hali_tlv_printdebug(SL_HEAD_S *psthead)
{
    HALI_TLV_NODE_S *pstlvnode = NULL;
    SL_NODE_S *pstnode = NULL;
    SL_NODE_S *pstnext = NULL;
    uint tmpnum;
    SL_FOREACH_SAFE(psthead,pstnode,pstnext){

        pstlvnode = SL_ENTRY(pstnode,HALI_TLV_NODE_S,stnode);
        printf("/****************/\n");
        printf("tlv   tag: %d\n",pstlvnode->uitag);
        printf("tlv   len: %d\n",pstlvnode->uilen);
        printf("tlv vlaue: ");
        for (tmpnum = 0 ; tmpnum < pstlvnode->uilen; tmpnum ++)
        {
            printf("%2x ",pstlvnode->aucdata[tmpnum]);
        }
        printf("\n");
    }
    return ;
}





#ifdef __cplusplus
}
#endif




