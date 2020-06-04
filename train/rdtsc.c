
#include <stdio.h>
#include <string.h>
#include <time.h>
/* Floating point types. */
typedef double f64;
typedef float f32;
typedef unsigned int u32;
typedef unsigned long u64;

//u64 clib_cpu_time_now (void);

#define __MACH__ 
#define CLIB_DEBUG 0

#if CLIB_DEBUG > 0
#define always_inline static inline
#define static_always_inline static inline
#else
#define always_inline static inline __attribute__ ((__always_inline__))
#define static_always_inline static inline __attribute__ ((__always_inline__))
#endif



/* Use 64bit floating point to represent time offset from epoch. */
always_inline f64
unix_time_now (void)
{
  struct timespec ts;
#ifdef __MACH__
  clock_gettime (CLOCK_REALTIME, &ts);
#else
  /* clock_gettime without indirect syscall uses GLIBC wrappers which
     we don't want.  Just the bare metal, please. */
  syscall (SYS_clock_gettime, CLOCK_REALTIME, &ts);
#endif
  return ts.tv_sec + 1e-9 * ts.tv_nsec;
} 
 
 
 
always_inline u64 clib_cpu_time_now (void)
{
  u32 a, d;
  asm volatile ("rdtsc":"=a" (a), "=d" (d));
  return (u64) a + ((u64) d << (u64) 32);
}

/*从pentium开始，很多80x86微处理器都引入TSC，一个用于时间戳计数器的64位的寄存器，它在每个时钟信号(CLK, CLK是微处理器中一条用于接收外部振荡器的时钟信号输入引线）到来时加一。
    通过它可以计算CPU的主频，比如：如果微处理器的主频是1MHZ的话，那么TSC就会在1秒内增加1000000。除了计算CPU的主频外，还可以通过TSC来测试微处理器其他处理单元的运算速度，资料[2]介绍了这个内容。
    那么如何获取TSC的值呢？rdtsc，一条读取TSC的指令，它把TSC的低32位存放在eax寄存器中，把TSC的高32位存放在edx中，更详细的描述见资料[1]。
    下面来看看rdtsc的具体用法，在linux源代码include/asm-i386/msr.h中，可以找到这么三个关于rdtsc的宏定义：*/
	
	
int  main(void)
{
	int i=0;
	u64 a =0;
	f64 f = 0;
	a = clib_cpu_time_now();
	f = unix_time_now();
	printf("SYS_clock_gettime consumer = %lu.\n", clib_cpu_time_now() - a);
	printf("time = %f.\n", f);   
	
	return 0;
}
