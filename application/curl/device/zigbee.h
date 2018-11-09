
#ifndef _ZIGBEE
#define _ZIGBEE

extern int g_ZigbeeFd;


#define DO_LIGHT   0x01              //开灯操作
#define GET_KEY     0x02              //按键操作
#define GET_TEMPER  0x03              //温度
#define GET_HUMI    0x04              //湿度

#define MO_LIGHT   0x05              //开灯操作
#define MO_KEY     0x06              //按键操作
#define MO_TEMPER  0x07              //温度操作
#define MO_HUMI    0x08              //湿度操作


#define SET_ADDR       0x09              //通知短地址
#define MO_DISTANCE    0x0a              //终端距离协调器的距离
#define REQ_DISCOVE    0x0b              //回复设备地址发现
#define RESP_DISCOVE    0x0c              //回复设备地址发现



typedef struct stgDevNode
{
	SL_NODE_S stNode;
	SL_HEAD_S stHead; /*属性节点头地址*/
	int devid;
	int devtype;
    int range;
}DEV_NODE_S;

extern char *shelpstr[];
void zigbee_devnode_init(void);
void zigbee_devnode_add(int devid,
						int devtype,
						int range);
DEV_NODE_S *zigbee_devnode_find(int devid);

void zigbee_devnode_del(int devid);
void zigbee_devnode_delall(void);
void zigbee_devnode_printal(void);



void zigbee_send_discover(void);

#endif
