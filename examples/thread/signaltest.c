#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<pthread.h>
#include<time.h>
pthread_t tid;　
sigset_t set;
void myfunc()
{
	printf("hello\n");
}
static void *mythread(void*p){
	int signum;
	while(1){
		sigwait(&set,&signum);
		if(SIGUSR1==signum)
		myfunc();
		if(SIGUSR2==signum)
		{
		printf("Iwillsleep2secondandexit\n");
		sleep(2);
		break;
		}
	}
}
int main()
{
	char tmp;
	void *status;
	sigemptyset(&set);
	sigaddset(&set,SIGUSR1);
	sigaddset(&set,SIGUSR2);
	sigprocmask(SIG_SETMASK,&set,NULL);
	pthread_create(&tid,NULL,mythread,NULL);
    while(1)
    {    
        printf(":");
        scanf("%c",&tmp);
        if('a'==tmp)
        {
            pthread_kill(tid,SIGUSR1);//发送SIGUSR1，打印字符串。
        }
        else if('q'==tmp)
        {
			//发出SIGUSR2信号，让线程退出，如果发送SIGKILL，线程将直接退出。
			pthread_kill(tid,SIGUSR2);
			//等待线程tid执行完毕，这里阻塞。
			pthread_join(tid,&status);
			printf("finish\n");
			break;
        }
        else
            continue;
    }
    return0;
}
