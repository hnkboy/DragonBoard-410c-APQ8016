#include <stdio.h>
#include <string.h>

typedef int *(funct1) (int * vm);
typedef int *(*funct2) (int * vm);


int (*p) (int x);  /*定义一个函数指针*/

funct1 *f1;
funct2 f2;

int * vlib_init_function(int *vm)
{



    return 0;
}
int vlib_init2_function(int vm)
{



    return 0;
}

int main()
{
    f1 = vlib_init_function;
    f2 = vlib_init_function;
    p = vlib_init2_function;

    return 0;
}


