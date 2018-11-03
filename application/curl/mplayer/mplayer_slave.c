#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<dirent.h>
#include<pthread.h>
#include<semaphore.h>
/*命令查询  mplayer -input cmdlist*/
int main(void)
{
	int fd = -1;
	char pathname[50] = {0};
	char name[50] = {0};
	strcat(name, "./result.mp3");
	strcat(pathname, "loadfile ");
	strcat(pathname, "./result.mp3");
	strcat(pathname, "\n");
  	if(access("./cmdfifo", F_OK) == 0)
	{
	//	printf("cmdfifo exist\n");
		unlink("./cmdfifo");
		mkfifo("./cmdfifo", 0777);
	}
	else
	{
		mkfifo("./cmdfifo", 0777);
	}
    
    pid_t pid = fork();
	if (pid < 0)
	{
        printf("fork error");
	}
    else if (pid == 0)
    {
        execlp("mplayer", "mplayer", "-slave", "-quiet", "-idle", "-input", "file=./cmdfifo", NULL, NULL);	//命名管道cmdfifo有命令数据，mplayer会自动读取
        /*fd = open("cmdfifo", O_WRONLY | O_NONBLOCK);
        if(fd < 0)
            printf("open file error %d",fd);
        write(fd, pathname, strlen(pathname));
        close(fd);
        */
    
    }
    else
    {
        sleep(1);
        fd = open("cmdfifo", O_WRONLY | O_NONBLOCK);
        if(fd < 0)
            printf("open file error %d",fd);
        write(fd, pathname, strlen(pathname));

        write(fd, "exit\n",5);
        sleep(10);
        write(fd, "quit\n",5);
        close(fd);

    }
    return 0;
}
