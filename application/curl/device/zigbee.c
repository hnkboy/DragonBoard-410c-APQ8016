#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>

#include "../lib/list.h"
#include "zigbee.h"

#include "../cJSON/cJSON.h"

char *shelpstr[]={
	"ERROR",
	"DO_LIGHT",
	"GET_KEY",
	"GET_TEMPER",
	"GET_HUMI",
	"MO_LIGHT",
	"MO_KEY",
	"MO_TEMPER",
	"MO_HUMI",
	"SET_ADDR",
	"MO_DISTANCE",
	"REQ_DISTANCE",
	"RESP_DISTANCE",
	"default"
};


SL_HEAD_S g_DevNode;



/*
 * 设备tlv 节点内容
 */

 void zigbee_endtlvnode_del(SL_HEAD_S *pstHead,
								  uint32_t uiTag){
	END_NODE_S *pstEndNode = NULL;
	SL_NODE_S *pstNode = NULL;
	SL_NODE_S *pstNext = NULL;
	SL_FOREACH_SAFE(pstHead,pstNode,pstNext){

		pstEndNode = SL_ENTRY(pstNode,END_NODE_S,stNode);

		if(uiTag == pstEndNode->uiTag)
		{
			sl_del(pstHead,pstNode);
			free(pstEndNode);
			break;
		}
	}
}

END_NODE_S *zigbee_endtlvnode_find(SL_HEAD_S *pstHead,
										 uint32_t uiTag){

	END_NODE_S *pstEndNode = NULL;
	SL_NODE_S *pstNode = NULL;
	SL_NODE_S *pstNext = NULL;
	SL_FOREACH_SAFE(pstHead,pstNode,pstNext){

		pstEndNode = SL_ENTRY(pstNode,END_NODE_S,stNode);

		if(uiTag == pstEndNode->uiTag)
		{
		 return pstEndNode;
		}
	}
	return NULL;
}

void zigbee_endtlvnode_add(SL_HEAD_S *pstHead,
							     uint32_t uiTag,
    							 uint8_t *pValue,
    						     uint32_t uiLen){
	uint32_t uiTemp;
    uint8_t *pStr;
	END_NODE_S *pstEndNode = NULL;
    //printf("%d,%d,%s,%d",pstHead,uiTag,pValue,uiLen);
	pstEndNode = zigbee_endtlvnode_find(pstHead,uiTag);
	if (NULL != pstEndNode){
		zigbee_endtlvnode_del(pstHead,uiTag);
		pstEndNode = NULL;
	}
	pstEndNode = (END_NODE_S *)malloc(sizeof(END_NODE_S) + uiLen);
    memset(pstEndNode,0,sizeof(END_NODE_S)+ uiLen);
    pstEndNode->uiTag = uiTag;
    pstEndNode->uiLen = uiLen;
    pStr = (uint8_t *)pstEndNode + sizeof(END_NODE_S);
	for(uiTemp = 0; uiTemp < uiLen; uiTemp ++){

		pStr[uiTemp] = pValue[uiTemp];

	}
    pstEndNode->pValue  = pStr;
	pstEndNode->pValue[uiLen]  = '\0';

	sl_addhead(pstHead,&pstEndNode->stNode);
}

 void zigbee_endtlvnode_delall(SL_HEAD_S *pstHead){

	END_NODE_S *pstEndNode = NULL;
	SL_NODE_S *pstNode = NULL;
	SL_NODE_S *pstNext = NULL;
	SL_FOREACH_SAFE(pstHead,pstNode,pstNext){
		pstEndNode = SL_ENTRY(pstNode,END_NODE_S,stNode);
		sl_del(pstHead,pstNode);
		free(pstEndNode);
	}
 }
 void zigbee_endtlvnode_printall(SL_HEAD_S *pstHead){

	 END_NODE_S *pstEndNode = NULL;
	 SL_NODE_S *pstNode = NULL;
	 SL_NODE_S *pstNext = NULL;
	 SL_FOREACH_SAFE(pstHead,pstNode,pstNext){

		 pstEndNode = SL_ENTRY(pstNode,END_NODE_S,stNode);
		 printf("/****************/\n");
		 printf("tlv   tag: %d,%s\n",pstEndNode->uiTag,shelpstr[pstEndNode->uiTag]);
		 printf("tlv   len: %d\n",pstEndNode->uiLen);
		 printf("tlv vlaue: %s\n",pstEndNode->pValue);
	 }
 }










