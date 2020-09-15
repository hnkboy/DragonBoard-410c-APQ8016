## VPP_MAIN线程与VPP_WORK线程交互机制





vpp的文件结构

------



## 二进制文件



src/vlibapi/api_shared.c 该文件是，用于vpp后台数据接收通信





vl_msg_api_set_handlers  注册非线程安全





vxlan_api_hookup api的钩子函数



## VPP节点调度机制



vlib_main_or_worker_loop