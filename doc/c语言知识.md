# C语言



###### __attribute

`__attribute__`关键字主要是用来在函数或数据声明中设置其属性。给函数赋给属性的主要目的在于让编译器进行优化。函数声明中的__attribute__((noreturn))，就是告诉编译器这个函数不会返回给调用者，以便编译器在优化时去掉不必要的函数返回代码。
GNU C的一大特色就是__attribute__机制。__attribute__可以设置函数属性（Function Attribute）、变量属性（Variable Attribute）和类型属性（Type Attribute）。

`__attribute__((packed))`  
告诉编译器取消结构在编译过程中的优化对齐

`__attribute__((constructor))`  

在函数调用之前调用

`__attribute__ ((unused))`

告诉编译器忽略此告警

`__attribute__ ((section()))`

告诉编译时把函数放到对应的数据段

`__attribute__ ((aligned(x)))`

让所作用的结构成员对齐在n字节自然边界上。如果结构中有成员的长度大于n，则按照最大成员的长度来对齐

aligned特性：aligned属性只能增加对齐字节数，不能减少到比默认对齐字节数还小。

aligned支持的最大对齐字节数由linker决定。

`__attribute__ ((weak))`

弱函数声明

    int  __attribute__((weak))  func(......)
    {
    	return 0;
    }









__attribute__ ((aligned(x)))作用域

	typedef struct {
	char  __attribute__ ((aligned(8))) pad1[0];
	char b;         // 1
	}vec_s_t;
	
	这样也是生效的，去掉__attribute__，结构体为1，不去为8；   其实是声明保证了最小不能超过8了






[]: https://blog.csdn.net/baidu_37973494/article/details/84099543	"字节对齐"

[]: https://www.jianshu.com/p/29eb7b5c8b2d	"attribute 总结"



###### __builtin

1.__builtin_parity(unsigned int x)

统计一个数二进制表示中1的个数是偶数还是奇数

2.__builtin_popcount(unsigned int x)

统计一个数的二进制表示中1的个数

3.__builtin_ffs(unsigned int x)

找出一个数的二进制表示中从末尾开始到遇见第一个1的的位置

4.__builtin_clz(unsigned int x)

返回一个数二进制表示中前导零的数目

5.__builtin_ctz(unsigned int x)

返回一个数二进制表示中尾零的数目









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

BSS段（bsssegment）通常是指用来存放程序中未初始化的全局变量的一块内存区域。BSS是英文BlockStarted by Symbol的简称。BSS段属于静态内存分配。  特点是:可读写的，在程序执行之前BSS段会自动清0。所以，未初始的全局变量在程序执行之前已经成0了。

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





通常，栈分配比堆分配更快，，因为它真正要做的只是移动堆栈指针。 使用内存池，您可以从堆分配中获得可比的性能，但这会带来一点点复杂性和麻烦。