/*设备节点*/


void zigbee_devnode_init(){
	sl_init(&g_DevNode);
}
void zigbee_devnode_add(int devid,
						int devtype,
						int range){

	DEV_NODE_S *pstDevNode;
	pstDevNode = (DEV_NODE_S *)malloc(sizeof(DEV_NODE_S));

    pstDevNode->devid = devid;
    pstDevNode->devtype = devtype;
    pstDevNode->range = range;
	pstDevNode->ioldheartnum = 0;
	pstDevNode->iheartnum = 1;
	sl_init(&pstDevNode->stTlvHead);
	sl_addhead(&g_DevNode,&pstDevNode->stNode);

}
DEV_NODE_S *zigbee_devnode_find(int devid){

	DEV_NODE_S *pstDevNode = NULL;
	SL_NODE_S *pstNode = NULL;
	SL_NODE_S *pstNext = NULL;
	SL_FOREACH_SAFE(&g_DevNode,pstNode,pstNext){

		pstDevNode = SL_ENTRY(pstNode,DEV_NODE_S,stNode);

		if(devid == pstDevNode->devid)
		{
			return pstDevNode;
		}
	}
	return NULL;
}
void zigbee_devnode_del(int devid){

	DEV_NODE_S *pstDevNode = NULL;
	SL_NODE_S *pstNode = NULL;
	SL_NODE_S *pstNext = NULL;
	SL_FOREACH_SAFE(&g_DevNode,pstNode,pstNext){

		pstDevNode = SL_ENTRY(pstNode,DEV_NODE_S,stNode);

		if(devid == pstDevNode->devid)
		{
			sl_del(&g_DevNode,pstNode);
			zigbee_endtlvnode_delall(&pstDevNode->stTlvHead);
			free(pstDevNode);
			break;
		}
	}
}
void zigbee_devnode_delall(void){

	DEV_NODE_S *pstDevNode = NULL;
	SL_NODE_S *pstNode = NULL;
	SL_NODE_S *pstNext = NULL;
	SL_FOREACH_SAFE(&g_DevNode,pstNode,pstNext){
		pstDevNode = SL_ENTRY(pstNode,DEV_NODE_S,stNode);
	    sl_del(&g_DevNode,pstNode);
		zigbee_endtlvnode_delall(&pstDevNode->stTlvHead);
		free(pstDevNode);
	}
}
void zigbee_devnode_printall(void){

	DEV_NODE_S *pstDevNode = NULL;
	SL_NODE_S *pstNode = NULL;
	SL_NODE_S *pstNext = NULL;
	SL_FOREACH_SAFE(&g_DevNode,pstNode,pstNext){

		pstDevNode = SL_ENTRY(pstNode,DEV_NODE_S,stNode);
        printf("/****************/\n");
        printf("dev    id: %d\n",pstDevNode->devid);
        printf("dev  type: %d\n",pstDevNode->devtype);
        printf("dev range: %d\n",pstDevNode->range);
		zigbee_endtlvnode_printall(&pstDevNode->stTlvHead);
	}
}






#if 0
void main(){

	DEV_NODE_S *pstDevNode = NULL;
	SL_NODE_S *pstNode = NULL;
	SL_NODE_S *pstNext = NULL;

    zigbee_devnode_init();
    zigbee_devnode_add(1,1,2);
    zigbee_devnode_add(2,1,2);
    zigbee_devnode_add(3,1,2);

	SL_FOREACH_SAFE(&g_DevNode,pstNode,pstNext){

		pstDevNode = SL_ENTRY(pstNode,DEV_NODE_S,stNode);
        printf("/****************/\n");
        printf("dev    id: %d\n",pstDevNode->devid);
        printf("dev  type: %d\n",pstDevNode->devtype);
        printf("dev range: %d\n",pstDevNode->range);
	}

    zigbee_devnode_delall();

}
#endif

int g_ZigbeeFd = -1;

/*
 * @brief   | SOP | Data Length  |   CMD   |   Data   |  FCS  |
 *          |  1  |     1        |    2    |  0-Len   |   1   |
*/

