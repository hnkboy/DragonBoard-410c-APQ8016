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
#include "playback.h"
#include "playback_wav.h"

#include <curl/curl.h>
#include "queue.h"

#define BUF_SIZE 30
#define EPOLL_SIZE 20
#define PKTMQNAME "/pktmq"
mqd_t mqd;

int mqueue_send2pkt(const char *buf,long len)
{
	int ret;
    ret = mq_send(mqd, buf, len, 2);
    if (ret == -1) {
        perror("mq_send()");
		return -1;
    } 
    printf("send pkt mqueue msg: %s\n",buf);
	return 0;
    
}
void error_handler(const char* message);
void playvoice(char *pbuf ,unsigned int lenth)
{
    char svstr[100] = {0};
    if (0 != pbuf) 
    {
        sprintf(svstr,"./voice/%s",pbuf);
        printf("%s\n",pbuf);
        playback_wav(svstr);    
    }
}


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
//		printf("%2x", temp);
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
					if (0x1 ==Uart2_Buffer[(Uart2_Buffer[3]+4)]){
						gsm_flag=1;						
					}
				}
			if(Uart2_Rx==40){ Uart2_Rx=0; } break;
			default:break;
			}
	}
}


void *prevpktmain(void *p)
{
    int serv_sock,clnt_sock;
    struct sockaddr_in serv_addr,clnt_addr;
    int clnt_addr_sz;

    char buf[BUF_SIZE];
    int str_len;

    int ep_fd,ep_cnt,i,flag;
    struct epoll_event event;
    struct epoll_event* pevents;
    int ret = 0;
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

    serv_sock=socket(AF_INET,SOCK_STREAM,0);

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port= htons(2000);

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
        error_handler("bind() error");

    if(listen(serv_sock,5)==-1)
        error_handler("listen() error");

    ep_fd=epoll_create(EPOLL_SIZE);
    event.events=EPOLLIN;
    event.data.fd=serv_sock;
    pevents=malloc(sizeof(struct epoll_event)*EPOLL_SIZE);
    epoll_ctl(ep_fd,EPOLL_CTL_ADD,serv_sock,&event);

    event.events=EPOLLIN;
    event.data.fd=mqd;
    epoll_ctl(ep_fd,EPOLL_CTL_ADD,mqd,&event);

    while( 0 == ret)
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
                        //write(pevents[i].data.fd,buf,str_len);
						if (0x0D == buf[0]){
							uartchar_proc(buf,str_len);
						}
						else{
                       		//buf[str_len-1] = 0;
                        	buf[str_len] = 0;
                        	printf("%s\n",buf);
                        	playvoice(buf,sizeof(buf));
						}
                    }

                }
            }
            else if(mqd == pevents[i].data.fd)
            {
                char rbuf[BUFSIZ];
                int val;
				ret = mq_receive(mqd, rbuf, BUFSIZ, &val);
				if (ret == -1) {
					perror("pkt mq_receive err()");
				}
                printf("rcv mqueue msg %s ,prio:%d\n",rbuf,val);
				if(strcmp(rbuf,"exit")==0)
       			{
					(void)mq_close(mqd);
                    ret = -1;
					break;
        		}	
            }
        }
    }

    close(serv_sock);
    close(ep_fd);
    printf("pkt thread exit\n");
    return 0;
}
void error_handler(const char* message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
