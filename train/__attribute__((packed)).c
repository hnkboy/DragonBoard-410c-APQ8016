#include <stdio.h>

typedef struct {
	char pad1;
	char pad2;
	int a;
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


int  main(void)
{
	vec_header_t *p=0x02;
	
	printf("ptr = %d\n",p);
	printf("ptr = %d\n",p-1);
	
	printf("sizeof(struct mystruct1) = %d.\n", sizeof(struct mystruct1));    //默认对齐时是8字节
    printf("sizeof(struct mystruct11) = %d.\n", sizeof(struct mystruct11));  //取消对齐时是7字节
	
	return 0;
}