void zigbee_send_discover(void){

	char buf[8];
	char i;

    printf("zigebee teset\n ");
	buf[0] = 0xFE;

	buf[1] = 0x03;  /*数据长度*/

	buf[2] = 0x00;  /* CMD */
	buf[3] = 0x00;

	buf[4] = REQ_DISCOVE;
	buf[5] = 0x01;
	buf[6] = 0x00;

	buf[7] = 0x00;
    for(i=0;i<8;i++){
    printf("%2x ",buf[i]);
    }
	write(g_ZigbeeFd,buf,8);
}
void zigbee_send_cmd(uint8_t cmd,
                    uint8_t devid,
                    uint8_t tlvtype,
                    uint8_t tlvalue,
                    uint8_t tlvlen){

	char buf[8];
	char i;

    printf("zigebee teset\n ");
	buf[0] = 0xFE;

	buf[1] = 0x03;  /*数据长度*/

	buf[2] = cmd;  /* CMD */
	buf[3] = devid;

	buf[4] = tlvtype;
	buf[5] = tlvlen;
	buf[6] = tlvalue;

	buf[7] = 0x00;
    for(i=0;i<8;i++){
    printf("%2x ",buf[i]);
    }
	write(g_ZigbeeFd,buf,8);
}
void zigbee_send_broadcast(void){

    zigbee_send_cmd(0,0xff,0,0,0);
}
 void zigbee_send_tlvbysl(uint8_t cmd,uint8_t devid,SL_HEAD_S *pstHead){
     uint8_t sndstrbuf[ZIGBEE_MAX_DATALEN];
	 END_NODE_S *pstEndNode = NULL;
	 SL_NODE_S *pstNode = NULL;
	 SL_NODE_S *pstNext = NULL;
	 uint8_t ucNum = 0;
     sndstrbuf[0] = 0xFE;
     sndstrbuf[2] = cmd;
     sndstrbuf[3] = devid;
     ucNum += 4;
	 SL_FOREACH_SAFE(pstHead,pstNode,pstNext){

		 pstEndNode = SL_ENTRY(pstNode,END_NODE_S,stNode);
         sndstrbuf[ucNum ++] = pstEndNode->uiTag;
         sndstrbuf[ucNum ++] = pstEndNode->uiLen;
         if(0 != pstEndNode->uiLen){
            memcpy(&sndstrbuf[ucNum],pstEndNode->pValue,pstEndNode->uiLen);
            ucNum += pstEndNode->uiLen;
         }
	 }
     sndstrbuf[1] = ucNum - 4;
     sndstrbuf[ucNum ++] = 0x0;
    for(int i=0;i<ucNum;i++){
    printf("%2x ",sndstrbuf[i]);
    }
     write(g_ZigbeeFd,sndstrbuf,ucNum);

 }



/* Create a bunch of objects as demonstration. */
static int print_preallocated(cJSON *root,int8_t *poutbuf,uint16_t *plen)
{
    /* declarations */
    char *out = NULL;
    char *buf = NULL;
    char *buf_fail = NULL;
    size_t len = 0;
    size_t len_fail = 0;
    /* formatted print */
    out = cJSON_Print(root);

    /* create buffer to succeed */
    /* the extra 5 bytes are because of inaccuracies when reserving memory */
    len = strlen(out) + 5;
    buf = (char*)malloc(len);
    if (buf == NULL)
    {
        printf("Failed to allocate memory.\n");
        exit(1);
    }

    /* create buffer to fail */
    len_fail = strlen(out);
    buf_fail = (char*)malloc(len_fail);
    if (buf_fail == NULL)
    {
        printf("Failed to allocate memory.\n");
        exit(1);
    }

    /* Print to buffer */
    if (!cJSON_PrintPreallocated(root, buf, (int)len, 1)) {
        printf("cJSON_PrintPreallocated failed!\n");
        if (strcmp(out, buf) != 0) {
            printf("cJSON_PrintPreallocated not the same as cJSON_Print!\n");
            printf("cJSON_Print result:\n%s\n", out);
            printf("cJSON_PrintPreallocated result:\n%s\n", buf);
        }
        free(out);
        free(buf_fail);
        free(buf);
        return -1;
    }

    /* success */
    printf("%s\n", buf);
    if (NULL != poutbuf)
    {
        if (len < BUFSIZ){
            *plen = snprintf(poutbuf,len - 1,"%s\n",buf);
        }
    }

    /* force it to fail */
    if (cJSON_PrintPreallocated(root, buf_fail, (int)len_fail, 1)) {
        printf("cJSON_PrintPreallocated failed to show error with insufficient memory!\n");
        printf("cJSON_Print result:\n%s\n", out);
        printf("cJSON_PrintPreallocated result:\n%s\n", buf_fail);
        free(out);
        free(buf_fail);
        free(buf);
        return -1;
    }

    free(out);
    free(buf_fail);
    free(buf);
    return 0;
}



