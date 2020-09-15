#include <stdio.h>
#include <string.h>

struct _s_application_init {
    int(*function)(void);
};

struct _s_application_init _init_start;//段".application_init"的起始地址，在*.lds文件中定义
struct _s_application_init _init_end;//段".application_init"的末尾地址，在*.lds文件中定义
#define __app_init_section __attribute__((section(".application_init")))
#define __application_init(function) \
    struct _s_application_init _s_a_init_##function  __app_init_section = {function}


static int application_init_a(void)
{
    printf("execute funtion : %s\n", __FUNCTION__);
    return 0;
}
__application_init(application_init_a);

static int application_init_b(void)
{
    printf("execute funtion : %s\n", __FUNCTION__);
    return 0;
}
__application_init(application_init_b);

static int application_init_c(void)
{
    printf("execute funtion : %s\n", __FUNCTION__);
    return 0;
}
__application_init(application_init_c);

int main(int argc, char **argv)
{
    /*
     * 从段的起始地址开始获取数据，直到末尾地址
     */
    struct _s_application_init *pf_init = &_init_start;
    do {
        printf("Load init function from address %p\n", pf_init);
        pf_init->function();
        ++pf_init;
    } while (pf_init < &_init_end);
    return 0;
}


#ifdef ___TEXT
/*
seven@root:~/section/$ ld --verbose > main.lds
seven@root:~/section/$ cat main.lds
然后我们修改这个文件：

首先我们需要将默认文件的首尾“==================================================”包含这一行要删除，不然会报格式错误

/usr/bin/ld:test.lds:1: syntax error
collect2: error: ld returned 1 exit status

然后选择在“__bss_start”前添加我们自己的段

  ...

  . = .;

  _init_start = .;      /* 获取当前的地址赋值给__init_start,在源码中有使用到，指向“.application_init”段的起始地址 */
  //.application_init  : { *(.application_init) }/* 将“.application_init”的所有内容放在这一段 */
  //_init_end = .;        /* 获取当前的地址赋值给__init_end,表示“.application_init”段的结束地址

  //__bss_start = .;



  //seven@root:~/section/$ gcc main.c -Tmain.lds

  //*/

#endif

