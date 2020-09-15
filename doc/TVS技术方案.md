# 需求分析

项目需求：

- 根据用户规则，将流量分流到指定的KVM，支持流量的steering，mirror。steering 串联设备，需要报文回注，mirror 并联设备，镜像给多个设备，报文不回注；
- 规则的下发通过gRPC和REST来实现，需要考虑集群。
- 需要对外部设备和内部KVM保活功能。





# 软件设计

## 整体方案

采用成熟的方案，收发包框架采用DPDK-vpp开源框架，控制平面采用ligatovpp-agent框架。

- vpp有acl功能，通过新加节点来实现流量的转发；
- ligato开源框架支持gRPC和REST，也需要增加插件来实现；
- vpp有abf机制，可以和KVM或外部设备keepalive；





> https://github.com/FDio/vpp
>
> https://github.com/ligato/vpp-agent

## vpp设计

DPDK作为流量输入，vpp根据匹配规则将流量通过vhost-user 发送给KVM。

- 由vpp创建vhost-socket，kvm启动后连接特定的 socket。 (qemu 需要v2.1版本)
- vpp通过dpdk-plugin加载指定网卡作为输入接口。



![image-20200820171155809](C:\Users\Jerry\AppData\Roaming\Typora\typora-user-images\image-20200820171155809.png)



### 内部组网结构

​       报文l2转发需要bridge，bridge连接dpdk接口和vhost-user接口，也就是外部输入端和kvm端口都连接到一个桥域上，配置桥虚接口提供三层业务需要。



![image-20200820180359652](C:\Users\Jerry\AppData\Roaming\Typora\typora-user-images\image-20200820180359652.png)

内部kvm和外部网络连接到一块，具备二层交换功能，可能会污染外部网络。

采用下图组网，设置两个桥域，内部kvm和外部输入二层隔离，但是kvm报文回注的时候需要走路由，影响报文回流。

需要 确认报文回注细节！





![image-20200820171101646](C:\Users\Jerry\AppData\Roaming\Typora\typora-user-images\image-20200820171101646.png)







### ifm功能（acl 单命中）

#### 新增命令行



配置输入端口组

set ifm-plugin input-group  [del]  <interface interface interface >

配置steering 策略 和mirror策越

set ifm-plugin policy  steer  acl <acl-index>  <interface> 

set ifm-plugin policy  mirror  acl <acl-index>  <interface interface interface>  

维护命令，用来显示下发表项

show ifm-plugin input-group 

show ifm-plugin steer [id]

show ifm-plugin mirror [id]



###### 典型命令行配置如下：

#配置acl返回分配的acl-index， api方式可以指定acl_index下发；

vpp#  set acl-plugin acl permit src 10.10.0.0/24 dst 10.10.1.0/24 sport 443

ACL index:0

#配置ifm, 输入端口组

vpp#  set ifm-plugin intput-group  VirtualEthernet0/0/1 VirtualEthernet0/0/2 VirtualEthernet0/0/3

#配置ifm mirror和steer 策略

vpp#  set ifm-plugin  policy mirror  acl 0 VirtualEthernet0/0/4  VirtualEthernet0/0/5  VirtualEthernet0/0/6

vpp#  set ifm-plugin  policy steer  acl 0 VirtualEthernet0/0/7 

#修改ifm mirror 策略  ？实现需要讨论

vpp#  set ifm-plugin  policy mirror  acl 0 VirtualEthernet0/0/4 

#删除ifm mirror和steer 策略策略

vpp#  set ifm-plugin  policy mirror acl 0 del

vpp#  set ifm-plugin  policy steer acl 0 del



**未命中策略执行动作 ？**



|                  | 命中steer策略   | 未命中steer策略 |
| :--------------: | --------------- | --------------- |
|  命中mirror策略  | mirror/steering | mirror/loopback |
| 未命中mirror策略 | /steering       | /loopback       |



#### 新增plugin 

vpp的业务有多个几点节点组成，新增业务模块名字为ifm，该业务依赖acl功能，需要一起定制；

ifm数据平面由二个node组成。目前只添加在二层路径上。

INPUT路径

- ifm-l2-classify  ： 匹配steering 报文；
- ifm-l2-fwd： 对匹配结果转发；



