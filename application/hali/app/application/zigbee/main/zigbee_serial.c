#ifdef __cplusplus
extern "C"{
#endif


#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <sys/stat.h>        /* For mode constants */
#include <sys/socket.h>
#include<arpa/inet.h>
#include<errno.h>
#include<fcntl.h>

#include "../../../include/base/hali_base.h"
#include "../../../include/base/list.h"
#include "../../../include/base/hali_error.h"
#include "../../../include/base/hali_thread.h"
#include "../../../include/base/hali_epoll.h"
#include "../../../include/base/hali_timer.h"
#include "../../../include/base/hali_tlv.h"


#include "../include/zigbee.h"

#include "zigbee_serial.h"

#define PKT_TYPE_SEND 0
#define PKT_TYPE_RCV 1



static int g_izigbee_soketfd = -1;
static int g_izigbee_socketacceptfd = -1;

#define SERIAL_BUF_SIZE 1024



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

/*
* @brief
*/
API void zigbee_serialdebugmsg(ushort type,const char *paudata, uint len)
{

    uchar szdata[MSG_MAX_LEN + 1];
    uchar *ptr = NULL;
    memset(szdata,0,sizeof(szdata));
    uint index;
    uint length = 0;
    uint uselen = 0;
    ptr = szdata;

    for(index = 0; ((index < len)&&(MSG_MAX_LEN > uselen)); index++)
    {
        length = snprintf(ptr, (MSG_MAX_LEN - uselen), "%02x ", paudata[index]);
        ptr += length;
        uselen += length;
    }
    if (PKT_TYPE_SEND == type)
    {
        zigbee_debug(ZIGBEE_DEBUG_PKT,"send data\n%s\nlen = %d\n",szdata,len);
    }
    else
    {
        zigbee_debug(ZIGBEE_DEBUG_PKT,"rcv data\n%s\nlen = %d\n",szdata,len);

    }


}


/*
* @brief   | SOP | Data Length  |   MODE | DEVID  |   Data   |  FCS  |
*          |  1  |     1        |    1   | 1      |  0-Len   |   1   |
*/
uint zigbee_serialsend(ushort shortaddr, SL_HEAD_S *pstdatalist){

    uchar ucbuf[ZIGBEE_MAX_DATALEN];
    uint uisendlen = 0;
    HALI_TLV_NODE_S *pstlvnode = NULL;
    ushort ustmp;
    SERIAL_RCVMSG_S *pstmsg = NULL;

    SL_NODE_S *pstnode = NULL;
    SL_NODE_S *pstnext = NULL;
    uchar ptrn = 0;

    if (-1 == g_izigbee_socketacceptfd)
    {

        zigbee_debug(ZIGBEE_DEBUG_ERROR,"g_izigbee_socketacceptfd is invaild");
        return uisendlen;
    }

    pstmsg = (SERIAL_RCVMSG_S *)ucbuf;

    memset(ucbuf, 0 , ZIGBEE_MAX_DATALEN);

    pstmsg->sop = 0xFE;
    pstmsg->addr1 = shortaddr;
    pstmsg->version = 0x01;



    ptrn += sizeof(SERIAL_RCVMSG_S);  /*偏移*/


    if (NULL != pstdatalist)
    {
        SL_FOREACH_SAFE(pstdatalist, pstnode, pstnext)
        {

            pstlvnode = SL_ENTRY(pstnode,HALI_TLV_NODE_S,stnode);
            ucbuf[ptrn ++] = pstlvnode->uitag;
            ucbuf[ptrn ++] = pstlvnode->uilen;
            memcpy(&ucbuf[ptrn], pstlvnode->aucdata, pstlvnode->uilen);
            ptrn += pstlvnode->uilen;
        }
    }

    pstmsg->len = ptrn - sizeof(SERIAL_RCVMSG_S);/*数据长度*/


    ustmp  =  pstmsg->len >> 8;   /*转换字节序*/
    ustmp |= (pstmsg->len << 8) & 0xff00;
    pstmsg->len = ustmp;


    uisendlen = (uint)ptrn;

    #if 0
    printf("\n send \n");
    for(uchar ucnum=0; ucnum < uisendlen; ucnum++)
    {
        printf("%02x ",ucbuf[ucnum]);
    }
    printf("\n len = %d\n",uisendlen);
    #endif
    zigbee_serialdebugmsg(PKT_TYPE_SEND, ucbuf, uisendlen);

    write(g_izigbee_socketacceptfd,"dd",2);

    write(g_izigbee_socketacceptfd,ucbuf,uisendlen);

    return uisendlen;
}

/*
* @brief   | SOP | Data Length  |   MODE | DEVID  |   Data   |  FCS  |
*          |  1  |     1        |    1   | 1      |  0-Len   |   1   |
*/


API void zigbee_serialsenddiscover(void)
{

    HALI_TLV_NODE_S *pstlvnode = NULL;
    SL_HEAD_S stheadlist;
    uchar data[3];
    SL_NODE_S *pstnode = NULL;
    SL_NODE_S *pstnext = NULL;



    sl_init(&stheadlist);
    pstlvnode = hali_tlv_creatnode(REQ_DISCOVE, &data, sizeof(data));
    if(NULL != pstlvnode)
    {
        sl_addafter(&stheadlist, pstlvnode);
    }

    zigbee_serialsend(0xFFFF, &stheadlist);

    sl_freeall(&stheadlist, hali_tlv_freenode);

    return;
}

API void zigbee_serialsendswitchcmd(ushort usaddr, uchar onstate)
{

    HALI_TLV_NODE_S *pstlvnode = NULL;
    SL_HEAD_S stheadlist;
    uchar data;
    SL_NODE_S *pstnode = NULL;
    SL_NODE_S *pstnext = NULL;


    data = onstate;

    sl_init(&stheadlist);
    pstlvnode = hali_tlv_creatnode(DO_LIGHT, &data, sizeof(data));
    if(NULL != pstlvnode)
    {
        sl_addafter(&stheadlist, pstlvnode);
    }

    zigbee_serialsend(usaddr, &stheadlist);

    sl_freeall(&stheadlist, hali_tlv_freenode);

    return;
}

API void zigbee_serialsendsinglemsg(ushort usaddr, uint tlvtype, void *data, uint len)
{

    HALI_TLV_NODE_S *pstlvnode = NULL;
    SL_HEAD_S stheadlist;
    SL_NODE_S *pstnode = NULL;
    SL_NODE_S *pstnext = NULL;

    sl_init(&stheadlist);
    pstlvnode = hali_tlv_creatnode(tlvtype, data, len);
    if(NULL != pstlvnode)
    {
        sl_addafter(&stheadlist, pstlvnode);
    }
    zigbee_serialsend(usaddr, &stheadlist);

    sl_freeall(&stheadlist, hali_tlv_freenode);

    return;
}


void zigbee_serialmsgntoh(SERIAL_RCVMSG_S *pstmsg)
{

    pstmsg->addr1 = ntohs(pstmsg->addr1);
    pstmsg->addr2 = ntohs(pstmsg->addr2);
    pstmsg->addr3 = ntohs(pstmsg->addr3);
    pstmsg->crc =   ntohs(pstmsg->crc);
    pstmsg->len =   ntohs(pstmsg->len);
    return;
}
void zigbee_serialsendcmd(uint8_t cmd,
                                         uint8_t devid,
                                         uint8_t tlvtype,
                                         uint8_t tlvalue,
                                         uint8_t tlvlen){

	char buf[8];
	char i;

    //printf("zigebee teset\n ");

	buf[0] = 0xFE;

	buf[1] = 0x03;  /*数据长度*/

	buf[2] = cmd;  /* CMD */
	buf[3] = devid;

	buf[4] = tlvtype;
	buf[5] = tlvlen;
	buf[6] = tlvalue;

	buf[7] = 0x00;

    //for(i=0; i<8; i++){
    //    printf("%2x ",buf[i]);
    //}

    zigbee_serialdebugmsg(PKT_TYPE_SEND, buf, 8);
	write(g_izigbee_socketacceptfd, buf, 8);
}
/*
* @brief
*/
ulong zigbee_serialmsgproc(IN uchar *aucbuf,IN int msglen,INOUT int *premainlen)
{

    ulong ulret = ERROR_SUCCESS;
    uint uiptrn = 0;
    uint uitmp = 0;
    SERIAL_RCVMSG_S *pstmsg = NULL;
    SERIAL_RCVMSG_TLV_HEAD_S *pstmsgtlvhead = NULL;
	uchar strbuf[ZIGBEE_MAX_DATALEN];
    ushort ustmp;
    int datalen;
	ZIGBEE_DEV_NODE_S *pstdevnode = NULL;



    /*校验长度*/
    if (sizeof(SERIAL_RCVMSG_S) > msglen)
    {
       *premainlen = 0;
        return ulret;
    }
    /*解析头部*/
    pstmsg = (SERIAL_RCVMSG_S *)aucbuf;
    /*字节序转换*/
    zigbee_serialmsgntoh(pstmsg);

    /*校验长度*/

    if (msglen  < (pstmsg->len + sizeof(SERIAL_RCVMSG_S)))
    {
        *premainlen = 0;
        return ulret;
    }
    else
    {
        *premainlen = msglen - (pstmsg->len + sizeof(SERIAL_RCVMSG_S));
    }


    /*目的地址不属于自己的消息不处理*/
    if(LOCAL_ADDR != pstmsg->addr1 )
    {
        return ERROR_SUCCESS;
    }
    else if(LOCAL_COOR == pstmsg->addr2 )
    {
        /* 协调器的消息暂不处理*/
        return ERROR_SUCCESS;
    }

    ustmp  =  pstmsg->addr2 >> 8;   /*转换字节序*/
    ustmp |= (pstmsg->addr2 << 8) & 0xff00;
    pstmsg->addr2 = ustmp;

    for (uiptrn += sizeof(SERIAL_RCVMSG_S);
         uiptrn < (pstmsg->len + sizeof(SERIAL_RCVMSG_S));
         uiptrn += pstmsgtlvhead->len)
    {

        pstmsgtlvhead = (SERIAL_RCVMSG_TLV_HEAD_S *)&aucbuf[uiptrn];
        uiptrn += sizeof(SERIAL_RCVMSG_TLV_HEAD_S);

        memset(strbuf, 0, ZIGBEE_MAX_DATALEN);
        memcpy(&strbuf,&aucbuf[uiptrn],pstmsgtlvhead->len);
        switch(pstmsgtlvhead->type)
        {
            case TLV_RESP_DISCOVE:
            {
                SERIAL_TLV_RESP_DISCOVE_S *pstrespdiscove;
                pstrespdiscove = (SERIAL_TLV_RESP_DISCOVE_S *)strbuf;
                zigbee_devnode_add((int)pstmsg->addr2, pstrespdiscove->devmode,pstmsg->rssi);
                break;
            }
            case TLV_RESP_SWITCH_STATE:
            {
                ulret = zigbee_devnode_setattrvlaue((int)pstmsg->addr2,
                                                    TLV_RESP_SWITCH_STATE,
                                                    (VOID *)(SERIAL_TLV_RESP_SWITCH_S *)strbuf,
                                                    pstmsgtlvhead->len);
                if (ERROR_SUCCESS == ulret)
                {
                    zigbee_mqttswitchpub((int)pstmsg->addr2, *(uchar *)strbuf);
                }
                else
                {
                    printf("get switch state error\n");
                }
                break;
            }
            case TLV_MO_GET_TEMPHUMI:
            {

                SERIAL_TLV_MO_GET_TEMPHUMI *psttemper;
                psttemper = (SERIAL_TLV_MO_GET_TEMPHUMI *)strbuf;
                /*检查合法性*/
                if ((100 <= psttemper->temper) || (100 <= psttemper->humi))
                {
                    //printf(" temper or humi is  invalid \n");
                    zigbee_debug(ZIGBEE_DEBUG_ERROR, " temper or humi is  invalid \n");
                    break;
                }
                ulret = zigbee_devnode_setattrvlaue((int)pstmsg->addr2,
                                                    TLV_MO_GET_TEMPHUMI,
                                                    (VOID *)(SERIAL_TLV_MO_GET_TEMPHUMI *)strbuf,
                                                    pstmsgtlvhead->len);

                if (ERROR_SUCCESS == ulret)
                {
                    zigbee_mqtttemperpub((int)pstmsg->addr2, psttemper->temper,psttemper->humi);
                }
                else
                {
                    //printf("get temper and humi state error\n");
                    zigbee_debug(ZIGBEE_DEBUG_ERROR, "get temper and humi state error\n");
                }
                break;
            }

            default:break;

        }
    }

    //zigbee_devnode_printall();
    //zigbee_serialsenddiscover();
    return ERROR_SUCCESS;
}


void zigbee_sendserialbroadcast(void){

    zigbee_serialsendcmd(0,0xff,0,0,0);
}

ulong zigbee_serialmsgcallback(int fd)
{
    ulong ulerrcode = ERROR_SUCCESS;
    int istrlen;
    uchar aucbuf[SERIAL_BUF_SIZE];

    memset(aucbuf,0,sizeof(aucbuf));
    while(1)
    {
        istrlen=read(fd, aucbuf, SERIAL_BUF_SIZE);
        if(0 == istrlen)
        {
            (void *)hali_epoll_del(fd);
            close(fd);
            g_izigbee_socketacceptfd = -1;
            break;
        }
        else if(0 > istrlen)
        {
            if(errno == EAGAIN)//标准输入里没有数据
            //printf("zigbee serial read error");
            break;
        }
        else
        {

            int t;
            int i;
            int remainlen = 0;
            aucbuf[istrlen] = 0;
            #if 0
            printf("\n");
            for (i = 0; i < istrlen; i++)
            {
                aucbuf[istrlen] = 0;
                printf("%02x ",aucbuf[i]);
            }
            printf("\n len = %d\n",istrlen);
            #endif
            //write(fd,aucbuf,istrlen);

            remainlen = istrlen;

            zigbee_serialdebugmsg(PKT_TYPE_RCV, aucbuf, istrlen);

            for ( ; 0 < remainlen; )
            {
                (void)zigbee_serialmsgproc(&aucbuf[istrlen - remainlen],remainlen, &remainlen);

            }
            memset(aucbuf,0,sizeof(aucbuf));


            #if 0
            if ((0x0D == buf[0]) && (0x0a == buf[1])){
                uartchar_proc(buf,str_len);
            }
            else{
                prevpkt_tlvproc(buf,str_len);
                //printf("%s\n",buf);
                //quemsg_snd_voice(buf,sizeof(buf));
            }
            #endif
#if 0
            /*暂时解决一个bug*/
            for(t=0;str_len>t;t++)
            {
            buf[t] = 0xff;
            }
            write(pevents[i].data.fd,buf,str_len);
#endif

        }

    }

    return ulerrcode;
}

ulong zigbee_serialconnectcallback(int fd)
{
    ulong ulerrcode = ERROR_FAILED;
    int iaccsocketfd = -1;
    struct sockaddr staddr;
    int isize;
    int flag;
    iaccsocketfd = accept(fd,(struct sockaddr*)&staddr,&isize);
    if (-1 != iaccsocketfd)
    {
        //将套接字设置成非阻塞模式
        flag = fcntl(iaccsocketfd, F_GETFL, 0);
        fcntl(iaccsocketfd, F_SETFL, flag | O_NONBLOCK);

        if (-1  != g_izigbee_socketacceptfd)
        {
            (void *)hali_epoll_del(g_izigbee_socketacceptfd);
            close(g_izigbee_socketacceptfd);
            g_izigbee_socketacceptfd = -1;
        }
        g_izigbee_socketacceptfd = iaccsocketfd;

        ulerrcode = hali_epoll_add(iaccsocketfd, zigbee_serialmsgcallback);
        if(ERROR_SUCCESS == ulerrcode)
        {
            printf("zigbee serial accept success\r\n");
        }
    }
    return ulerrcode;
}

API ulong zigbee_serialinit(ushort usport)
{

    struct sockaddr_in staddr;
    int izigbeeserifd = -1 ;

    int on;
    int iret = 0;
    ulong ulerr = ERROR_FAILED;

    izigbeeserifd = socket(AF_INET, SOCK_STREAM, 0);
    iret = setsockopt(izigbeeserifd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
    if (0 != iret)
    {   g_izigbee_soketfd = izigbeeserifd;
        printf("setsockopt() error");
    }
    memset(&staddr,0,sizeof(staddr));

    staddr.sin_family=AF_INET;
    staddr.sin_addr.s_addr= htonl(INADDR_ANY);
    staddr.sin_port= htons(usport);

    if(bind(izigbeeserifd,(struct sockaddr*)&staddr,sizeof(staddr))==-1)
    {
        printf("bind() error");
        close(izigbeeserifd);
        return ulerr;
    }

    if(-1 == listen(izigbeeserifd, 5))
    {
        printf("listen() error");
        close(izigbeeserifd);
        return ulerr;
    }

    ulerr = hali_epoll_add(izigbeeserifd, zigbee_serialconnectcallback);
    if(ERROR_SUCCESS != ulerr)
    {
        printf(" zigbee epolll add error");
    }
    return ulerr;
}

API void zigbee_serialfini()
{
    if (-1 != g_izigbee_soketfd)
    {
        shutdown(g_izigbee_soketfd, SHUT_RDWR);
        (void)hali_epoll_del(g_izigbee_soketfd);
    }
    if (-1 != g_izigbee_socketacceptfd)
    {
        (void)hali_epoll_del(g_izigbee_soketfd);
    }
    return;
}

#ifdef __cplusplus
}
#endif


