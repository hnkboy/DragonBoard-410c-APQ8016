#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>

#include "../lib/list.h"
#include "zigbee.h"

char *shelpstr[]={
	"DO_LIGHT",
	"GET_KEY",
	"GET_TEMPER",
	"MO_LIGHT",
	"MO_KEY",
	"MO_TEMPER",
	"MO_HUMI",
	"SET_ADDR",
	"MO_DISTANCE",
	"default"
};


SL_HEAD_S g_DevNode;


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