vpp节点图：



![image-20200820155402298](C:\Users\Jerry\AppData\Roaming\Typora\typora-user-images\image-20200820155402298.png)



medata，传递命中的acl索引。



#### **修改acl匹配**



现有vpp，ACl支持两种匹配方法

- 普通的遍历：报文遍历所有acl规则；
- HASH命中：报文和mask处理后生成key，hash查找；

现有的acl hash命中，需要和下发的所有的mask类型做不同的key，多次hash查找，返回最后一次有效结果。

需求中，要求acl实现多命中或acl最长掩码匹配。目前的vpp只按照下发顺序，生效最后下发的规则。

方案分析：

1.多命中：代码上能够修改实现多命中，匹配结果以vec的形式返回ifm，但是报文后续的处理逻辑复杂。

2.acl最长掩码匹配：只需要排序掩码类型，返回首次命中结果，修改较简单。（议此方案）



#### 配置规则生效

规则生效范围可以是接口，vlan，bridge。按照接口下发规则，则每个接口都需要维护一份规则数据。

vpp中规则下发是按照接口或bridge，vlan新版本才引入暂不止支持。

以防洪的功能举例说明，泛洪的实现，既能基于接口又能基于bridge

cli命令如下

```
set interface l2 flood                   set interface l2 flood <interface> [disable]
set bridge-domain flood                  set bridge-domain flood <bridge-domain-id> [disable]
```

该两条命令是分别下发到接口配置和桥配置，报文经过L2-INPUT节点会根据配置定制feature节点路径。



![image-20200820162426688](C:\Users\Jerry\AppData\Roaming\Typora\typora-user-images\image-20200820162426688.png)



ifm生效范围由set ifm-plugin input-group命令决定，

set ifm-plugin input-group  [del]  <interface interface interface >

该命令会是使能接口的ifm功能，报文会经过l2-fwd节点处理。



#### acl命中和规则查找

acl模块支持hash查找，key是五元组加lc_index。lc_index是注册索引，该索引是模块向acl注册时生成的，需要模块传递参数;

通过命令show acl-plugin lookup user 查看其它模块注册acl用到的参数

```
DBGvpp# show acl-plugin lookup user 
index 0:interface ACL:sw_if_index:is_input
index 1:ABF plugin:sw_if_index:(nil)
index 2:GBP ACL :src-epg:dst-epg
```

show命令可以看出来acl以接口和IN/OUT方向做lc_index。

lc_index如果注册时候以接口索引注册，该lc_index下存的ACE也只服务与该接口下；



##### ifm和acl的具体实现

acl模块使用的bi-hash，来做查找，但是没用bi-hash的冲突机制，bi-hash的冲突需要扩展整个page可能考虑性能没有用。acl自己维护冲突链只是利用了hash查找和桶，桶里存的是自己维护的冲突链下标。

acl配置是全局数据不是生效数据，当调用下发acl的时候，会生成生效数据(ACE)存到对应lc_index的线性表vec中，并维护冲突关系。

在报文匹配的时候，找到桶，拿到ACE对应的vec下标，根据lc_index遍历冲突链，最终找到ACE，ACE下标对应着ifm转发规则policy，能找到转发规则。

![image-20200820162729709](C:\Users\Jerry\AppData\Roaming\Typora\typora-user-images\image-20200820162729709.png)



### ifm功能（acl 多命中）



DPDK作为流量输入，vpp根据匹配规则将流量通过vhost-user 发送给KVM。

- 由vpp创建vhost-socket，kvm启动后连接特定的 socket。 (qemu 需要v2.1版本)
- vpp通过dpdk-plugin加载指定网卡作为输入接口。





![image-20200818102200697](C:\Users\Jerry\AppData\Roaming\Typora\typora-user-images\image-20200818102200697.png)



![image-20200818102609399](C:\Users\Jerry\AppData\Roaming\Typora\typora-user-images\image-20200818102609399.png)



#### 新增命令行

配置输入端口组

set ifm-plugin input-group  <interface interface interface >

配置策越

set ifm-plugin policy  [add|del]   acl <acl-index>  steer <interface>  [mirror <interface ... > ]

