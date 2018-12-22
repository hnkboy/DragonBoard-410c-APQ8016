
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


#define ZIGBEE_MAX_DATALEN 50         /*与zigbee 通信 数据缓冲区大小*/

typedef struct stgDevNode
{
	SL_NODE_S stNode;
	SL_HEAD_S stTlvHead; /*属性节点头地址*/
	int devid;
	int devtype;
    int range;
	int ioldheartnum;
	int iheartnum;
}DEV_NODE_S;

typedef struct stgEndNode
{
	SL_NODE_S stNode;
    unsigned int uiTag;    /*T*/
    unsigned int uiLen;  /*L*/
    unsigned char *pValue; /*V*/
}END_NODE_S;



extern char *shelpstr[];



void zigbee_endtlvnode_del(SL_HEAD_S *pstHead,
						          uint32_t uiTag);

END_NODE_S *zigbee_endtlvnode_find(SL_HEAD_S *pstHead,
										 uint32_t uiTag);

void zigbee_endtlvnode_add(SL_HEAD_S *pstHead,
							     uint32_t uiTag,
    							 uint8_t *pValue,
    						     uint32_t uiLen);
void zigbee_endtlvnode_delall(SL_HEAD_S *pstHead);


void zigbee_endtlvnode_printall(SL_HEAD_S *pstHead);



void zigbee_devnode_init(void);
void zigbee_devnode_add(int devid,
						int devtype,
						int range);
DEV_NODE_S *zigbee_devnode_find(int devid);

void zigbee_devnode_del(int devid);
void zigbee_devnode_delall(void);
void zigbee_devnode_printall(void);



void zigbee_send_discover(void);

void zigbee_send_cmd(uint8_t cmd,
                    uint8_t devid,
                    uint8_t tlvtype,
                    uint8_t tlvalue,
                    uint8_t tlvlen);
void zigbee_send_tlvbysl(uint8_t cmd,uint8_t devid,SL_HEAD_S *pstHead);
void zigbee_devnode_print2json(char *poutbuf,uint16_t *plen);
void zigbee_devproc_syncdata(void);
void zigbee_devproc_checkheart(void);

#endif
