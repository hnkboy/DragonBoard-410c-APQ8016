#include <fcntl.h>
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define MQNAME "/mqtest"
mqd_t mqd;
int mqueue_init()
{
	mqd = mq_open(MQNAME, O_RDWR|O_CREAT, 0600, NULL);
    if(mqd<0)
    {
        perror("mq_open fail");
        return 1;
    }
    return 0;
}
int get_mqfd()
{
	return mqd;
}
int quemsg_rcv(long type,char *aumsg)
{
	bzero(msre.mtex,40);
	if(msgrcv(msid,&msre,sizeof(msre),type,0)==-1)//从消息队列msid中，获取类型（mtype）为100的信息，放到msre中，未获得则阻塞
	{
		printf("get message fail\n");
		return -1;
	}
	printf("rev massage : %s\n",msre.mtex);
	stpcpy(aumsg,msre.mtex);
	return 0;
}
void queue_fini()
{
	msgctl(msid,IPC_RMID,NULL);
}
