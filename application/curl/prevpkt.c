#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<errno.h>
#include<fcntl.h>
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdlib.h>
#include "alsaplay.h"
#include "pvoice.h"
#include <curl/curl.h>
#include "queue.h"
#include "prevpkt.h"
#include "lib/list.h"
#include "device/zigbee.h"

#define BUF_SIZE 30
#define EPOLL_SIZE 20
#define PKTMQNAME "/pktmq"
mqd_t mqd;
mqd_t mqd_w;

int serv_sock,clnt_sock;
void pkt_send(char *pbuf, int lent)
{
    write(clnt_sock,pbuf,lent);
}
int mqueue_send2pkt(const char *buf,long len)
{
	int ret;
    ret = mq_send(mqd, buf, strlen(buf), 2);
    if (ret == -1) {
        perror("mq_send()");
		return -1;
    }
    printf("send to pkt mqueue msg: %s.\n",buf);
	return 0;

}
void error_handler(const char* message);

/*zibee 发送的字符串处理*/
void uartchar_proc(char *buf, unsigned int len)
{
	unsigned char temp,gsm_flag;
	unsigned char str2 = 0x0;
	unsigned char Uart2_Buffer[50];
	unsigned char Uart2_Rx=0;
	unsigned char i=0;
	char buf1[100];
    unsigned int index = 0;
    for (;index<len;index++)
	{
		//printf("redatad: nread = %s\n\n\r", buf1);
		temp = buf[index];
		//printf("tmp=%x\n\n\r", temp);
		printf("%2x", temp);
		//printf("str2=%d\n\n\r", str2);
		switch(str2){
			case 0x0:
				if (temp == 0x0D)str2=0x1;break;
			case 0x1:
				if (temp == 0x0A)str2=0x2;else str2=0x0;break;
			case 0x2:
				if (temp == 0xef)str2=0x0;else {str2=0x3;Uart2_Buffer[Uart2_Rx]=temp; Uart2_Rx++;}break;
			case 0x3:
				Uart2_Buffer[Uart2_Rx]=temp;
				Uart2_Rx++;
				if(Uart2_Buffer[Uart2_Rx-1]==0x0D){
					for (i=0;i<Uart2_Rx-1;i++){
					printf("%02x ",Uart2_Buffer[i]);
					}
					printf("\n\r");
					str2=0x0;
					Uart2_Rx=0;
					//if (0x1 ==Uart2_Buffer[(Uart2_Buffer[3]+4)]){
                    //    quemsg_snd_voice("opendoor.mp3","70");
					//}
					if (0x1 ==Uart2_Buffer[3]){
                        quemsg_snd_voice("opendoor.mp3","50");
					}
				}
			if(Uart2_Rx==40){ Uart2_Rx=0; } break;
			default:break;
			}
	}
}


