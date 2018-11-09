#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

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


LS_HEAD_S g_DevNode;


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
LS_NODE_S zigbee_devnode_find(int devid){

	DEV_NODE_S *pstDevNode = NULL;
	LS_NODE_S *pstNode = NULL;
	LS_NODE_S *pstNext = NULL;
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
void zigbee_devnode_del(int devid){

	DEV_NODE_S *pstDevNode = NULL;
	LS_NODE_S *pstNode = NULL;
	LS_NODE_S *pstNext = NULL;
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
	LS_NODE_S *pstNode = NULL;
	LS_NODE_S *pstNext = NULL;
	SL_FOREACH_SAFE(&g_DevNode,pstNode,pstNext){
		pstDevNode = SL_ENTRY(pstNode,DEV_NODE_S,stNode);
	    sl_del(&g_DevNode,pstNode);
		free(pstDevNode);
	}
}
#if 0
void main(){

	DEV_NODE_S *pstDevNode = NULL;
	LS_NODE_S *pstNode = NULL;
	LS_NODE_S *pstNext = NULL;

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
