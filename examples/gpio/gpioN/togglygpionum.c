/*
============================================================================
Name : gpionum.c
Author : jiali
Version : 0.0.2
Copyright : Your copyright notice
Description : Basic Hardware access, Ansi-style
============================================================================
*/
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define MAX_BUF 30
#define GPIO_A 36
#define GPIO_B 12
int Export_GPIO(int gpio);
int UnExport_GPIO(int gpio);
int Write_GPIO(int gpio, int value);
int Read_GPIO(int gpio, int *value);
int main(void)
{
    char c=' ';
    int ret;
    int out_value = 1;
    int in_value = 0;
    //
    //将制定的GPIO口export
    //
    ret = Export_GPIO(GPIO_A);
    if(ret != 0)
    printf("Error exporting GPIO_%d", GPIO_A);
    ret = Export_GPIO(GPIO_B);
    if(ret != 0)
     printf("Error exporting GPIO_%d", GPIO_B);
        //
        //向指定的GPIO口写值
        //
    printf("press any key to toggle GPIO_A or 'q' to quit:\n");
    while(c != 'q'){
        printf("Writing GPIO_%d: value=%d \n", GPIO_A, out_value);
        ret = Write_GPIO(GPIO_A, out_value);
        if (ret != 0)
            printf("Error writing GPIO_%d", GPIO_A);
		ret = Read_GPIO(GPIO_B, (int*) &in_value);
        if (ret != 0)
			 printf("Error reading GPIO_%d", GPIO_B);
        printf("Reading GPIO_%d: value=%d \n", GPIO_B, in_value);
        out_value = !out_value;
	
        c= getchar();
	}
    //
    //将指定的GPIO口进行unexport
    //
    ret = UnExport_GPIO(GPIO_A);
    if(ret != 0)
        printf("Error UnExporting GPIO_%d", GPIO_A);
    ret = UnExport_GPIO(GPIO_B);
    if(ret != 0)
        printf("Error UnExporting GPIO_%d", GPIO_B);
    return 0;
}
int Export_GPIO(int gpio){
    int fd;
    char buf[MAX_BUF];
    //将指定位置的GPIO进行export
    //在/sys/class/gpio目录下将生成一个可用的GPIO文件
    sprintf(buf, "%d", gpio);
    //打开export文件夹并对其进行操作
    fd = open("/sys/class/gpio/export", O_WRONLY);
    if(fd < 0)
        return -1;
    write(fd, buf, strlen(buf));
    close(fd);
    return 0;
}
int UnExport_GPIO(int gpio){
    int fd;
    char buf[MAX_BUF];
    sprintf(buf, "%d", gpio);
    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if(fd < 0)
        return -1;
    write(fd, buf, strlen(buf));
    close(fd);
    return 0;
}
int Write_GPIO(int gpio, int value){
    int fd;
    char buf[MAX_BUF];
    //将GPIO方向设置为输出
    sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);
    fd = open(buf, O_WRONLY);
    if(fd<0)
        return -1;
    write(fd, "out", 3);// Set out direction
    close(fd);
    sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
    fd = open(buf, O_WRONLY);
    if(fd<0)
        return -1;
    // 向GPIO口写值
    sprintf(buf, "%d", value);
    write(fd, buf, strlen(buf));
    close(fd);
    return 0;
}
int Read_GPIO(int gpio, int *value){
    int fd;
    char val;
    char buf[MAX_BUF];
    sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
    fd = open(buf, O_RDONLY);
    if(fd<0)
        return -1;
    // 读取GPIO的值
    read(fd, &val, 1);
    *value = atoi(&val);
    close(fd);
    return 0;
}