void *prevpktmain(void *p)
{
    struct sockaddr_in serv_addr,clnt_addr;
    int clnt_addr_sz;

    char buf[BUF_SIZE];
    int str_len;

    int on;
    int ep_fd,ep_cnt,i,flag;
    struct epoll_event event;
    struct epoll_event* pevents;
    int ret = 0;
    _Bool bisbreak = 0;
   /*关于设备的初始化*/
    zigbee_devnode_init();
    int count;/*facgi 请求数目 ** ****/
   /*
    if(argc!=2)
    {
        printf("Usage %s <port>\n",argv[0]);
        exit(1);
    }*/

    printf("hello pkt thread \n");
    /*mqueue*/
  	mqd = mq_open(PKTMQNAME, O_RDWR|O_CREAT, 0600, NULL);
    if (mqd == -1) {
        perror("pkt mq_open()");
        exit(1);
    }
    /*fcgi mqueue*/
    mqd_w = mq_open("/fastcgi_write", O_RDWR|O_CREAT, 0600, NULL);
    if (mqd_w == -1)
    {
        perror("pkt fcgi  mq_open()");
        exit(1);
    }

    serv_sock=socket(AF_INET,SOCK_STREAM,0);
    on = 1;
    ret = setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port= htons(2000);

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
    {
        error_handler("bind() error");
        return NULL;
    }

    if(listen(serv_sock,5)==-1)
    {
        error_handler("listen() error");
        return NULL;
    }

    ep_fd=epoll_create(EPOLL_SIZE);
    event.events=EPOLLIN;
    event.data.fd=serv_sock;
    pevents=malloc(sizeof(struct epoll_event)*EPOLL_SIZE);
    epoll_ctl(ep_fd,EPOLL_CTL_ADD,serv_sock,&event);

    event.events=EPOLLIN;
    event.data.fd=mqd;
    epoll_ctl(ep_fd,EPOLL_CTL_ADD,mqd,&event);

    while( 0 == bisbreak)
    {
        ep_cnt=epoll_wait(ep_fd,pevents,EPOLL_SIZE,-1);
        for(i=0;i<ep_cnt;i++)
        {
            if(serv_sock==pevents[i].data.fd)
            {
                clnt_addr_sz=sizeof(clnt_addr);
                clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_sz);
                //将套接字设置成非阻塞模式
                flag=fcntl(clnt_sock,F_GETFL,0);
                fcntl(clnt_sock,F_SETFL,flag|O_NONBLOCK);

                event.events=EPOLLIN|EPOLLET;
                event.data.fd=clnt_sock;
                epoll_ctl(ep_fd,EPOLL_CTL_ADD,clnt_sock,&event);
			    g_ZigbeeFd = clnt_sock;
            }
            else if(clnt_sock==pevents[i].data.fd)
            {
                while(1)
                {
                    str_len=read(pevents[i].data.fd,buf,BUF_SIZE);
                    if(str_len==0)
                    {
                        epoll_ctl(ep_fd,EPOLL_CTL_DEL,pevents[i].data.fd,NULL);
                        close(pevents[i].data.fd);
                        break;
                    }
                    else if(str_len<0)
                    {
                        if(errno==EAGAIN)//标准输入里没有数据
                            break;
                    }
                    else
                    {
                        int t;
                        int i;
                        for ( i = 0;i<str_len;i++){
                        	buf[str_len] = 0;
                        	printf("%2x ",buf[i]);}
                        printf("\nlen = %d\n",str_len);
                        //write(pevents[i].data.fd,buf,str_len);
						if ((0x0D == buf[0]) && (0x0a == buf[1])){
							uartchar_proc(buf,str_len);
						}
						else{
                            prevpkt_tlvproc(buf,str_len);
                        	//printf("%s\n",buf);
                        	//quemsg_snd_voice(buf,sizeof(buf));
						}
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
            }
            else if(mqd == pevents[i].data.fd)
            {
                char rbuf[BUFSIZ] = {0};
                int val;
				ret = mq_receive(mqd, rbuf, BUFSIZ, &val);
				if (ret == -1) {
					perror("pkt mq_receive err()");
				}
                printf("pktthr rcv mqueue msg, prio: %d value: %s\r",val,rbuf);
				if(strcmp(rbuf,"exit")==0)
       			{
					(void)mq_close(mqd);
                    bisbreak = 1;
					break;
        		}
				else
				{
                    char wbuf[BUFSIZ] = {0};
                    int val = 1,ret;
					bzero(wbuf,BUFSIZ);
					sprintf(wbuf,"\r\n hello fcgi %d\r\n",count);
					ret = mq_send(mqd_w, wbuf, BUFSIZ, val);
					if (ret == -1) {
						perror("pkt mq_receive err()");
					}
					printf("send mqueue msg, prio: %d value: %s\r",val,wbuf);
					count ++;
				}
            }
        }
    }

    //close(clnt_sock);
    //close(serv_sock);
    shutdown(serv_sock,SHUT_RDWR);
    close(ep_fd);
	(void)mq_close(mqd_w);

    zigbee_devnode_delall();

    printf("pkt thread exit\n");
    return 0;
}
void error_handler(const char* message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    //exit(1);

}
void prevpkt_tlvproc(char *arg,char len){
    uint16_t tag,i;
    uint16_t devid;
    uint16_t tlvlen;
	uint8_t strbuf[ZIGBEE_MAX_DATALEN];
	uint8_t *pstrbuf;

	DEV_NODE_S *pstDevNode = NULL;
    if (arg[0] != len)
        return;
    if ((arg[1] != 0xff)&&(arg[1] != 0x02))
        return;
    devid = arg[2];
    pstDevNode = zigbee_devnode_find(devid);
    if(NULL == pstDevNode){
        zigbee_devnode_add(devid,0,0);
        pstDevNode = zigbee_devnode_find(devid);
        if(NULL == pstDevNode){
            return;
        }
    }
    for(i = 0; i < len-3;)
    {
        tag = arg[i+3];
        tlvlen = arg[i+4];
		pstrbuf = &arg[i+5];
        switch(tag)
        {
            case GET_KEY:
                printf("get key value = %d\n\r", pstrbuf[0]);
                quemsg_snd_voice("opendoor.mp3","40");
                break;
            case MO_DISTANCE:
                printf("get distance = %d\n\r", pstrbuf[0]);
                pstDevNode->range = pstrbuf[0];
                break;
            case RESP_DISCOVE:
                printf("get discove = %d\n\r", pstrbuf[0]);

				pstDevNode = zigbee_devnode_find(pstrbuf[0]);
				if(NULL == pstDevNode){
					zigbee_devnode_add(pstrbuf[0],pstrbuf[1],pstrbuf[2]);
				}
				else{
					pstDevNode->devtype = pstrbuf[1];
					pstDevNode->range   = pstrbuf[2];
				}
                break;
			case MO_HUMI:
				itoa(pstrbuf[0], strbuf, 10);
				//memcpy(strbuf,pstrbuf,tlvlen);
				zigbee_endtlvnode_add(&pstDevNode->stTlvHead,tag,strbuf,tlvlen);
			case MO_TEMPER:
				itoa(pstrbuf[0], strbuf, 10);
				//memcpy(strbuf,pstrbuf,tlvlen);
				zigbee_endtlvnode_add(&pstDevNode->stTlvHead,tag,strbuf,tlvlen);
            default:break;
        }
        i += tlvlen +2;

    }
}
