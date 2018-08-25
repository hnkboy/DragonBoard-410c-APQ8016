#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
struct msgbuf
{
    long mtype;
    char mtex[40];
};
struct msgbuf msre,mswr;
int msid;
int queue_init()
{
    key_t key = ftok(".",123);
    msid = msgget(key,IPC_CREAT|0666);

    if(msid<0)
    {
        perror("get msgid fail");
        return 1;
    }
    return 0;
}
int get_queid()
{
	return msid;
}
int quemsg_snd(long type,char *a,int len)
{
	if (len >40)
	{
		return -1;
	}
	mswr.mtype = type;
	bzero(mswr.mtex,40);
	stpcpy(mswr.mtex, a);
	if(msgsnd(msid,&mswr,strlen(mswr.mtex),0)==-1)//将mswr中的内容放到消息队列msid中放入大小为strlen,未发送则阻塞
	{
		perror("send fail");
		return 1;
	}
	printf("snd massage : %s.   type=%ld\n",mswr.mtex,type);
	return 0;
}
int quemsg_rcv(long type,char *aumsg)
{
	bzero(msre.mtex,40);
	if(msgrcv(msid,&msre,sizeof(msre),type,0)==-1)//从消息队列msid中，获取类型（mtype）为100的信息，放到msre中，未获得则阻塞
	{
		printf("get message fail\n");
		return -1;
	}
	printf("rev massage : %s.   type=%ld \n",msre.mtex,type);
	stpcpy(aumsg,msre.mtex);
	return 0;
}
void queue_fini()
{
	msgctl(msid,IPC_RMID,NULL);
}
