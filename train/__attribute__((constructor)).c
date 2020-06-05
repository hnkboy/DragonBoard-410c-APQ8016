#include<stdio.h>

__attribute__((constructor(102))) void before2_main() {
   printf("before2 main\n");
}
__attribute__((constructor(101))) void before_main() {
   printf("before main\n");
}
__attribute__((constructor)) void before1_main() {
   printf("before1 main\n");
}
__attribute__((destructor)) void after_main() {
   printf("after main\n");
}



int main(int argc, char **argv) {
   printf("in main\n");
   return 0;
}


/*
今天写一个动态库，需要让动态库有一个类似于windows的DLLMain函数一样功能的函数，可惜发现Linux没有这样的功能，于是查阅了大量的资料，最后发现GCC的__attribute__属性设置可以将函数设置成类似于这样功能的函数：

__attribute__((constructor)) // 在main函数被调用之前调用
__attribute__((destructor)) // 在main函数被调用之后调

这个例子的输出结果将会是：
before main
in main
after main

*/