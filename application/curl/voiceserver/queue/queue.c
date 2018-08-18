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

int msid;
struct msgbuf msre,mswr;

void * f(void * buf)
{
    while(1)//子线程
    {
        bzero(msre.mtex,40);
        if(msgrcv(msid,&msre,sizeof(msre),100,0)==-1)//从消息队列msid中，获取类型（mtype）为100的信息，放到msre中，未获得则阻塞
        {
            printf("get message fail\n");
            exit(-1);
        }
        printf("rev massage : %s\n",msre.mtex);

        if(strcmp(msre.mtex,"end")==0)
        {
            msgctl(msid,IPC_RMID,NULL);
            exit(0);
        }
        printf("Please import massage or wait for massage:\n");
    }
}

int main (void)
{
    key_t key = ftok(".",123);
    msid = msgget(key,IPC_CREAT|0666);

    if(msid<0)
    {
        perror("get msgid fail");
        return 0;
    }

    mswr.mtype = 100;
    char a[40] = {'\0'};

    pthread_t id;

    pthread_create(&id,NULL,f,NULL);
    printf("Please import massage or wait for massage:\n");//创建线程
    while(1)//主线程
    {

        bzero(mswr.mtex,40);
 ///       gets(a);
        scanf("%s",a);
        stpcpy(mswr.mtex,a);
        if(msgsnd(msid,&mswr,strlen(mswr.mtex),0)==-1)//将mswr中的内容放到消息队列msid中放入大小为strlen,未发送则阻塞
        {
            perror("send fail");
            return 0;
        }
        printf("massage : %s\n",mswr.mtex);
        if(strcmp(msre.mtex,"end")==0)
        {
            msgctl(msid,IPC_RMID,NULL);
            exit(0);
        }
        printf("Please import massage or wait for massage:\n");

    }
    pthread_join(id,NULL);

}
