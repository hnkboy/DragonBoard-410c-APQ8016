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

按照x字节对齐；

aligned特性：aligned属性只能增加对齐字节数，不能减少到比默认对齐字节数还小。

aligned支持的最大对齐字节数由linker决定。

`__attribute__ ((weak))`

弱函数声明

    int  __attribute__((weak))  func(......)
    {
    	return 0;
    }

`__attribute__ ((optimize ("O3")))`

在全局已经使能O1， O2， O3， Os的情况下，某个单独的函数我们不想做任何的优化，可以用`__attribute__((optimize("O0")))`来修饰这个函数，比如我们把上述用O2可以编译过的代码进行如下修改：



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
typedef int (*p) (int x);  #1
typedef int (p) (int x);   #2

int (*p) (int x);  /*定义一个函数指针*/
#函数指针  #1
p q;
q = fun;

#函数     #2
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

`objdump -h a.out`

堆、栈、静态存储区，



1 **.rodata段：**存放只读数据，比如printf语句中的格式字符串和开关语句的跳转表。也就是你所说的常量区。

> 例如，全局作用域中的 const int ival = 10，ival存放在.rodata段
> 再如，函数局部作用域中的printf("Hello world %d\n", c);语句中的格式字符串"Hello world %d\n"，也存放在.rodata段



2 **.text段：**存放已编译程序的机器代码。

**注意：程序加载运行时，.rodata段和.text段通常合并到一个Segment（Text Segment）中，操作系统将这个Segment的页面只读保护起来，防止意外的改写。**

3 **.data段：**存放已初始化的全局变量。而局部变量在运行时保存在栈中，既不出现在.data段，也不出现在.bss段中。就是你所说的全局区。

> 例如：全局作用域中的int ival = 10，static int a = 30，以及局部作用域中的static int b = 30，这3个变量均存放在.data段中。注意，局部作用域中的static变量的生命周期和其余栈变量的生命周期是不同的。


4 **.bss段：**存放未初始化的全局变量。<u>在目标文件中这个段不占据实际的空间</u>，它仅仅是一个占位符，在加载时这个段用0填充。目标文件区分初始化和未初始化变量是为了空间效率：在目标文件中，未初始化变量不需要占据任何实际的磁盘空间。全局变量如果不初始化则初值为0，同理可以推断，static变量（不管是函数里的还是函数外的）如果不初始化则初值也是0，也分配在.bss段。

> 例如，全局作用域中的int ival; ival显然存放在.bss段

**



5. **栈：**函数的参数和局部变量是分配在栈上（但不包括static声明的变量）。在函数被调用时，栈用来传递参数和返回值。由于栈的后进先出特点，所以栈特别方便用来保存/恢复调用现场。

6. **堆：**用于存放进程运行中被动态分配的内存段，它的大小并不固定，可动态扩张或缩减。当进程调用malloc/free等函数分配内存时，新分配的内存就被动态添加到堆上（堆被扩张）/释放的内存从堆中被剔除（堆被缩减）





```
objdump -t exam1_elf | grep "\.bss"
```


可以看到，全局变量未初始化的，**和全局变量初始化为0的都在里面在bss段**，初始化其他值的在.data



```
root@ubuntu:~/train# objdump -h a.out 

a.out:     file format elf64-x86-64

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .interp       0000001c  0000000000000238  0000000000000238  00000238  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  1 .note.ABI-tag 00000020  0000000000000254  0000000000000254  00000254  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  2 .note.gnu.build-id 00000024  0000000000000274  0000000000000274  00000274  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  3 .gnu.hash     0000001c  0000000000000298  0000000000000298  00000298  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  4 .dynsym       000000d8  00000000000002b8  00000000000002b8  000002b8  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  5 .dynstr       00000090  0000000000000390  0000000000000390  00000390  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  6 .gnu.version  00000012  0000000000000420  0000000000000420  00000420  2**1
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  7 .gnu.version_r 00000020  0000000000000438  0000000000000438  00000438  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  8 .rela.dyn     000000c0  0000000000000458  0000000000000458  00000458  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  9 .rela.plt     00000048  0000000000000518  0000000000000518  00000518  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 10 .init         00000017  0000000000000560  0000000000000560  00000560  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 11 .plt          00000040  0000000000000580  0000000000000580  00000580  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 12 .plt.got      00000008  00000000000005c0  00000000000005c0  000005c0  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 13 .text         000003a2  00000000000005d0  00000000000005d0  000005d0  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 14 .fini         00000009  0000000000000974  0000000000000974  00000974  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 15 .rodata       000000b9  0000000000000980  0000000000000980  00000980  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 16 .eh_frame_hdr 00000044  0000000000000a3c  0000000000000a3c  00000a3c  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 17 .eh_frame     00000128  0000000000000a80  0000000000000a80  00000a80  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 18 .init_array   00000008  0000000000200da8  0000000000200da8  00000da8  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 19 .fini_array   00000008  0000000000200db0  0000000000200db0  00000db0  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 20 .dynamic      000001f0  0000000000200db8  0000000000200db8  00000db8  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 21 .got          00000058  0000000000200fa8  0000000000200fa8  00000fa8  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 22 .data         00000014  0000000000201000  0000000000201000  00001000  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 23 .bss          00000058  0000000000201020  0000000000201020  00001014  2**5
                  ALLOC
 24 .comment      00000029  0000000000000000  0000000000000000  00001014  2**0
                  CONTENTS, READONLY
```



首先看a.out

file format elf64-x86-64:文件所在平台属性，这里是64位x86处理器平台

Sections:

Size: 段的大小，字节为单位

VMA: 段在虚拟地址中的位置

LMA：段在加载地址中的位置

File off： File offset，段的起始位置

Algn：字节对齐方式，2**2表示2的平方即为4，2**3表示2的3次方即为8

这里面重要的有

.text（代码段）

.data（数据段）

.bss

```
root@ubuntu:~/train# readelf -h a.out 
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              DYN (Shared object file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x5d0
  Start of program headers:          64 (bytes into file)
  Start of section headers:          6696 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         9
  Size of section headers:           64 (bytes)
  Number of section headers:         29
  Section header string table index: 28
```

 Entry point address:               0x5d0就是text段了

















![在这里插入图片描述](https://www.freesion.com/images/926/76de31769ed28fe2e7e3e72ce595ea16.png)





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



介绍一下char str[]="hello"与char *str="hello"的区别

今天做题的时候遇到一个问题，如何实现一个函数返回一个指针，返回一个整型值很简单，因为“C语言是按值传递的”，而想要返回一个指针，比如char型的指针，然后想在主调函数中使用被调函数的返回指针，分析了一下关于char str[]和char *str的区别。

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



###### 原子操作

__atomic_exchange_n     原子替换`obj`指向的值`desired`并返回`obj`之前保存的值。操作是读取 - 修改 - 写入操作。C atomic_exchange                      （volatile A * obj，需要C语言）;