void zigbee_devnode_print2json(char *poutbuf,uint16_t *plen){
//void main(void){
	char pbuf[40];
	cJSON *root = NULL;
	cJSON *dev = NULL;
	cJSON *devnode = NULL;
	cJSON *devdata = NULL;
    int i = 0;

	DEV_NODE_S *pstDevNode = NULL;
	SL_NODE_S *pstSlNode = NULL;
	SL_NODE_S *pstSlNext = NULL;

	END_NODE_S *pstEndNode = NULL;
	SL_HEAD_S *pstHead ;
	SL_NODE_S *pstNode = NULL;
	SL_NODE_S *pstNext = NULL;

	root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("JIALI (\"Bee\") Nimble"));
    cJSON_AddItemToObject(root, "dev", dev = cJSON_CreateArray());
#if 0
    for (i = 0; i < 4; i++)
    {
        cJSON_AddItemToArray(dev, devnode = cJSON_CreateObject());
        cJSON_AddStringToObject(devnode, "devid", "12");
        cJSON_AddStringToObject(devnode, "devtype", "1");
        cJSON_AddStringToObject(devnode, "devrange", "100");
        cJSON_AddItemToObject(devnode, "devdata", devdata = cJSON_CreateObject());
        cJSON_AddStringToObject(devdata, "temperature", "12");
        cJSON_AddStringToObject(devdata, "humidity", "12");
    }
#endif
	SL_FOREACH_SAFE(&g_DevNode,pstSlNode,pstSlNext){

		pstDevNode = SL_ENTRY(pstSlNode,DEV_NODE_S,stNode);

        cJSON_AddItemToArray(dev, devnode = cJSON_CreateObject());
        cJSON_AddNumberToObject(devnode, "devid", pstDevNode->devid);
        cJSON_AddNumberToObject(devnode, "devtype", pstDevNode->devtype);
        cJSON_AddNumberToObject(devnode, "devrange", pstDevNode->range);
        if ( NULL != pstDevNode->stTlvHead.pFirst)
        {
		    cJSON_AddItemToObject(devnode, "devdata", devdata = cJSON_CreateObject());
            SL_FOREACH_SAFE(&pstDevNode->stTlvHead,pstNode,pstNext){

                pstEndNode = SL_ENTRY(pstNode,END_NODE_S,stNode);
                cJSON_AddStringToObject(devdata, shelpstr[pstEndNode->uiTag], pstEndNode->pValue);
            }
        }
	}
	print_preallocated(root,poutbuf,plen);
    cJSON_Delete(root);
}

void zigbee_devproc_syncdata(void){

    static uint16_t usSecNum = 0;
    SL_HEAD_S stHead;

    /*发现设备*/
    if (0 == usSecNum)
    {
        zigbee_send_discover();
    }
    else if (1 == usSecNum)
    {
        usSecNum = 0;
        sl_init(&stHead);
        zigbee_endtlvnode_add(&stHead,GET_HUMI,0,0);
        zigbee_endtlvnode_add(&stHead,GET_TEMPER,0,0);
        zigbee_send_tlvbysl(CMD_COOR,0xff,&stHead);
        //zigbee_send_tlvbysl(CMD_END,0x03,&stHead);
        //zigbee_send_tlvbysl(CMD_END,0x02,&stHead);
        zigbee_endtlvnode_delall(&stHead);
    }
    usSecNum ++;

}
void zigbee_devproc_checkheart(void){

	DEV_NODE_S *pstDevNode = NULL;
	SL_NODE_S *pstNode = NULL;
	SL_NODE_S *pstNext = NULL;
	SL_FOREACH_SAFE(&g_DevNode,pstNode,pstNext){
		pstDevNode = SL_ENTRY(pstNode,DEV_NODE_S,stNode);

		if(pstDevNode->ioldheartnum == pstDevNode->ioldheartnum){
			sl_del(&g_DevNode,pstNode);
			zigbee_endtlvnode_delall(&pstDevNode->stTlvHead);
			free(pstDevNode);
		}
		else{
			pstDevNode->ioldheartnum = pstDevNode->ioldheartnum;
		}
	}
}

