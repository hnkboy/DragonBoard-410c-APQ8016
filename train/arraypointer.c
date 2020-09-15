#include <stdio.h>
#include <string.h>


int a[3][4];


char *strl[] = {"nihao",  "wohenhao"}; //定义了一个字符串指针数组变量

char **str = {"nihao",  "wohenhao"};  //用法错误

int main(int argc, char * * argv)
{
    int (*p)[4];  //定义数组指针
    int **q;      //只是定义了指向指针的指针，二级指针； 可以理解为指针数组的性质(一维的)
    p = a;
    q = a;
    printf("OX%p\n", a);
    printf("OX%p\n", a[0]);
    printf("OX%p\n", a[1]);

    printf("OX%p\n", p);
    printf("OX%p\n", p[0]);
    printf("OX%p\n", p[1]);

    printf("OX%p\n", q );
    printf("OX%p\n", q[0]);
    printf("OX%p\n", q[1]);



    printf("OX%p\n", str );
    printf("OX%p\n", str[0]);
    printf("OX%p\n", str[1]);

    #if 0
    printf("%s\n",str[0]);  //程序直接跑死，不能这样用 char **str，只是个变量不能存实体
    printf("%s\n",str[1]);
    #endif


    printf("OX%p\n", strl );
    printf("OX%p\n", strl[0]);
    printf("OX%p\n", strl[1]);

    printf("%s\n",strl[0]);
    printf("%s\n",strl[1]);

    return 0;
}



