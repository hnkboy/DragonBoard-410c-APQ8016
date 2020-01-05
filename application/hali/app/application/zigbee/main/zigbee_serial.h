#ifndef __HALI_ZIGBEESERIAL_
#define __HALI_ZIGBEESERIAL_


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

#define ZIGBEE_MAX_DATALEN 50         /*与zigbee 通信 数据缓冲区大小*/

typedef enum tgtlvtype
{
    TLV_DO_LIGHT = 1,         /*开灯操作*/
    TLV_GET_KEY,
    TLV_GET_TEMPER,
    TLV_GET_HUMI,

    TLV_RESP_DO_LIGHT = 0x05,
    TLV_RESP_GET_KEY,
    TLV_RESP_GET_TEMPER,
    TLV_RESP_GET_HUMI,

    TLV_SET_ADDR     = 0x09,
    TLV_MO_DISTANCE  = 0x0a,
    TLV_REQ_DISCOVE  = 0x0b,
    TLV_RESP_DISCOVE = 0x0c,   /*回复设备地址发现*/

    TLV_REQ_SWITCH_STATE  = 0x0f,
    TLV_RESP_SWITCH_STATE = 0x10,
    TLV_REQ_DO_SWITCH   = 0x11,
    TLV_RESP_DO_SWITCH  = 0x12,
    TLV_MO_GET_TEMPHUMI = 0x13,
    TLV_MO_DEVNAME,            /* 设备别名      */

    TLV_MAX,
}SERIAL_TLV_E;



/*
* @brief   | SOP | Data Length  |   MODE | DEVID  |   Data   |  FCS  |
*          |  1  |     1        |    1   | 1      |  0-Len   |   1   |
*/

typedef enum tgtlvmsgtype
{

     SERIAL_MSG_COOR = 0,     /*发向协调器*/
     SERIAL_MSG_END,      /*发向终端*/
     SERIAL_MSG_LINUX,     /*linux*/
     SERIAL_MSG_MAZ,
}SERIAL_MSG_TYPE_E;

#define LOCAL_ADDR 0xdeaf
#define LOCAL_COOR 0x0001

typedef struct stserialmsghead
{
    uchar  sop;
    uchar  version;
    ushort len;
    ushort addr1;
    ushort addr2;
    ushort addr3;
    uchar  rssi;
    uchar  pad[3];
    ushort crc;
    uchar data[0];
}SERIAL_RCVMSG_S;
typedef struct stserialmsgtlv
{
    uchar type;
    uchar len;   /*设备ID*/
}SERIAL_RCVMSG_TLV_HEAD_S;

typedef struct sttyperespdiscovevalue
{
    uchar devid;   /*设备ID*/
    uchar devmode; /*设备类型*/
    uchar rssi;    /*设备信号强度*/
}SERIAL_TLV_RESP_DISCOVE_S;

typedef struct sttyperespswitchstate
{
    uchar switchstate;   /*设备ID*/
}SERIAL_TLV_RESP_SWITCH_S;

typedef struct sttyperesptemphumi
{
    uchar temper;
    uchar humi;
}SERIAL_TLV_MO_GET_TEMPHUMI;




#endif


