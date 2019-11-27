#ifndef __HALI_ZIGBEE_
#define __HALI_ZIGBEE_




#define DO_LIGHT    0x01              //开灯操作
#define GET_KEY     0x02              //按键操作
#define GET_TEMPER  0x03              //温度
#define GET_HUMI    0x04              //湿度

#define MO_LIGHT    0x05              //开灯操作
#define MO_KEY      0x06              //按键操作
#define MO_TEMPER   0x07              //温度操作
#define MO_HUMI     0x08              //湿度操作


#define SET_ADDR       0x09              //通知短地址
#define MO_DISTANCE    0x0a              //终端距离协调器的距离
#define REQ_DISCOVE    0x0b              //回复设备地址发现
#define RESP_DISCOVE   0x0c              //回复设备地址发现


/*
 * @brief   | SOP | Data Length  |   CMD   |   Data   |  FCS  |
 *          |  1  |     1        |    2    |  0-Len   |   1   |
*/


/*CMD =   cmd+devid*/
/*cmd*/
#define CMD_COOR       0x00              //发往协调器
#define CMD_END        0x01              //发往终端
/*devid */
#define DEVEND_ID_0 0x00
#define DEVEND_ID_ALL 0xff






extern char *shelpstr[];




typedef enum tg_endevmod
{
	 DEV_MODE_COOR = 1,
	 DEV_MODE_SENSOR,
	 DEV_MODE_SWITCH,
	 DEV_MODE_OTHER,
	 DEV_MODE_MAX,
}ZIGBEE_DEV_MODE_E;


typedef struct stgzigbeedevnode
{
	SL_NODE_S stnode;
	SL_HEAD_S stattributelist; /*属性节点头地址*/
	int devid;
	int devtype;
    int range;
	int ioldheartnum;
	int iheartnum;
}ZIGBEE_DEV_NODE_S;



API void zigbee_serialsenddiscover(void);
API void zigbee_serialsendswitch(ushort usaddr, uchar onstate);

API ulong zigbee_serialinit(ushort usport);

API void zigbee_serialfini();






API void zigbee_devnode_init();

API void zigbee_devnode_add(int devid,
						           int devtype,
						           int range);
API ZIGBEE_DEV_NODE_S *zigbee_devnode_find(int devid);
API void zigbee_devnode_del(int devid);
API void zigbee_devnode_delall(void);
API void zigbee_devnode_printall(void);


API ulong hali_dev_syntimeout(int itimerfd);


API void zigbee_mqttmsgproc(void *pdata);
API void zigbee_mqttsub(void );

#endif

