#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


#include "../../../include/base/hali_base.h"
#include "../../../include/base/list.h"
#include "../../../include/base/hali_error.h"
#include "../../../include/base/hali_thread.h"
#include "../../../include/base/hali_epoll.h"
#include "../../../include/base/hali_timer.h"
#include "../../../include/base/hali_tlv.h"

#include "../include/zigbee.h"
#include "zigbee_serial.h"



SL_HEAD_S g_stdevlisthead;



char *g_pcdevmodestr[DEV_MODE_MAX]=
{
	[DEV_MODE_COOR]    =   "coor",
    [DEV_MODE_SENSOR]  = "sensor",
    [DEV_MODE_SWITCH]  = "switch",
    [DEV_MODE_OTHER]   =  "other",
};



/**
*  功能描述: 设备节点初始化
*  @param param1
*
*  @return
*/
API void zigbee_devnode_init()
{
	sl_init(&g_stdevlisthead);
}

API void zigbee_devnode_add(int devid,
						    int devtype,
						    int range)
{



	ZIGBEE_DEV_NODE_S *pstdevnode = NULL;

    pstdevnode = zigbee_devnode_find(devid);

    if(NULL != pstdevnode)
    {
        pstdevnode->devtype = devtype;
        pstdevnode->range = range;
    }
    else
    {
        pstdevnode = (ZIGBEE_DEV_NODE_S *)malloc(sizeof(ZIGBEE_DEV_NODE_S));

        if(NULL != pstdevnode)
        {
            pstdevnode->devid = devid;
            pstdevnode->devtype = devtype;
            pstdevnode->range = range;
            pstdevnode->ioldheartnum = 0;
            pstdevnode->iheartnum = 1;

            sl_addhead(&g_stdevlisthead, &pstdevnode->stnode);
            zigbee_devnode_attrvlaueinit(pstdevnode);
            /*订阅事件*/
            if(DEV_MODE_SWITCH == devtype)
            {
                zigbee_mqttswitchsub(devid);
            }
        }

    }

}
API ZIGBEE_DEV_NODE_S *zigbee_devnode_find(int devid)
{

	ZIGBEE_DEV_NODE_S *pstdevnode = NULL;
	SL_NODE_S *pstnode = NULL;
	SL_NODE_S *pstnext = NULL;
	SL_FOREACH_SAFE(&g_stdevlisthead,pstnode,pstnext){

		pstdevnode = SL_ENTRY(pstnode,ZIGBEE_DEV_NODE_S,stnode);

		if(devid == pstdevnode->devid)
		{
			return pstdevnode;
		}
	}
	return NULL;
}
API void zigbee_devnode_del(int devid)
{

	ZIGBEE_DEV_NODE_S *pstdevnode = NULL;
	SL_NODE_S *pstnode = NULL;
	SL_NODE_S *pstnext = NULL;
	SL_FOREACH_SAFE(&g_stdevlisthead,pstnode,pstnext){

		pstdevnode = SL_ENTRY(pstnode,ZIGBEE_DEV_NODE_S,stnode);

		if(devid == pstdevnode->devid)
		{
			sl_del(&g_stdevlisthead,pstnode);
			sl_freeall(&pstdevnode->stattributelist, free);
			free(pstdevnode);
			break;
		}
	}
}
API void zigbee_devnode_delall(void)
{

	ZIGBEE_DEV_NODE_S *pstdevnode = NULL;
	SL_NODE_S *pstnode = NULL;
	SL_NODE_S *pstnext = NULL;
	SL_FOREACH_SAFE(&g_stdevlisthead,pstnode,pstnext){
		pstdevnode = SL_ENTRY(pstnode,ZIGBEE_DEV_NODE_S,stnode);
	    sl_del(&g_stdevlisthead,pstnode);
        sl_freeall(&pstdevnode->stattributelist, free);
		free(pstdevnode);
	}
}

