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
#define DEVEND_ID_0    0x00
#define DEVEND_ID_ALL  0xff


#define DEVEND_NAME_MAXSIZE 20



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
	SL_HEAD_S stattributelist;                     /*属性节点头地址*/
	int devid;
	int devtype;
    int range;
	int ioldheartnum;
	int iheartnum;
    void *pdata;                                   /*不同属性结构不一样*/
    unsigned char devname[DEVEND_NAME_MAXSIZE];    /*设备的别名*/
}ZIGBEE_DEV_NODE_S;




typedef enum tg_switchstate
{
	 SWITCH_STATE_OFF = 0,
	 SWITCH_STATE_ON
}ZIGBEE_DEV_SWITCH_STATE_E;











API void zigbee_serialsenddiscover(void);
API void zigbee_serialsendswitchcmd(ushort usaddr, uchar onstate);
API void zigbee_serialsendsinglemsg(ushort usaddr, uint tlvtype, void *data, uint len);

API ulong zigbee_serialinit(ushort usport);

API void zigbee_serialfini();






API void zigbee_devnode_init();

API void zigbee_devnode_add(int devid,
						           int devtype,
						           int range);
API ZIGBEE_DEV_NODE_S *zigbee_devnode_find(int devid);
API void zigbee_devnode_del(int devid);
API void zigbee_devnode_delall(void);
API void zigbee_devnode_printall(char *pstr);
API ulong zigbee_devnode_getattrvlaue(int devid, uint uitag, void *pvalue);
API ulong zigbee_devnode_setattrvlaue(int devid, uint uitag, void *pvalue, uint size);


API ulong hali_dev_syntimeout(int itimerfd);


API void zigbee_mqttmsgproc(void *pdata);
API void zigbee_mqttsub(void);
API void zigbee_mqttswitchsub(int devid);
API void zigbee_mqttswitchpub(int devid, uint uistate);
API void zigbee_mqtttemperpub(int devid, uint temper,uint humi);


/*debug类型*/
typedef enum tagzigbeedebugtype
{
    ZIGBEE_DEBUG_PKT = 0,
    ZIGBEE_DEBUG_ERROR,
    ZIGBEE_DEBUG_EVENT,
    ZIGBEE_DEBUG_MAX
}ZIGBEE_DEBUGTYPE_E;

#define MSG_MAX_LEN  256

API void zigbee_setdebugsw(ushort type, BOOL_T bswitch);
API void zigbee_debug(ushort type,const char *cmd, ...);
API void zigbee_setdebug(char *pstr);


#endif

