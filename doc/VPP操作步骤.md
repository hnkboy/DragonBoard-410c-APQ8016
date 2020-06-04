# VPP的学习资料



#### VPP介绍

vpp软件架构

依次由 Infra，VLIB，VNET，Plugins层次组成

<img src="https://fd.io/docs/vpp/master/_images/VPP_Layering.png" alt="图片" style="zoom: 30%;" />





vpp的函数调用像各个节点之间相互链接，通过决定下一跳节点的路径在确定整个代码的执行路径



![技术分享图片](http://image.mamicode.com/info/201806/20180601122052629136.png)

<img src="https://fd.io/img/tech-02.png" alt="img" style="zoom: 50%;" />

#### VPP实践

##### vpp的安装

下载源码：git clone https://gerrit.fd.io/r/vpp

安装依赖：apt-get install -y python-cffi python-pycparser iproute iproute2 libc-ares2 libudev-dev numactl wget

[1]: https://blog.csdn.net/weixin_40815457/article/details/86476920	"安装参考链接"



##### vpp典型组网配置实验

###### 配置典型组网



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

###### qemu-kvm启动虚拟机

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



###### vpp配置镜像口

```
set interface span <if-name> [disable | destination <if-name> [l2] [both|rx|tx]].
```

[]: https://docs.fd.io/vpp/17.07.01/clicmd_src_vnet_span.html	"参考cli"



##### 使用api配置vpp

```
#启动vat   ./vpp/build-root/build-vpp-native/vpp/bin/vpp_api_test

acl_add_replace permit, ipv4 permit
acl_add_replace   ipv4 deny src 192.168.41.128/32  
acl_add_replace 0  ipv4 deny src 192.168.41.127/32      //作为修改使用
acl_interface_add_del  sw_if_index 1 add  input  acl 0
acl_interface_add_del  sw_if_index 1 del  input  acl 0  //删除使用


```

[参考链接](https://blog.csdn.net/turbock/article/details/100731125)



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

[1]: https://fd.io/docs/vpp/master/gettingstarted/developers/gdb_examples.html	"开发者文档，包括build gdb"

