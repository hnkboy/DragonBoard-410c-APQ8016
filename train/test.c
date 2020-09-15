#include <stdio.h>
#include <string.h>

typedef unsigned int u32;
typedef unsigned long u64;

typedef u32 uword;

u32 g_test;
u32 g_test1 = 1;

typedef struct {
	char pad1;
	char pad2;
	int a;
	char pad[0];
}vec_header_t;


typedef struct __attribute__((packed)) {
	char pad1;
	char pad2;
	int a;

}vec_header2_t;




struct mystruct1    // 使用默认的4字节对齐（32位编译器）
{                   // 1字节对齐（无对齐）  4字节对齐
    int a;          // 4                  4
    char b;         // 1                  2(1+1)
    short c;        // 2                  2
};

//使用__attribute__((packed))取消对齐
struct mystruct11
{                   // 1字节对齐        4字节对齐
    int a;          // 4               4
    char b;         // 1               2(1+1)
    short c;        // 2               2
}__attribute__((packed));


uword
round_pow2 (uword x, uword pow2)
{
  return (x + pow2 - 1) & ~(pow2 - 1);
}



typedef struct  __attribute__ ((aligned(1))){

	//long pad[0];
    char    pad1[0];
    int a;
    char b;         // 1
}vec_s_t;

typedef struct __attribute__ ((packed)){

	//long pad[0];
    char    pad1[0];
    long a;
    char b;         // 1
}vec_s_t1;

struct stu{
char sex;
int length;
char name[10];
}__attribute__ ((aligned (1)));


/*class 1*/

struct body
{
    int num;
};

struct node1 g_node_data;

static struct body bodynum[10];

struct node1
{
    struct body *boynum_t;
};

/*sizeof长度为32*/
struct node2
{
    struct
    {
        u64 num1:1;
    }node2x;


    u64 num2:63;
    u64 num3:34;
    u64 num4:32;


};
/*sizeof长度为24*/

struct node3
{

    u64 num1:1;
    u64 num2:63;
    u64 num3:34;
    u64 num4:32;
};

struct A
{
    int a;
    short b;
    int c;
    char d;
};
struct B{
    int a ;
    int c;
    short b;
    char d;
};


int  main(void)
{

    int a =0;
	vec_header_t *p;
	p = malloc(sizeof(vec_header_t));
	printf("ptr = %d  %d\n",p,&p->pad[0]);
	printf("ptr = %d\n",p-1);

	printf("sizeof(struct mystruct1) = %d.\n", sizeof(struct mystruct1));    //默认对齐时是8字节
    printf("sizeof(struct mystruct11) = %d.\n", sizeof(struct mystruct11));  //取消对齐时是7字节


    if(a=1)
    {
    printf(" a1 \n");

    }

    if(a=0)
    {
    printf(" a2 \n");

    }
    printf("%d\n",round_pow2(3,4));

    printf("%d\n",sizeof(vec_s_t));
    printf("%d\n",sizeof(struct stu));
    {
        u32 first_bit;
        u32 bitmap = 0x74;  /*BIN: 111 0100*/

        first_bit = __builtin_clzll(bitmap);
        printf("first_bit %d\n",first_bit);
        first_bit = 64 - 1 - first_bit;
        printf("first_bit %d\n",first_bit);
    }
    {

        struct node1 *node_data = &g_node_data;
        struct body *boynum_t;

        /*把数组指针传递,要理解指针和指针带*的取得值不一样*/
        node_data->boynum_t = bodynum;

        boynum_t = (node_data->boynum_t + 1);

        boynum_t->num = 1;
        printf("%d\n",bodynum[1].num );


        printf("len: %d   \n",sizeof(struct node2));


    }

    {
    uword x ,pow2;
    x = 3;
    pow2 = 3;

    printf("x =%x, pow2= %d.\nround_pow2= %d\n",x ,pow2, round_pow2(x ,pow2));


    }


    printf("%d,%d \n",sizeof(struct A), sizeof(struct B));



	return 0;
}












