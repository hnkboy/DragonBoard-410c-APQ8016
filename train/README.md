# C语言



###### attribute

```
__attribute__关键字主要是用来在函数或数据声明中设置其属性。给函数赋给属性的主要目的在于让编译器进行优化。函数声明中的__attribute__((noreturn))，就是告诉编译器这个函数不会返回给调用者，以便编译器在优化时去掉不必要的函数返回代码。
GNU C的一大特色就是__attribute__机制。__attribute__可以设置函数属性（Function Attribute）、变量属性（Variable Attribute）和类型属性（Type Attribute）。


__attribute__((packed))  
告诉编译器取消结构在编译过程中的优化对齐

```

[]: https://blog.csdn.net/baidu_37973494/article/details/84099543	"字节对齐"



###### setjmp/longjmp

```
C语言中有一个goto语句，其可以结合标号实现函数内部的任意跳转（通常情况下，很多人都建议不要使用goto语句，因为采用goto语句后，代码维护工作量加大）。另外，C语言标准中还提供一种非局部跳转“no-local goto"，其通过标准库<setjmp.h>中的两个标准函数setjmp和longjmp来实现。
```



结构体指针减1加1

```
((vec_header_t *) (v) - 1)
减一加一，增量都是结构体的大小
```

