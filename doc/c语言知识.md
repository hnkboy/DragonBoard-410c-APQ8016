# C语言



###### attribute

__attribute__关键字主要是用来在函数或数据声明中设置其属性。给函数赋给属性的主要目的在于让编译器进行优化。函数声明中的__attribute__((noreturn))，就是告诉编译器这个函数不会返回给调用者，以便编译器在优化时去掉不必要的函数返回代码。
GNU C的一大特色就是__attribute__机制。__attribute__可以设置函数属性（Function Attribute）、变量属性（Variable Attribute）和类型属性（Type Attribute）。

__attribute__((packed))  
告诉编译器取消结构在编译过程中的优化对齐

__attribute__((constructor))  

在函数调用之前调用

__attribute__ ((unused))

告诉编译器忽略此告警

[]: https://blog.csdn.net/baidu_37973494/article/details/84099543	"字节对齐"



###### setjmp/longjmp

C语言中有一个goto语句，其可以结合标号实现函数内部的任意跳转（通常情况下，很多人都建议不要使用goto语句，因为采用goto语句后，代码维护工作量加大）。另外，C语言标准中还提供一种非局部跳转“no-local goto"，其通过标准库<setjmp.h>中的两个标准函数setjmp和longjmp来实现。



###### 结构体指针减1加1

```
((vec_header_t *) (v) - 1)
```


减一加一，增量都是结构体的大小



###### 函数指针typedef

```
typedef int (*p) (int x);
typedef int (p) (int x);

int (*p) (int x);  /*定义一个函数指针*/
#函数指针
p q;
q = fun;

#函数
p *q;
q = fun;

#直接定义一个函数指针
p= fun

```

###### typedef和define的区别

#define 是在编译预处理时进行简单的替换，不做正确性检查。它是预处理指令

typedef 是在编译时处理的，它在自己的作用域内给一个已经存在的类型一个别名。 

typedef时关键字，#define时宏命令，所以define需要加#

\#define INTPTR1 int*

typedef int* INTPTR2;

INTPTR1 p1, p2;

INTPTR2 p3, p4;

最后结果，p3,p4都是指针



###### 数组指针和指针数组

以最后的词组做性质判定，指针数组就是数组。

数组指针（也称行指针），定义 `int (*p)[n];`如要将二维数组`int a[3][4];`赋给一指针`int (*p)[4];` ，应这样赋值：

p=a;       //p++;    该语句执行过后，也就是p=p+1;p跨过行a[0][]指向了行a[1][]



###### bss段，数据段，代码段

[]: http://blog.chinaunix.net/uid-27018250-id-3867588.html

`objdump -s -d a.out` 

堆、栈、静态存储区，



bss段：

BSS段（bsssegment）通常是指用来存放程序中未初始化的全局变量的一块内存区域。BSS是英文BlockStarted by Symbol的简称。BSS段属于静态内存分配。

data段：

数据段（datasegment）通常是指用来存放程序中已初始化的全局变量的一块内存区域。数据段属于静态内存分配。

text段：

代码段（codesegment/textsegment）通常是指用来存放程序执行代码的一块内存区域。这部分区域的大小在程序运行前就已经确定，并且内存区域通常属于只读,某些架构也允许代码段为可写，即允许修改程序。在代码段中，也有可能包含一些只读的常数变量，例如字符串常量等。

rodata段：

存放C中的字符串和#define定义的常量，在实际编译中个，<!--实验发现字符常量都放在了rodata里-->

heap堆：

堆是用于存放进程运行中被动态分配的内存段，它的大小并不固定，可动态扩张或缩减。当进程调用malloc等函数分配内存时，新分配的内存就被动态添加到堆上（堆被扩张）；当利用free等函数释放内存时，被释放的内存从堆中被剔除（堆被缩减）

stack栈：

是用户存放程序临时创建的局部变量，也就是说我们函数括弧“{}”中定义的变量（但不包括static声明的变量，static意味着在数据段中存放变量）。除此以外，在函数被调用时，其参数也会被压入发起调用的进程栈中，并且待到调用结束后，函数的返回值也会被存放回栈中。由于栈的先进先出特点，所以栈特别方便用来保存/恢复调用现场。从这个意义上讲，我们可以把堆栈看成一个寄存、交换临时数据的内存区

常量段：

常量段一般包含编译器产生的数据（与只读段包含用户定义的只读数据不同）。比如说由一个语句a=2+3编译器把2+3编译期就算出5，存成常量5在常量段中



###### malloc calloc realloc

[]: https://www.runoob.com/cprogramming/c-function-realloc.html



###### kb mb gb 换算

	  main_heap_size = size;
	
	  if (*sizep == 'g' || *sizep == 'G')
	    main_heap_size <<= 30;
	  else if (*sizep == 'm' || *sizep == 'M')
	    main_heap_size <<= 20;
2的32次方  = 4G；   一个Byte 是8个bit，1024个Byte等于1Kb，1024个kb等于1mb;