![img](https://img-blog.csdn.net/20140904215636015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmd6aGVianV0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

heap：是由malloc之类函数分配的空间所在地。地址是由低向高增长的。 
stack：是自动分配变量，以及函数调用的时候所使用的一些空间。地址是由高向低减少的。



![在这里插入图片描述](https://img-blog.csdnimg.cn/2019091713383668.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poZW5nbmlhbmxp,size_16,color_FFFFFF,t_70)







###### malloc calloc realloc

[]: https://www.runoob.com/cprogramming/c-function-realloc.html



###### kb mb gb 换算

	  main_heap_size = size;
	
	  if (*sizep == 'g' || *sizep == 'G')
	    main_heap_size <<= 30;
	  else if (*sizep == 'm' || *sizep == 'M')
	    main_heap_size <<= 20;
2的32次方  = 4G；   一个Byte 是8个bit，1024个Byte等于1Kb，1024个kb等于1mb;



打印函数名字和行号



用法：可以用于程序错误时打印错误位置

```
printf("行号：%d 函数名：%s \n", __LINE__, __FUNCTION__);
```



###### if 等于和双等于

单等于，赋值后`if`会判断赋值后的变量，等于`if(a)`

    if(a=1)
    {
    printf(" a1 \n");
    }
    if(a==0)
    {
    printf(" a2 \n");
    }


###### mprotect()

在Linux中，mprotect()函数可以用来修改一段指定内存区域的保护属性。
函数原型如下：

```
#include <unistd.h>
#include <sys/mmap.h>
int mprotect(const void *start, size_t len, int prot);
```


mprotect()函数把自start开始的、长度为len的内存区的保护属性修改为prot指定的值。

prot可以取以下几个值，并且可以用“|”将几个属性合起来使用：





###### return的知识点

return是需要释放局部变量的栈内存地址的，return的值是怎么传递出去的呢？是利用临时变量，会申请临时变量。



被调函数运行结束后才会返回主调函数，但是被调函数运行结束后系统为被调函数中的局部变量分配的内存空间就会被释放。也就是说，return 返回的那个值在被调函数运行一结束就被释放掉了，那么它是怎么返回给主调函数的呢？
　　事实上在执行 return 语句时系统是在内部自动创建了一个临时变量，然后将 return 要返回的那个值赋给这个临时变量。所以当被调函数运行结束后 return 后面的返回值真的就被释放掉了，最后是通过这个临时变量将值返回给主调函数的。而且定义函数时指定的返回值类型实际上指定的就是这个临时变量的类型。
　　这也是为什么当 return 语句中表达式的类型和函数返回值类型不一致时，将 return 的类型转换成函数返回值类型的原因。return 语句实际上就是将其后的值赋给临时变量，所以它要以临时变量的类型为准，即函数返回值的类型。
　　注意：
　　1、C/C++里返回值是复制出去的，而对于大的对象，复制的代价很高。
　　 2、有些对象是不能复制的，至少编译器不知道怎么复制，比如数组




###### char *str与char str[]的区别

`char str[]`表达式表示的是在动态变量区中开辟一个能连续放6个字符的数组，数组名称是str.而赋值运算符右边是一个字符串常量，这个字符串常量是存放在常量区的，这个表达式的意思就是将“hello”这个字符串常量拷贝到刚才开辟的数组中。C语言规定，表达式如果是一个数组名，则代表的意思是该数组的起始地址，如果这个数组在一个函数中定义，如果以数组名返回时，因为数组在函数中定义，是个局部变量，函数返回之后，这个数组所占用的空间就被释放掉了，数组也被破坏掉了，因此返回的数组名也就没有意义，不能被其主调函数使用了。



`char *str`这个表达式的意思是在动态变量区中开辟一个存放指针的存储单元，指针变量名是str,"hello"同样也是一个字符串常量，存储在常量区，在程序的运行过程中一直存在。把字符串“hello”的地址值拷贝到刚才的存储单元中，即指针变量str的初值是字符串“hello”的地址。这时如果char *str="hello"定义在一个函数中并且以return str返回，因为str是一个变量名，返回的仅仅是str的值，所以在其他函数中可以使用该值，照样能够访问到“hello



[1]: https://blog.csdn.net/qq_40946787/article/details/79154671



###### __builtin_prefetch

__builtin_prefetch() 是 gcc 的一个内置函数。它通过对数据手工预取的方法，减少了读取延迟，从而提高了性能，但该函数也需要 CPU 的支持。

该函数的原型为：

```
void` `__builtin_prefetch (``const` `void` `*addr, ...)
```



其中参数 addr 是个内存指针，它指向要预取的数据，我们人工需要判定这些数据是很快能访问到的，或者说是它们就在最近的内存中 --- 一般来说，对于链表而言，各个节点在内存中基本上是紧挨着的，所以我们容易预取链表节点里的指针项。

该函数还有两个可选参数，rw 和 locality 。

rw 是个编译时的常数，或 1 或 0 。1 时表示写(w)，0 时表示读(r) 。

locality 必须是编译时的常数，也称为“时间局部性”(temporal locality) 。时间局部性是指，如果程序中某一条指令一旦执行，则不久之后该指令可能再被执行；如果某数据被访问，则不久之后该数据会被再次访问。该值的范围在 0 - 3 之间。为 0 时表示，它没有时间局部性，也就是说，要访问的数据或地址被访问之后的不长的时间里不会再被访问；为 3 时表示，被访问的数据或地址具有高 时间局部性，也就是说，在被访问不久之后非常有可能再次访问；对于值 1 和 2，则分别表示具有低 时间局部性 和中等 时间局部性。该值默认为 3 。

用法：

```
for (i = 0; i < n; i++)
            {
              a[i] = a[i] + b[i];
              __builtin_prefetch (&a[i+j], 1, 1);
              __builtin_prefetch (&b[i+j], 0, 1);
              /* ... */
            }
```



另外，参数 addr 是需要用户保证地址的正确，函数不会对不正确的地址做出错误的提示。

在 linux 内核中，经常会使用到这种预抓取技术。通常是通过宏和包装器函数使用预抓取。下面是一个辅助函数示例，它使用内置函数的包装器（见 ./linux/include/linux/prefetch.h）。这个函数为流操作实现预抓取机制。使用这个函数通常可以减少缓存缺失和停顿，从而 提高性能。

```
#ifndef ARCH_HAS_PREFETCH
#define prefetch(x) __builtin_prefetch(x)
#endif
 
static inline void prefetch_range(void *addr, size_t len)
{
#ifdef ARCH_HAS_PREFETCH
    char *cp;
    char *end = addr + len;
 
    for (cp = addr; cp < end; cp += PREFETCH_STRIDE)
        prefetch(cp);
#endif
}
```

###### __builtin_expect

在很多源码如Linux内核、Glib等,我们都能看到likely()和unlikely()这两个宏,通常这两个宏定义是下面这样的形式。

```
#define likely(x)      __builtin_expect(!!(x), 1)
#define unlikely(x)    __builtin_expect(!!(x), 0)
```

可以看出这2个宏都是使用函数 *__builtin_expect*()实现的, *__builtin_expect*()函数是GCC的一个内建函数(build-in function).

###### __thread 

 `__thread` 是GCC内置的线程局部存储设施,__thread变量每一个线程有一份独立实体，各个线程的值互不干扰。可以用来修饰那些带有全局性且值可能变，但是各线程独立不干扰的变量；

可以认为对每个 pthread_key, 库内部提供了一个 __thread void* 接受 pthread_setspecific 设置的指针，从而可以指向 class 类型



###### __sync_synchronize



我们在做多线程编程的时候，会涉及到一个称为memory order的问题。

例如

int x(0),y(0);
x=4;
y=3;

请问，实际执行的时候，这两条赋值语句谁先执行，谁后执行？ 会不会有某个时间点，在某个CPU看来，y比x大？

答案很复杂。本文的目的是从非常实践的角度来考虑这个问题。

首先，它分为两个层面。在编译器看来，x和y是两个没有关联的变量，那么编译器有权利调整这两行代码的执行顺序，只要它乐意。

其次，CPU也有权利这么做。

如果我非要严格要求顺序，那么就应该插入一个memory barrier

int x(0),y(0);
x=4;
在此插入memory barrier指令
y=3;

下面要论述，中间那行怎么写。请耐心看下去，因为大多数人都在瞎整。



###### 结构体数组指针减

struct A[N]； 

 (&A[1]  - &A[0])结果为1；