维护命令，用来显示下发表项

show ifm-plugin policy [acl <acl-index>]



##### 典型命令行配置：

#配置acl返回分配的acl-index， api方式可以指定acl_index下发；

vpp#  set acl-plugin acl permit src 10.10.0.0/24 dst 10.10.1.0/24 sport 443

ACL index:0

#配置ifm, 输入端口组

vpp#  set ifm-plugin intput-group  VirtualEthernet0/0/1 VirtualEthernet0/0/2 VirtualEthernet0/0/3

#配置ifm mirror和steer 策略

vpp#  set ifm-plugin policy  acl 0  steer  VirtualEthernet0/0/4  mirror  VirtualEthernet0/0/5  

#删除ifm mirror和steer 策略策略

vpp#  set ifm-plugin policy  del  acl 0



**未命中策略执行动作 ？**

直接翻转mac



需不需要 policy <id> ？需要修改的时候，是覆盖还是增删？





#### 新增plugin 

vpp的业务有多个几点节点组成，新增业务模块名字为ifm，该业务依赖acl功能，需要一起定制；

ifm数据平面由二个node组成。目前只添加在二层路径上。

INPUT路径

- ifm-l2-fwd： 执行匹配转发；



vpp节点图：

![image-20200820160605372](C:\Users\Jerry\AppData\Roaming\Typora\typora-user-images\image-20200820160605372.png)





在多命中方案中结果不确定metadata数量有限只有12字节可用（cache1），匹配加转发在一个节点完成；



#### **修改acl**

acl利用hash查找，会每种mask类型找到一个结果，遍历完返回最新的结果。如果支持多命中需要修改返回结果支持vec或数组的方式。

acl命中多个结果，对应的转发端口需要做去重处理吗？

主要修改函数 

multi_acl_match_get_applied_ace_index

acl_plugin_match_5tuple_inline





### BFD功能



IFM通过注册BFD接口事件，获取接口状态，并自己维护端口状态数据。

接口：bfd_register_listener()



**内部BFD（vpp与vhost方向）**

目的端口DOWN： mirror报文的不转发，steering的报文回环；普通报文回环；

目的端口UP：报文按照正常逻辑转发；



**外部BFD（VPP 通过DPDK接口与外部保活）**

端口DOWN:  不关心此场景；









#### *BFD介绍*

*BFD模块，支持异步模式和查询模式，echo报文功能。*



*BFD有以下节点组成：*

- *"bfd-process" ：节点类型(VLIB_NODE_TYPE_PROCESS)起新线程进行报文定时发送，和控制面数据处理。*

- *"bfd-udp-echo6-input"，"bfd-udp-echo4-input"，"bfd-udp4-input"，"bfd-udp6-input" ：*

  ​       *节点类型(VLIB_NODE_TYPE_INTERNAL)，处理收到的报文内容，更新bfd状态。*

*控制面数据由spinlock保证线程安全，bfd-process 该协程维护bfd状态。*







### 详细设计



文件划分



|              |                   |
| :----------: | ----------------- |
|    ifm.c     | 全局数据          |
| ifm_policy.c | policy命令处理    |
|  ifm_node.c  | 数据节点处理      |
|  ifm_api.c   | 二进制api后台实现 |
|  ifm_bfd.c   | bfd功能实现       |
|  ifm_test.c  | vpp_api_test测试  |













## 部署实现







# 资料背景

## vpp介绍

​    vpp是Vector Packet Processing的简称，VPP平台是一个可扩展的框架，可提供开箱即用的生产质量交换机/路由器功能。它是思科矢量数据包处理（VPP）技术的开源版本：一种高性能的数据包处理堆栈，可以在商用CPU上运行。

​    VPP是通过graph node串联起来形成一条datapath来处理报文，通过插件的形式引入新的graph node或者重新排列报文的graph node。VPP平台可以用于构建任何类型的报文处理应用。



![Custom Packet Processing Graph](C:\Users\Jerry\Desktop\图片\VPP_custom_application_packet_processing_graph.280.jpg)





选择vpp的理由，和ovs对比性能有优势

> https://wiki.fd.io/view/VPP/What_is_VPP%3F









