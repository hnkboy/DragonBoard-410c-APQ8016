





# consume.c
#生产者消费者模型  
#编译文件时  gcc consume.c -pthread  

# signaltest.c 
#如果输入a，子线程打印"hello"，主程序继续等待输入；  
#如果输入q，主程序等待子程序结束。子线程打印"I will sleep 2 second and exit"，并延时两秒后结束。主线程随之打印"finish"，程序结束；  
