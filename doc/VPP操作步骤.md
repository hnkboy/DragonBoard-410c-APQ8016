# VPP的学习资料



#### VPP介绍

vpp软件架构

依次由 Infra，VLIB，VNET，Plugins层次组成

<img src="https://fd.io/docs/vpp/master/_images/VPP_Layering.png" alt="图片" style="zoom: 30%;" />





vpp的函数调用像各个节点之间相互链接，通过决定下一跳节点的路径在确定整个代码的执行路径



![技术分享图片](http://image.mamicode.com/info/201806/20180601122052629136.png)

<img src="https://fd.io/img/tech-02.png" alt="img" style="zoom: 50%;" />

![image-20200812165711931](C:\Users\Jerry\AppData\Roaming\Typora\typora-user-images\image-20200812165711931.png)

![image-20200813135901134](C:\Users\Jerry\AppData\Roaming\Typora\typora-user-images\image-20200813135901134.png)

#### VPP指南

##### vpp的安装

下载源码：git clone https://gerrit.fd.io/r/vpp

安装依赖：apt-get install -y python-cffi python-pycparser iproute iproute2 libc-ares2 libudev-dev numactl wget

[1]: https://blog.csdn.net/weixin_40815457/article/details/86476920	"安装参考链接"



##### vpp典型配置实验

###### 

###### vpp的启动命令

```
./vpp/build-root/install-vpp_debug-native/vpp/bin/vpp -c /etc/vpp/startup.conf 
```

/etc/vpp/startup.conf定制接口和插件



###### vppctl命令配置接口

```
vppctl --进入cli shell

create vhost socket /home/jerry/vhost1.sock server
set interface state VirtualEthernet0/0/0 up
create vhost socket /home/jerry/vhost2.sock server
set interface state VirtualEthernet0/0/1 up	
set interface l2 bridge VirtualEthernet0/0/0 1
set interface l2 bridge VirtualEthernet0/0/1 1

set interface state GigabitEthernet2/1/0 up
set interface l2 bridge GigabitEthernet2/1/0 1

create tap
set interface state tap0 up
set interface l2 bridge tap0 1
```

[命令行参考链接](https://wiki.fd.io/view/VPP/Command-line_Interface_(CLI)_Guide)

[命令行cli](https://docs.fd.io/vpp/19.08.2/d8/d37/clicmd_src_vnet_l2.html)

https://wiki.fd.io/view/VPP/SecurityGroups



###### qemu-kvm启动虚拟机连接sock

带命令启动

```
qemu-system-x86_64 \
    -enable-kvm -m 512 \
    -bios OVMF.fd \
    -smp 4 -cpu host \
    -vga none -nographic \
    -drive file="clear-33210-kvm.img",if=virtio,aio=threads \
    -chardev socket,id=char1,path=/home/jerry/vhost1.sock \
    -netdev type=vhost-user,id=mynet1,chardev=char1,vhostforce \
    -device virtio-net-pci,mac=00:00:00:00:00:01,netdev=mynet1 \
    -object memory-backend-file,id=mem,size=512M,mem-path=/dev/hugepages,share=on \
    -numa node,memdev=mem -mem-prealloc \
    -debugcon file:debug.log -global isa-debugcon.iobase=0x402
    
qemu-system-x86_64 \
    -enable-kvm -m 128 \
    -bios OVMF.fd \
    -smp 4 -cpu host \
    -vga none -nographic \
    -drive file="2-clear-33210-kvm.img",if=virtio,aio=threads \
    -chardev socket,id=char1,path=/home/jerry/vhost2.sock \
    -netdev type=vhost-user,id=mynet1,chardev=char1,vhostforce \
    -device virtio-net-pci,mac=00:00:00:00:00:02,netdev=mynet1 \
    -object memory-backend-file,id=mem,size=128M,mem-path=/dev/hugepages,share=on \
    -numa node,memdev=mem -mem-prealloc \
    -debugcon file:debug.log -global isa-debugcon.iobase=0x402
```

修改xml



virsh edit <domain-name>, 添加下面的字段，请修改正确的路径

    <interface type='vhostuser'>
      <mac address='52:54:00:3b:83:1b'/>
      <source type='unix' path='/home/test/vhost1.sock' mode='client'>
        <reconnect enabled='yes' timeout='10'/>
      </source>
      <model type='virtio'/>
      <driver name='vhost' rx_queue_size='256'/>
      <address type='pci' domain='0x0000' bus='0x00' slot='0x09' function='0x0'/>
    </interface>


###### 使用api配置vpp

```
#启动vat   ./vpp/build-root/build-vpp-native/vpp/bin/vpp_api_test

acl_add_replace permit, ipv4 permit
acl_add_replace   ipv4 deny src 192.168.41.128/32 
acl_add_replace 0  ipv4 deny src 192.168.41.127/32  dst 192.168.41.127/32   ipv4 deny src 192.168.41.127/32  dst 192.168.41.127/32                 

//作为修改使用
acl_interface_add_del  sw_if_index 1 add  input  acl 0
acl_interface_add_del  sw_if_index 1 del  input  acl 0  //删除使用
acl_interface_add_del  tap0 add  input  acl 0
```

[参考链接](https://blog.csdn.net/turbock/article/details/100731125)

[]: https://wiki.fd.io/view/VPP/SecurityGroups

##### 添加一个plugin

```
$ cd ./src/plugins
$ ../../extras/emacs/make-plugin.sh
<snip>
Loading /scratch/vpp-docs/extras/emacs/tunnel-c-skel.el (source)...
Loading /scratch/vpp-docs/extras/emacs/tunnel-decap-skel.el (source)...
Loading /scratch/vpp-docs/extras/emacs/tunnel-encap-skel.el (source)...
Loading /scratch/vpp-docs/extras/emacs/tunnel-h-skel.el (source)...
Loading /scratch/vpp-docs/extras/emacs/elog-4-int-skel.el (source)...
Loading /scratch/vpp-docs/extras/emacs/elog-4-int-track-skel.el (source)...
Loading /scratch/vpp-docs/extras/emacs/elog-enum-skel.el (source)...
Loading /scratch/vpp-docs/extras/emacs/elog-one-datum-skel.el (source)...
Plugin name: myplugin
Dispatch type [dual or qs]: dual
(Shell command succeeded with no output)

OK...
```

> https://blog.csdn.net/icebluechao/article/details/51208479

##### 维护命令

```
binary-api acl_add_replace  ipv4 deny src 192.168.41.128/32
show api clients
src/vlib/cli.c 有维护命令
show cli not-mp-safe
show cli mp-safe
show api message-table
show vlib graph ip4-vxlan-gpe-bypass

```

查看node节点命令

VirtualEthernet0/0/0

show interface VirtualEthernet0/0/0 features

set interface feature  VirtualEthernet0/0/0 jer3 arc device-input 

sudo vppctl xxx | grep xxx 可以快速查找一个特性的所有命令，例如可按如下方法察看GRE的所有命令

show interface addr 查看ip address

show interface [address|addr|features|feat] [<interface> [<interface> [..]]] [verbose]

clear trace

trace add virtio-input 100

show trace



##### vpp配置镜像口

```
set interface span <if-name> [disable | destination <if-name> [l2] [both|rx|tx]].
```

[]: https://docs.fd.io/vpp/17.07.01/clicmd_src_vnet_span.html	"参考cli"



##### 策略路由的实现

```
#abf  利用acl策略路由

#vpp上配置
#创建tap口并映射到内核
create tap id 0 host-ip4-addr 192.168.10.2/24 host-if-name lstack
set int state tap0 up
set int ip address tap0 192.168.10.3/24  //必须配置地址，为arp应答
create vhost socket /home/jerry/vhost1.sock server
set interface state VirtualEthernet0/0/0 up
create vhost socket /home/jerry/vhost2.sock server
set interface state VirtualEthernet0/0/1 up

set int l2 bridge VirtualEthernet0/0/0 1
set int l2 bridge VirtualEthernet0/0/1 1
#set int ip address VirtualEthernet0/0/0 192.168.57.1/24
#set int ip address VirtualEthernet0/0/1 192.168.57.2/24

#linux主机上配置

ip route add default dev lstack via 192.168.10.3

#配置网关
route add default gw 192.168.57.1

创建loop口设置IP，并挂接网桥, 设置为虚拟接口
loopback create
set int l2 bridge loop0 1 bvi  
set int ip address loop0 192.168.57.1/24
set int state loop0 up

#创建策略路由
acl_add_replace permit, ipv4 permit 
acl_add_replace 0  ipv4 permit dst 192.168.57.2/32      //作为修改使用

abf policy add id 1 acl 0 via 192.168.57.3
abf attach ip4 policy 1 tap0
```



##### 端口镜像

```
sw_interface_span_enable_disable src GigabitEthernet0/8/0 dst GigabitEthernet0/9/0
sw_interface_span_enable_disable src_sw_if_index 1 dst_sw_if_index 2
```

> https://blog.csdn.net/weixin_40815457/article/details/86523457



##### 配置bfd

linux OpenBFD配置

```
//运行bfd
//设置发送时长为1s
//设置接收时长为1s
//设置检测次数
//创建会话
bfdd-beacon --control=127.0.0.1:6666
bfdd-control --control=127.0.0.1:6666 session new set mintx 1 s
bfdd-control --control=127.0.0.1:6666 session new set minrx 1 s
bfdd-control --control=127.0.0.1:6666 session new set multi 3
bfdd-control --control=127.0.0.1:6666 connect local 192.168.12.2 remote 192.168.12.3

设置状态
bfdd-control --control=127.0.0.1:6666 session all state up
查看状态
bfdd-control --control=127.0.0.1:6666 status
```

vpp配置部分

```
create tap id 2 host-ip4-addr 192.168.12.2/24 host-if-name lstack
set interface ip address tap2 192.168.12.3/24
set interface state tap2 up
bfd udp session add interface tap2 local-addr 192.168.12.3 peer-addr 192.168.12.2 desired-min-tx 1000000 required-min-rx 1000000 detect-mult 3

#设置状态
bfd udp session set-flags interface tap2 local-addr 192.168.12.3 peer-addr 192.168.12.2 admin up
```

##### 配置端口dhcp

```
添加：
set dhcp client intfc GigabitEthernet2/1/0

查看：
show dhcp client intfc GigabitEthernet2/1/0

删除：
set dhcp client del intfc GigabitEthernet2/1/0
```

##### 配置端口access和trunk



create trunk ports

```
create vhost socket /var/run/vpp/sock0.sock server
create vhost socket /var/run/vpp/sock1.sock server
set interface state VirtualEthernet0/0/0 up
set interface state VirtualEthernet0/0/1 up
create sub VirtualEthernet0/0/0 200
create sub VirtualEthernet0/0/1 200
set interface state VirtualEthernet0/0/0.200 up
set interface state VirtualEthernet0/0/1.200 up
set interface l2 bridge VirtualEthernet0/0/0.200 200
set interface l2 bridge VirtualEthernet0/0/1.200 200
```

create access ports

```
create vhost socket /var/run/vpp/sock2.sock server
set interface state VirtualEthernet0/0/2 up
set interface l2 bridge VirtualEthernet0/0/2 200
set interface l2 tag-rewrite VirtualEthernet0/0/2 push dot1q 200
```

##### 配置classify

单个table中配置多条session
example：对源IP地址为30.1.1.100、30.1.1.101、30.1.1.102以及30.1.1.103的数据包进行过滤。

\# classify table mask l3 ip4 src buckets 16
\# classify session acl-hit-next deny opaque-index 0 table-index 0 match l3 ip4 src 30.1.1.100
\# classify session acl-hit-next deny opaque-index 1 table-index 0 match l3 ip4 src 30.1.1.101
\# classify session acl-hit-next deny opaque-index 2 table-index 0 match l3 ip4 src 30.1.1.102
\# classify session acl-hit-next deny opaque-index 3 table-index 0 match l3 ip4 src 30.1.1.103
\# set int input acl intfc GigabitEthernet13/0/0 ip4-table 0

使用多个表
example：使用多个表过滤源IP地址和源MAC地址，即将参数next-table指向上一个表（要用到的表）

\# classify table mask l3 ip4 src
\# classify session acl-hit-next deny opaque-index 0 table-index 0 match l3 ip4 src 30.1.1.100
\# classify table mask l2 src next-table 0
\# classify session acl-hit-next deny opaque-index 1 table-index 1 match l2 src 00:0c:29:e9:0e:9c
\# set int input acl intfc GigabitEthernet13/0/0 ip4-table 1





















https://wiki.fd.io/view/VPP/Interconnecting_vRouters_with_VPP



与python交互

https://www.jianshu.com/p/16ea79641475?utm_source=oschina-app

https://wiki.fd.io/view/VPP/Python_API

![建筑.png](https://wiki.fd.io/images/thumb/3/37/Architecture.png/600px-Architecture.png)

![img](https://img2018.cnblogs.com/i-beta/1414775/202002/1414775-20200223163038584-2062684261.png)





![img](https://img2018.cnblogs.com/i-beta/1414775/202002/1414775-20200223163055158-1355967417.png)



![在这里插入图片描述](https://img-blog.csdnimg.cn/20190131115923801.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dlbmd6aGlrdWkxOTky,size_16,color_FFFFFF,t_70)

![image-20200812165658381](C:\Users\Jerry\AppData\Roaming\Typora\typora-user-images\image-20200812165658381.png)



## vpp-agent

[vpp-agent](https://github.com/ligato/vpp-agent)







## DPDK

下载源码，

解压后进入运行 ./usertools/dpdk-setup.sh, 先选择编译器编译，在安装驱动，然后绑定对应网卡

https://blog.csdn.net/shhchen/article/details/88554777

https://www.jianshu.com/p/4b1d3ac3a389



IGB UIO

## 虚拟化



### libvirt

virsh xml文档

https://libvirt.org/formatdomain.html



## VPP学习计划

set interface l2 rewrite  

vpp官方学习文档

> https://wiki.fd.io/view/VPP#Documents

vpp性能分析

> https://docs.fd.io/csit/rls1908_1/report/vpp_performance_tests/packet_throughput_graphs/vm_vhost-2n-skx-xxv710.html#b-2t1c-vhost-base-i40e-vpp
>
> https://blog.csdn.net/icebluechao/article/details/51208479

软件架构

> https://wiki.fd.io/view/VPP/Software_Architecture

开发者文档

[1]: https://fd.io/docs/vpp/master/gettingstarted/developers/gdb_examples.html	"开发者文档，包括build gdb，和各模块代码介绍"

[2]: https://blog.csdn.net/wangcg123/article/details/71682145	"命令汇总"

[3]: https://www.jianshu.com/p/524b2c7627d8	"vpp Makefile解读"
[4]: https://blog.csdn.net/jqh9804/article/details/55100994	"VPP Bihash分析"

[5]: https://blog.csdn.net/turbock/article/details/100731125	"CLASSIFY 和ACL命令"



[6]: https://fd.io/vppproject/vppfeatures/	"vpp特性介绍"
[7]: https://docs.fd.io/vpp/20.05/	"vpp官方文档"

测试框架

[]: https://docs.fd.io/vpp/17.01/vpp_make_test/html/



参考博客

[]: https://yq.aliyun.com/articles/610476?spm=a2c4e.11155435.0.0.5db8421eE5rhqm	"vpp的多线程"



[]: https://turbock79.cn/?p=1563	"路由的创建"



vpp的安装过程

> https://fd.io/docs/vpp/master/gettingstarted/developers/building.html#build-vpp-debug



DPDK版本下载，

进入/usertools/dpdk_setup.sh安装

http://core.dpdk.org/download/

kvm和vpp

https://code.umbriel.fr/Nihl/galqv





-lvlibmemoryclient -lsvm -lvppinfra -lvatplugin -lpthread -lm -lrt -ldl -lcrypto -I/usr/include/vpp_plugins

怎么创建一个控制平面

https://wiki.fd.io/view/VPP/How_to_Create_a_VPP_binary_control-plane_API

vpp其他功能资料

https://vpp.flirble.org/master/d4/d09/vapi_doc.html



agent-vpp  ligato

https://docs.ligato.io/en/latest/intro/overview/



acl崩溃的问题，vpp论坛地址

https://lists.fd.io/g/vpp-dev/topic/crash_in_acl_code_while/75527176?p=,,,20,0,0,0::recentpostdate%2Fsticky,,,20,2,20,75527176#