API void zigbee_devnode_printall(char *pstr)
{

	ZIGBEE_DEV_NODE_S *pstdevnode = NULL;
	SL_NODE_S *pstnode = NULL;
	SL_NODE_S *pstnext = NULL;

    printf("/****************/\n");
	SL_FOREACH_SAFE(&g_stdevlisthead,pstnode,pstnext)
    {

		pstdevnode = SL_ENTRY(pstnode,ZIGBEE_DEV_NODE_S, stnode);
        printf("/****************/\n");
        printf("dev    id: 0X%04X\n",(ushort)pstdevnode->devid);
        printf("dev  type: %s\n",g_pcdevmodestr[pstdevnode->devtype]);
        printf("dev range: %d\n",pstdevnode->range);
		hali_tlv_printdebug(&pstdevnode->stattributelist);
	}
}

API ulong zigbee_devnode_getattrvlaue(int devid, uint uitag, void *pvalue)
{

    ulong ulerr = ERROR_FAILED;

	ZIGBEE_DEV_NODE_S *pstdevnode = NULL;

    pstdevnode = zigbee_devnode_find(devid);

    if(NULL != pstdevnode)
    {
        //printf("get success devnode\n");
        zigbee_debug(ZIGBEE_DEBUG_EVENT, "get success devnode\n");
        ulerr = hali_tlv_getvalue(&pstdevnode->stattributelist, uitag, pvalue);

    }
    return ulerr;
}
API ulong zigbee_devnode_setattrvlaue(int devid, uint uitag, void *pvalue , uint size)
{

    ulong ulerr = ERROR_FAILED;

	ZIGBEE_DEV_NODE_S *pstdevnode = NULL;
    HALI_TLV_NODE_S *pstlvnode = NULL;
    void *ptmp;

    ptmp = (void *)malloc(size);
    if (NULL == ptmp)
    {
        return ulerr;
    }
    pstdevnode = zigbee_devnode_find(devid);

    if(NULL != pstdevnode)
    {
        ulerr = hali_tlv_getvalue(&pstdevnode->stattributelist, uitag, ptmp);
        if (ERROR_SUCCESS == ulerr)
        {
            ulerr = hali_tlv_changevalue(&pstdevnode->stattributelist, uitag, pvalue);
        }
        else
        {
            pstlvnode = hali_tlv_creatnode(uitag, pvalue, size);
            if(NULL != pstlvnode)
            {
                sl_addafter(&pstdevnode->stattributelist, pstlvnode);
                ulerr = ERROR_SUCCESS;
            }

        }

    }
    free(ptmp);
    return ulerr;
}

API void zigbee_devnode_attrvlaueinit(ZIGBEE_DEV_NODE_S *pstdevnode)
{

    HALI_TLV_NODE_S *pstlvnode = NULL;
    char data = SWITCH_STATE_OFF;
    sl_init(&pstdevnode->stattributelist);

    if (DEV_MODE_SWITCH == pstdevnode->devtype)
    {
        pstlvnode = hali_tlv_creatnode(TLV_RESP_SWITCH_STATE, &data, sizeof(data));
        if(NULL != pstlvnode)
        {
            //printf("init success devnode\n");
            zigbee_debug(ZIGBEE_DEBUG_EVENT, "init success devnode\n");
            sl_addafter(&pstdevnode->stattributelist, pstlvnode);
        }

    }

    return ;
}


/**
 *  功能描述:
 *  @param param1
 *
 *  @return
 */
API ulong hali_dev_syntimeout(int itimerfd)
{
    uint64_t ui64data;
    char szmbuf[20];
    static uchar state = 0;
    ZIGBEE_DEV_NODE_S *pstdevnode = NULL;
	SL_NODE_S *pstnode = NULL;
	SL_NODE_S *pstnext = NULL;

    read(itimerfd, &ui64data, sizeof(uint64_t));

    /*发送广播查找设备*/
    zigbee_serialsenddiscover();
    /*根据已有设备同步信息*/
	SL_FOREACH_SAFE(&g_stdevlisthead,pstnode,pstnext){

		pstdevnode = SL_ENTRY(pstnode,ZIGBEE_DEV_NODE_S,stnode);

		if(DEV_MODE_SWITCH == pstdevnode->devtype)
		{
            zigbee_serialsendsinglemsg(pstdevnode->devid,
                                      TLV_REQ_SWITCH_STATE,
                                      NULL,
                                      NULL);
		}
	}

/*
    zigbee_serialsendswitchcmd(0x796F,state);
    if (0 == state)
    {state=1;}
    else
    {state=0;}
*/
    return ERROR_SUCCESS;
}


#ifdef __cplusplus
}
#endif

