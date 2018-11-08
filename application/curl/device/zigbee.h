
#ifndef _ZIGBEE
#define _ZIGBEE

typedef struct stgDevNode
{   
	LS_NODE_S stNode;
	int devid;
	int devtype;
    int range;
}DEV_NODE_S;

extern char *shelpstr[];
#endif
