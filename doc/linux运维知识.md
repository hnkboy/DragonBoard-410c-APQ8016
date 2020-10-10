# linux运维知识

##### 基础命令

###### 关闭SELinux

临时关闭（不用重启机器）：

```
setenforce 0 #   设置SELinux 成为permissive模式
#setenforce 1    设置SELinux 成为enforcing模式
```

修改配置文件需要重启机器：

```
修改/etc/selinux/config 文件

将SELINUX=enforcing改为SELINUX=disabled
```

查看方法

```
1、/usr/sbin/sestatus -v    ##如果SELinux status参数为enabled即为开启状态
SELinux status:         enabled
2、getenforce         ##也可以用这个命令检查
```

ubuntu 关闭防火墙

```
1、关闭ubuntu的防火墙 
ufw disable
    开启防火墙
ufw enable
2、卸载了iptables
apt-get remove iptables
3、关闭ubuntu中的防火墙的其余命令
iptables -P INPUT ACCEPT
iptables -P FORWARD ACCEPT
iptables -P OUTPUT ACCEPT
iptables -F
```

###### 建立软链接

```
#指定python的
sudo ln -s /usr/bin/python3.4 /usr/bin/python

```

###### 设置dns

 \# vi /etc/resolv.conf

nameserver /*DNS地址*/   #==>此地址就是添加的DNS地址

###### 添加路由

```
route add -host 10.20.30.148 netmask  gw 10.20.30.40

route add -net 10.20.30.148 netmask 255.255.255.0 gw 10.20.30.40
```

```
route add default gw 192.168.1.1 添加默认路由，指向网关
```

###### 查看进程线程状态

`ps -T -p` 查看特定进程的线程

###### 查看页的大小

`getconf PAGE_SIZE`



###### 查看系统信息

```
 #查看当前操作系统版本信息
 cat /proc/version
 cat /etc/issue
 #查看cpu相关信息，包括型号、主频、内核信息
 cat /proc/cpuinfo
```

###### 查看cpu



```
#查看是否支持numa
dmesg | grep -i numa
#查看是否支持kvm
egrep 'vmx|svm' /proc/cpuinfo --color=auto

   查看物理CPU的个数
#cat /proc/cpuinfo |grep "physical id"|sort |uniq|wc -l

   查看逻辑CPU的个数
#cat /proc/cpuinfo |grep "processor"|wc -l

  查看CPU是几核
#cat /proc/cpuinfo |grep "cores"|uniq

    查看CPU的主频
#cat /proc/cpuinfo |grep MHz|uniq
直接获得CPU核心数  （该命令即可全部算出多少核）
 #grep 'model name' /proc/cpuinfo | wc -l

cat /proc/cpuinfo命令部分输出信息的含义

physical id    物理封装的处理器的id
processor      逻辑核的id
core id           一颗处理器中的每个物理核的id
cpu cores      位于相同物理封装的处理器中的物理核的数量
siblings         位于相同物理封装的处理器中的逻辑核的数量
```



###### 配置IP地址 netplan



利用/etc/netplan/01-netcfg.yaml`

和`sudo netplay apply`命令管理IP地址.

```bash
network:
  version: 2
  renderer: networkd
  ethernets:
    enp0s3:
     dhcp4: no
     addresses: [192.168.1.222/24]
     gateway4: 192.168.1.1
     nameservers:
       addresses: [8.8.8.8,8.8.4.4]
```

###### 配置关闭session进程不退出

nohup ./a.out &

使用&后台运行程序：

结果会输出到终端

使用Ctrl + C发送SIGINT信号，程序免疫

关闭session发送SIGHUP信号，程序关闭

使用nohup运行程序：

结果默认会输出到nohup.out

使用Ctrl + C发送SIGINT信号，程序关闭

关闭session发送SIGHUP信号，程序免疫

平日线上经常使用nohup和&配合来启动程序：

同时免疫SIGINT和SIGHUP信号

https://blog.csdn.net/hl449006540/article/details/80216061

###### 配置命令行历史

在linux系统中，history命令可以输出历史命令，历史命令默认保存在文件~/.bash_history中。

HISTFILESIZE 与 HISTSIZE都是history命令需要用到的两个shell变量，这两个变量到底有什么区别呢？

HISTFILESIZE 定义了在 .bash_history 中保存命令的记录总数，可以理解为.bash_history文件中最多只有HISTFILESIZE行

HISTSIZE 定义了 history 命令输出的记录数，即输出.bash_history文件中的最后HISTSIZE行



vim ~/.bashrc

HISTSIZE=10000000
HISTFILESIZE=200000000
HISTTIMEFORMAT='%F %T '



如果想让当前终端的历史命令写入.bash_history文件中，则执行：history -w

如果想获取终端的最新历史命令，则执行：history -r

查看 命令用history

##### 配置特定场景



###### 网卡操作



创建一个 bridge 

ip link add name br0 type bridge

ip link set br0 up 

连接br0

ip link set dev veth0 master br0

bridge link

删除br0

ifdown br0

brctl delbr br0

连接网卡和br

ip link set dev veth0 master br0

ip link set dev eth0 nomaster

 

查看网卡类型

ethtool -i eth0

安装ifconfig命令

yum search ifconfig   提示需要安装net-tools.x86_64

yum install net-tools.x86_64

更改ip地址

 

\# vi /etc/sysconfig/network-scripts/ifcfg-xxx

使用nmcli重新回载网络配置

\# nmcli c reload

如果之前没有xxx的connection，则上一步reload后就已经自动生效了

\# nmcli c up xxx

ip addr  add 172.168.61.17/16 dev

创建tap设备

 

默认创建 tap 接口：

tunctl

以上等价于 tunctl -p

为用户 user 创建一个 tap 接口：

\# tunctl -u user

创建 tun 接口：

tunctl -n

为接口配置 IP 并启用：

\# ifconfig tap0 192.168.0.254 up

\# route add -host 192.168.0.1 dev tap0

删除接口：

\# tunctl -d tap0

ip tuntap#



静态网卡的配置

TYPE=Ethernet
BOOTPROTO=static
IPADDR=192.168.122.10
PREFIX=24
GATEWAY=192.168.122.1
DEFROUTE=yes
PEERDNS=yes
PEERROUTES=yes
IPV4_FAILURE_FATAL=no
IPV6INIT=yes
IPV6_AUTOCONF=yes
IPV6_DEFROUTE=yes
IPV6_PEERDNS=yes
IPV6_PEERROUTES=yes
IPV6_FAILURE_FATAL=no
NAME=eth0
UUID=adfa3b7d-bf60-47e6-8482-871dee686fb5
DEVICE=eth0
ONBOOT=yes
DNS1=8.8.8.8





###### samba共享



1.安装samba  

```
apt-get install samba samba-common //安装samba服务
useradd jerry //创建用户名
passwd  jerry//设置用户密码
#useradd -d / home / user -m user -g root //创建用户用户并同时创建用户目录
#userdel user //删除用户
#groupadd groupname //新建用户组
#usermod -G groupname username //将用户添加进用户组
smbpasswd -a jerry //创建用户
smbpasswd -x //删除用户
```

2.修改配置文件

vim /etc/samba/smb.conf

```
[root]
   comment = share folder
   browseable = yes
   path = /root/
   create mask = 0777
   directory mask = 0777
   valid users = jerry
   force user = root
   force group = root
   public = yes
   available = yes
   writeable = yes

[jerry]
   comment = share folder
   browseable = yes
   path = /home/jerry
   create mask = 0777
   directory mask = 0777
   valid users = jerry
   force user = jerry
   force group = jerry
   public = yes
   available = yes
   writeable = yes
```

3.重启

```
service smbd restart
```

[1]: https://jingyan.baidu.com/article/3a2f7c2ed314ef26afd611a1.html	"参考"

###### 配置ssh

 root用户登录

```
vi  /etc/ssh/sshd_config

增加条目
PermitRootLogin yes
保存文件，重启ssh
```





###### 配置大页内存





非NUMA系统配置

echo 1024 > /sys/kernel/mm/hugepages/hugepages-2048kb/nr_hugepages

NUMA系统中，两个NODE的系统中，预留1024个2M的大页

echo 1024 > /sys/devices/system/node/node0/hugepages/hugepages-2048kb/nr_hugepages
echo 1024 > /sys/devices/system/node/node1/hugepages/hugepages-2048kb/nr_hugepages

大页预留了，下面讲到使用的问题。DPDK使用HUGETLBFS来使用大页。首先，它需要把大页mount到某个路径 比如 /mnt/huge。

mkdir /mnt/huge
mount -t hugetlbfs nodev /mnt/huge

可以开机时自动挂载：
nodev /mnt/huge hugetlbfs defaults 0 0



查看大页内存

```
root@ubuntu:~/train# cat /proc/meminfo  | grep  Huge
AnonHugePages:         0 kB
ShmemHugePages:        0 kB
HugePages_Total:    1024
HugePages_Free:      978
HugePages_Rsvd:        0
HugePages_Surp:        0
Hugepagesize:       2048 kB
Hugetlb:         2097152 kB
```

HugePages_Total 是指系统总共预留了多少、

 HugePages. HugePages_Free 指当前还有多少 

HugePages 未分配 (allocate)，HugePages_Rsvd 是指有多少 

HugePages 是系统承诺了会分配给程序(commitment to allocate)，但实际并未分配。

HugePages_Surp 指超分的页。





###### 查看和启动服务

```
systemctl list-units            ##列出当前系统服务的状态

systemctl list-unit-files       ##列出服务的开机状态

systemctl status sshd           ##查看指定服务的状态

systemctl stop sshd             ##关闭指定服务

systemctl start sshd            ##开启指定服务

systemctl restart sshd          ##从新启动服务

systemctl enable sshd           ##设定指定服务开机开启

systemctl disable sshd          ##设定指定服务开机关闭

systemctl reload sshd           ##使指定服务从新加载配置

systemctl list-dependencies sshd    ##查看指定服务的倚赖关系

systemctl mask  sshd            ##冻结指定服务

systemctl unmask sshd           ##启用服务

systemctl set-default multi-user.target ##开机不开启图形

systemctl set-default graphical.target  ##开机启动图形

setterm                 ##文本界面设定color

```

###### 修改apt源

1、首先备份系统自带`apt-get`源配置文件`/etc/apt/sources.list`
 `cp /etc/apt/sources.list /etc/apt/sources.list.bak`

2、编辑源配置文件/etc/apt/sources.list,替换内容为：

```cpp
deb http://mirrors.aliyun.com/ubuntu/ trusty main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ trusty-security main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ trusty-updates main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ trusty-proposed main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ trusty-backports main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ trusty main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ trusty-security main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ trusty-updates main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ trusty-proposed main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ trusty-backports main restricted universe multiverse
```

3、更新测试一下
apt-get update

###### 安装apt



下载对应的版本http://security.ubuntu.com/ubuntu/pool/main/a/apt/

dpkg -i *deb



###### 设置pip国内源



pip 是一个现代的，通用的 Python 包管理工具。提供了对 Python 包的查找、下载、安装、卸载的功能。

关于pip 默认安装路径有问题的可以看这篇文章：https://blog.csdn.net/weixin_41712059/article/details/82940516

pip国内源设置方法
很多朋友反映在装完pip后，使用pip安装python包特别慢。这主要是应为国内网络不稳定的原因，将pip安装源设置为国内的源即可。

1.pip国内的一些镜像

 阿里云 http://mirrors.aliyun.com/pypi/simple/ 
 中国科技大学 https://pypi.mirrors.ustc.edu.cn/simple/ 
 豆瓣(douban) http://pypi.douban.com/simple/ 
 清华大学 https://pypi.tuna.tsinghua.edu.cn/simple/ 
 中国科学技术大学 http://pypi.mirrors.ustc.edu.cn/simple/

2.临时修改： 
可以在使用pip的时候在后面加上-i参数，指定pip源 
eg: pip install scrapy -i https://pypi.tuna.tsinghua.edu.cn/simple

3.永久修改： 
linux: 
修改 ~/.pip/pip.conf (没有就创建一个)， 内容如下：

```
[global]
index-url = https://pypi.tuna.tsinghua.edu.cn/simple
```





###### 创建用户

sudo不使用密码

\#useradd -s /bin/bash -d /opt/stack -m stack

\#echo “stack ALL=(ALL) NOPASSWD: ALL” | tee /etc/sudoers.d/stack

\#su - stack

###### 修改linux kernel options



那么如何修改呢？

修改linux kernel options
记录过程如下：
通过cmdline查看当前

cat /proc/cmdline

BOOT_IMAGE=/boot/vmlinuz-3.10.0-957.12.2.el7.x86_64 root=LABEL=img-rootfs ro console=tty0 crashkernel=auto console=ttyS0,115200 console=tty0 console=ttyS0,115200 no_timer_check nofb nomodeset vga=normal

修改/etc/default/grub
加入下列配置：

GRUB_CMDLINE_LINUX="console=tty0 console=ttyS0,115200 crashkernel=auto rhgb quiet iommu=pt intel_iommu=on"

每当对此文件进行任何更改时，都必须运行以下命令以将更改应用于GRUB2：

$ sudo update-grub

重启后再查看：

cat /proc/cmdline



###### 配置网卡\*******\*bind\*******\*模式

Linux网口绑定

通过网口绑定(bond)技术,可以很容易实现网口冗余，负载均衡，从而达到高可用高可靠的目的。前提约定：

2个物理网口分别是：eth0,eth1

绑定后的虚拟口是：bond0

服务器IP是：192.168.0.100

第一步，配置设定文件：

/etc/sysconfig/network-scripts/ifcfg-bond0

DEVICE=bond0

BOOTPROTO=none

ONBOOT=yes

IPADDR=192.168.0.100

NETMASK=255.255.255.0

NETWORK=192.168.0.0

BROADCAST=192.168.0.255

\#BROADCAST广播地址

 

/etc/sysconfig/network-scripts/ifcfg-eth0

DEVICE=eth0

BOOTPROTO=none

MASTER=bond0

SLAVE=yes

ONBOOT=yes

/etc/sysconfig/network-scripts/ifcfg-eth1

DEVICE=eth1

BOOTPROTO=none

MASTER=bond0

SLAVE=yes

ONBOOT=yes

第二步，修改modprobe相关设定文件，并加载bonding模块：

1.在这里，我们直接创建一个加载bonding的专属设定文件/etc/modprobe.d/bonding.conf

[root@test ~]# vi /etc/modprobe.d/bonding.conf

\#追加

alias bond0 bonding

options bonding mode=1 miimon=200

2.加载模块(重启系统后就不用手动再加载了)

[root@test ~]# modprobe bonding

3.确认模块是否加载成功：

[root@test ~]# lsmod

grep bonding

bonding 100065 0

第三步，重启一下网络，然后确认一下状况：

[root@test ~]# /etc/init.d/network restart

[root@test ~]# cat /proc/net/bonding/bond0

Ethernet Channel Bonding Driver: v3.5.0 (November 4, 2008)

Bonding Mode: fault-tolerance (active-backup)

Primary Slave: None

Currently Active Slave: eth0

……

[root@test ~]# ifconfig

grep HWaddr

bond0 Link encap:Ethernet HWaddr 00:16:36:1B:BB:74

eth0 Link encap:Ethernet HWaddr 00:16:36:1B:BB:74

eth1 Link encap:Ethernet HWaddr 00:16:36:1B:BB:74

从上面的确认信息中，我们可以看到3个重要信息：

1.现在的bonding模式是active-backup

2.现在Active状态的网口是eth0

3.bond0,eth1的物理地址和处于active状态下的eth0的物理地址相同，这样是为了避免上位交换机发生混乱。

任意拔掉一根网线，然后再访问你的服务器，看网络是否还是通的。

第四步，系统启动自动绑定、增加默认网关：

[root@test ~]# vi /etc/rc.d/rc.local

\#追加

ifenslave bond0 eth0 eth1

route add default gw 192.168.0.1

\#如可上网就不用增加路由，0.1地址按环境修改.

###### 配置gre通道

1、加载ip_gre模块，开通转发

> 要在Linux上创建GRE隧道，需要ip_gre内核模块，它是GRE通过IPv4隧道的驱动程序。

```
在两台LINUX开通路由转发，为等下的路由功能做准备。     
vim /etc/sysctl.conf
修改net.ipv4.ip_forward=0 修改成1
sysctl -p
```

2、建立隧道

```
A机器：
ip tunnel add tun1 mode gre remote 43.xx.xx.11 local 118.xx.xx.184
ip link set tun1 up
ip addr add 2.2.2.1 peer 2.2.2.2 dev tun1        
route add -net 172.16.1.0/24 dev tun1  

B机器：
ip tunnel add tun1 mode gre remote 118.xx.xx.184 local 43.xx.xx.11
ip link set tun1 up
ip addr add 2.2.2.2 peer 2.2.2.1 dev tun1        
```

###### centos安装kvm-qemu

1.查看CPU是否支持KVM
egrep 'vmx|svm' /proc/cpuinfo --color=auto

2.安装KVM环境

安装 kvm 基础包

---

yum -y install kvm

安装 kvm 管理工具

------------------------

yum -y install qemu-kvm python-virtinst libvirt libvirt-python virt-manager libguestfs-tools bridge-utils virt-install

qemu-kvm: KVM模块

pyhon-virtinst: 包含python模块和工具（virt-install，virt-clone和virt-image），

用于安装和克隆虚拟机使用libvirt。 它完全支持paravirtulized客人和客人虚拟客人。 

支持的虚拟机管理程序是Xen，qemu（QEMU）和kvm

libvirt: 虚拟管理模块

virt-manager: 图形界面管理虚拟机

libguestfs* : virt-cat等命令的支持软件包

重启宿主机，以便加载 kvm 模块

------------------------

reboot

查看KVM模块是否被正确加载

---

lsmod | grep kvm
kvm_intel 162153 0

kvm 525259 1 kvm_intel
3.启动并设置开机启动
.
systemctl start acpid.service
systemctl enable acpid.service
systemctl start libvirtd.service
systemctl enable libvirtd.service

systemctl status libvirtd
systemctl is-enabled libvirtd

查看状态操作结果，如Active: active (running)，说明运行情况良好
4.创建虚拟磁盘
1.创建虚拟磁盘 使用qemu命令创建一个10G(大小自定)的硬盘,并命名为:CentOS7
qemu-img create -f raw /data/CentOS7.raw 10G



###### 安装内核头文件



1：获取最新的内核的：apt-cache search linux|grep linux-image*

2：直接安装：apt-get install linux-image-4.8.0-58-generic linux-headers-4.8.0-58-generic

3：apt upgrade Reboot重启  

4：check安装是否成功：sudo uname -r



假设你没有手动编译内核，你可以使用apt-get命令来安装匹配的内核头文件。

首先，使用dpkg-query命令检查是否有可用的内核头文件。

```
$ dpkg-query -s linux-headers-$(uname -r) 
```

------

```
dpkg-query: package 'linux-headers-3.11.0-26-generic' is not installed and no information is available
```

接着使用下面的命令安装匹配的内核头文件。

```
$ sudo apt-get install linux-headers-$(uname -r)
```

###### 编译内核，需要安装的包



sudo apt-get install -y bison flex

sudo apt-get install libncurses5-dev

###### 磁盘满了解决办法

1.手动删除占空间的文件

列出当前目录文件夹大小

du -h --max-depth=1

2.扩容

vmware调整硬盘，

ubuntu安装gparted，https://blog.csdn.net/m0_43403238/article/details/85480314

###### 查看线程在哪个cpu上

```
top -H -p PID
按f键，并使用上下切换，利用空格键选中nTH,P：
按esc键，P所在的列就是线程运行的CPU号：
```

##### qemu命令



qemu-system-x86_64 -vga none -nographic  -s -S  -kernel ./arch/x86_64/boot/bzImage     -initrd ./rootfs  -m 4096   -append "root=/dev/ram rdinit=/helloworld"

普通的img镜像

https://download.cirros-cloud.net/0.5.1/cirros-0.5.1-x86_64-disk.img

###### vhost方式的网卡



```
qemu-system-x86_64 \
    -enable-kvm -m 512 \
    -smp 4 -cpu host \
    -vga none -nographic \
    -drive file=/home/jerry/cirros-0.3.4-x86_64-disk.img,if=virtio,aio=threads \
    -netdev tap,id=mynet1,vhost=on,ifname=tap0,script=/home/jerry/qemu-ifup \
    -device virtio-net-pci,mac=0a:0b:0c:0d:0e:0f,netdev=mynet1 \
    -object memory-backend-file,id=mem,size=512M,mem-path=/dev/hugepages,share=on \
    -numa node,memdev=mem -mem-prealloc \
    -debugcon file:debug.log -global isa-debugcon.iobase=0x402
```

-netdev tap,中指定vhost=on，初始化设备的同时会创建内核线程



vhost-user的网卡



```
qemu-system-x86_64 \
    -enable-kvm -m 512 \
    -smp 4 -cpu host \
    -vga none -nographic \
    -drive file=/home/jerry/cirros-0.3.4-x86_64-disk.img,if=virtio,aio=threads \
    -chardev socket,id=char1,path=/home/jerry/vhost1.sock \
    -netdev type=vhost-user,id=mynet1,chardev=char1,vhostforce \
    -device virtio-net-pci,mac=00:00:00:00:00:01,netdev=mynet1 \
    -object memory-backend-file,id=mem,size=512M,mem-path=/dev/hugepages,share=on \
    -numa node,memdev=mem -mem-prealloc \
    -debugcon file:debug.log -global isa-debugcon.iobase=0x402
```

```
qemu-system-x86_64 \
    -enable-kvm -m 512 \
    -smp 4 -cpu host \
    -vga none -nographic \
    -drive file=/home/jerry/cirros-0.3.4-x86_64-disk.img,if=virtio,aio=threads \
    -chardev socket,id=char1,path=/home/jerry/vhost1.sock,reconnect=10 \
    -netdev type=vhost-user,queues=5,id=mynet1,chardev=char1,vhostforce \
    -device virtio-net-pci,mac=00:00:00:00:00:01,netdev=mynet1 \
    -object memory-backend-file,id=mem,size=512M,mem-path=/dev/hugepages,share=on \
    -numa node,memdev=mem -mem-prealloc \
    -debugcon file:debug.log -global isa-debugcon.iobase=0x402
```



修改 vim  /home/jerry/qemu-ifup

#!/bin/bash
#
bridge=virbr0

if [ -n “$1” ];then
ip link set $1 up
sleep 1
brctl addif $bridge $1
[ $? -eq 0 ] && exit 0 || exit 1
else
echo “Error: no interface specified.”
exit 2
fi



###### 编译命令

mkdir build 

cd build./configure --enable-debug  --prefix=/data  --enable-trace-backend=simple 

../configure 

可执行文件位置

sudo /home/jerry/qemu-5.1.0/x86_64-softmmu/qemu-system-x86_64







##### 特定现象解决办法

###### 用户不能sudo

```
添加sudo文件的写权限
chmod u+w /etc/sudoers

编辑sudoers文件
vi /etc/sudoers
找到root ALL=(ALL) ALL,在他下面添加xxx ALL=(ALL) ALL (这里的xxx是你的用户名)
```

###### connect: Network is unreachable

```
配置网关

临时：route add default gw X.X.X.X

设置dns
```



###### ubuntu 18.04 网络图标不见的问题解决方案

sudo service network-manager stop
sudo rm /var/lib/NetworkManager/NetworkManager.state
sudo service network-manager start



###### cmake

```
cmake .
```

###### 镜像转化

> https://www.cnblogs.com/EasonJim/p/9582202.html





###### git clone进度卡住

> https://blog.csdn.net/weixin_38705903/article/details/100120285

配置了类似proxy的环境变量，

可以用export验证，

`export -p`

解决办法

```
export -n ftp_proxy  
export -n http_proxy 
export -n https_proxy 
```



###### Git 克隆出现RPC错误

Ubuntu Linux用Git 克隆出现错误‘error: RPC failed; curl 56 GnuTLS recv error (-54): Error in the pull...

原因是：由于git默认缓存大小不足导致的。
解决方法是：使用下面的命令增加缓存大小

```
git config --global http.postBuffer 2000000000
```



解决方法
 配置git的最低速度和最低速度时间 单位 秒：

```csharp
git config --global http.lowSpeedLimit 0
git config --global http.lowSpeedTime 999999  
```





出现

error: RPC failed; curl 18 transfer closed with outstanding read data remaining

```
git clone http://github.com/large-repository --depth 1
```



###### libvirtd 启动不起来

systemctl start libvirtd.service

Jul 08 14:52:16 test-server systemd[1]: libvirtd.service: Start request repeated too quickly.
Jul 08 14:52:16 test-server systemd[1]: libvirtd.service: Failed with result 'exit-code'.



因为，它的启动记录文件还存在，需要先删除：

[root@db1 run]# rm /var/run/libvirtd.pid
rm: remove regular file `libvirtd.pid’? y



###### mysql登录问题

vim  /etc/mysql/my.cnf

加入skip-grant-tables

https://blog.csdn.net/qq_30938705/article/details/87184729



###### python 虚拟环境 pip install问题



Retrying (Retry(total=4, connect=None, read=None, redirect=None, status=None))



解决办法：pip install keras -i http://pypi.douban.com/simple --trusted-host pypi.douban.com（其中的keras是你需要下载的，根据自己需求自行更改）



###### 解决GitHub的raw.githubusercontent.com无法连接问题

输入raw.githubusercontent.com

查询IP地址

 

修改hosts Ubuntu，CentOS及macOS直接在终端输入

sudo vi /etc/hosts


添加以下内容保存即可 （IP地址查询后相应修改，可以ping不同IP的延时 选择最佳IP地址）



52.74.223.119 github.com
192.30.253.119 gist.github.com
54.169.195.247 api.github.com
185.199.111.153 assets-cdn.github.com
**151.101.76.133 raw.githubusercontent.com**
151.101.108.133 user-images.githubusercontent.com
151.101.76.133 gist.githubusercontent.com
151.101.76.133 cloud.githubusercontent.com
151.101.76.133 camo.githubusercontent.com
151.101.76.133 avatars0.githubusercontent.com
151.101.76.133 avatars1.githubusercontent.com
151.101.76.133 avatars2.githubusercontent.com
151.101.76.133 avatars3.githubusercontent.com
151.101.76.133 avatars4.githubusercontent.com
151.101.76.133 avatars5.githubusercontent.com
151.101.76.133 avatars6.githubusercontent.com
151.101.76.133 avatars7.githubusercontent.com
151.101.76.133 avatars8.githubusercontent.com



###### docker 指定国内镜像

亲测第三种可用，需要重新创建文件

方法一：

 编辑 /etc/docker/daemon.json

vi /etc/docker/daemon.json
#添加如下网易镜像源
{
"registry-mirrors": ["http://hub-mirror.c.163.com"]
}
方法二：

 编辑/etc/sysconfig/docker，在OPTIONS变量后追加参数  --registry-mirror=镜像源地址

vi /etc/sysconfig/docker
#编辑OPTIONS，添加中国科技大学的镜像源
OPTIONS='--selinux-enabled --log-driver=journald --registry mirror=https://docker.mirrors.ustc.edu.cn'


方法三：

 **编辑/etc/default/docker,添加DOCKER_OPTS="--registry-mirror=镜像源"**

**vi /etc/default/docker**
**#指定镜像源为阿里的镜像源**
**DOCKER_OPTS="--registry-mirror=https://pee6w651.mirror.aliyuncs.com"**
**其他：docker pull拉取镜像时也可以指定仓库下载地址**



###### go 全部包的问题

下载最新版1.15 问题解决能自动下载

```
1.下载全部依赖包：

CMD项目目录下运行下面指令可以自动获取依赖包

go get -d -v ./...

2.设置代理：

配置代理地址：https://goproxy.io，在idea中可以直接配置go moudle的代理地址为这个url；
```



如果被长城墙了，您可以这这么操作：(参看 http://www.fecshop.com/topic/805)

```bash
cd ~/go/src



mkdir -p golang.org/x



cd golang.org/x



git clone https://github.com/golang/sys.git
```

参考：

###### XML如何添加注释



注释以 <!-- 开始并以 --> 结束，

例如 <!--注释内容-->。




注释可以出现在文档序言中，包括文档类型定义 (DTD)；文档之后；或文本内容中。 注释不能出现在属性值中。 不能出现在标记中。

分析器在遇到 > 时，就认为注释已结束；然后继续将文档作为正常的 XML 处理。 因此，字符串 > 不能出现在注释中。 除了该限制之外，任何合法的 XML 字符均可以出现在注释中，与 CDATA 节非常类似。 这样，可以从分析器看到的输出流中删除 XML 注释，同时又不会删除文档的内容。

以下注释可以用于暂时除去标记。

<!--- <test pattern="SECAM" /><test pattern="NTSC" /> -->



##### 命令查询

###### readelf

一般用于查看ELF格式的文件信息，常见的文件如在Linux上的可执行文件，动态库(*.so)或者静态库(*.a) 等包含ELF格式的文件。以下命令的使用是基于android编译出来的so文件上面去运行。  详细用法见 c语言知识文档

###### tcpdump

`tcpdump src host 127.0.0.1 and port 4321 -i lo -w socket_test.pcap`

tcpdump -e 显示mac地址

###### git



`git branch -avv`查看所有分支

`git remote -v`查看远程分支



```
git clone -b 1.18.0 https://github.com/passedbylove/poiji.git
```

git checkout 切换分支

git push



提交注释

git commit -m "misc: add Matt Smith to the committer list.

In hopes of restoring his +2 button...

Type: fix"



- Add ( 新加入的需求 )
- Mod 修改 
- Del 删除 (删除文件等)
- Fixed  ( 修复 bug )
- Changed ( 完成的任务 )
- Updated ( 完成的任务,升级，或者由于第三方模块变化而做的变化 )
- rem:移除（Remove,移除功能）



###### netstat 

netstat -nlp  查看端口情况

netstat -anp |grep 2379  查看端口占用关系

netstat -anpt 查看tcp连接中的地址



###### docker 



docker exec -it

       exec:                                     Run a command in a running container(在运行的容器中运行命令)
       exec -i:  --interactive(相互作用的)       Keep STDIN open even if not attached(即使没有连接，也要保持STDIN打开)
       exec -t:   --tty                          Allocate a pseudo-TTY(分配一个 冒充的终端设备)



###### getconf

查看页的大小

getconf PAGESIZE



###### lshw



lshw -class network -businfo  查看网卡

###### traceroute



###### tracert

windows 命令

###### nmap

端口扫描

sudo nmap -sT -p- 10.10.8.8

sudo nmap -uT -p- 10.10.8.8  扫描udp

###### which

查看命令位置

$ which python
/usr/bin/python

###### lspci



常用参数：

-v 显示设备的详细信息。

-vv 显示设备更详细的信息。

-vvv 显示设备所有可解析的信息。

-x 以16进制显示配置空间的前64字节，或者CardBus桥的前128字节。

-xxx 以16进制显示整个PCI配置空间（256字节）。

-xxxx 以16进制显示整个PCI-E配置空间（4096字节）。

     -s [[[[<domain>]:]<bus>]:][<slot>][.[<func>]]：

显示指定设备。



lspci -vv -s 21:00.0 |grep MSI
Capabilities: [70] MSI-X: Enable+ Count=64 Masked-
count=64 代表支持64个队列

###### gdb

set args

set scheduler-locking off


##### 工具介绍

###### packETH 

发包软件



###### journalctl

systemctl status "devstack@*" | grep ●

journalctl -f --unit devstack@n-sch.service

journalctl -e

```
journalctl -f  以flow形式查看日志
# 查看指定服务日志
$ journalctl -u docker.serivce
#指定进程号
$ journalctl   _PID=1
#从尾部显示
$ journalctl -n 100
```



**1 概述**

日志管理工具journalctl是centos7上专有的日志管理工具，该工具是从message这个文件里读取信息。Systemd统一管理所有Unit的启动日志。带来的好处就是，可以只用journalctl一个命令，查看所有日志（内核日志和应用日志）。日志的配置文件是/etc/systemd/journald.conf

journalctl功能强大，用法非常多。本文将介绍journalctl的相关使用方法。

**2 journalctl 使用方法**

 

**.查看所有日志**

默认情况下，只保存本次启动的日志

journalctl

**.查看内核日志（不显示应用日志）**

journalctl -k

**.查看系统本次启动的日志**

journalctl  -b

journalctl -b -0

**.查看上一次启动的日志**

需更改设置,如上次系统崩溃，需要查看日志时，就要看上一次的启动日志。

journalctl -b -1

**.查看指定时间的日志**

journalctl --since="2012-10-3018:17:16"

journalctl --since "20 minago"

journalctl --since yesterday

journalctl --since"2015-01-10" --until "2015-01-11 03:00"

journalctl --since 09:00 --until"1 hour ago"

journalctl --since"15:15" --until now

**.显示尾部的最新10行日志**

journalctl -n

**.显示尾部指定行数的日志**

查看的是/var/log/messages的日志，但是格式上有所调整，如主机名格式不一样而已

journalctl -n 20

**.实时滚动显示最新日志**

journalctl  -f

**.查看指定服务的日志**

journalctl /usr/lib/systemd/systemd

**.查看指定进程的日志**

journalctl  _PID=1

**.查看某个路径的脚本的日志**

journalctl  /usr/bin/bash

**.查看指定用户的日志**

journalctl _UID=33 --since today

**.查看某个Unit的日志**

journalctl -u nginx.service

journalctl -u nginx.service --since today

**.实时滚动显示某个Unit的最新日志**

journalctl -u nginx.service -f

**.合并显示多个Unit的日志**

journalctl -u nginx.service -u php-fpm.service --since today

**查看指定优先级（及其以上级别）的日志**

日志优先级共有8级

0: emerg

1: alert

2: crit

3: err

4: warning

5: notice

6: info

7: debug

journalctl -p err -b

**.不分页标准输出**

日志默认分页输出--no-pager改为正常的标准输出

journalctl --no-pager

**.以JSON格式（单行）输出**

JSON(JavaScript Object Notation)是一种轻量级的数据交换格式。易于人阅读和编写。同时也易于机器解析和生成。它基于JavaScriptProgramming Language, Standard ECMA-262 3rd Edition - December 1999的一个子集。JSON采用完全独立于语言的文本格式，但是也使用了类似于C语言家族的习惯（包括C, C++, C#, Java,JavaScript, Perl, Python等）。这些特性使JSON成为理想的数据交换语言。

JSON建构于两种结构：

“名称/值”对的集合（A collection ofname/value pairs）：不同的语言中，它被理解为对象（object），纪录（record），结构（struct），字典（dictionary），哈希表（hash table），有键列表（keyed list），或者关联数组（associativearray）。

值的有序列表（An ordered list of values）：在大部分语言中，它被理解为数组（array）。

这些都是常见的数据结构。事实上大部分现代计算机语言都以某种形式支持它们。这使得一种数据格式在同样基于这些结构的编程语言之间交换成为可能。

例子

以JSON格式（单行）输出

journalctl -b -u httpd.service -o json

.以JSON格式（多行）输出，可读性更好，建议选择多行输出

journalctl -b -u httpd.service -o json-pretty

**.显示日志占据的硬盘空间**

journalctl --disk-usage

**.指定日志文件占据的最大空间**

journalctl  --vacuum-size=1G

**.指定日志文件保存多久**

journalctl  --vacuum-time=1years





###### vim





在你的项目根目录运行下面的命令： ctags -R *

- `gc` 选中若干行后, 输入`gc`注释它们。(go comment)

  

- `ngcc` n 是数字，注释 n 行，这个非常好用，也是起飞的关键指令之一。

- Ctrl+]      跳转到定义处

-  Ctrl+t      返回

- 文件间切换 Ctrl+6 :bn //下一个文件

  :bp //上一个文件

  :ls //列出打开的文件，带编号

  :b1~n //切换至第n个文件 对于用(v)split在多个窗格中打开的文件，这种方法只会在当前窗格中切换不同的文件。

- vim已经启动 输入 :e file 可以再打开一个文件，并且此时vim里会显示出file文件的内容。

- ctrl + e

- ,tt			打开关闭tagbar，显示当前文件函数列表，变量列表、属性等

- ```
  W w            移动到下一个单词开头
  E e            移动到下一个单词结尾
  B b            倒退到上一个单词开头
  ```

neocomplete requires ...th Lua support的警告

,安装vim-gnome:

快捷键设置

set pastetoggle=<F12>           " pastetoggle (sane indentation on pastes)

set nu 显示行数

配色方案

echo colorscheme peaksea  >> ~/.vimrc.local





###### scapy

1.只发

sendp(Ether()/IP(dst="1.2.3.4",ttl=(1,1)), iface="lstack")

sendp(Ether(dst='1a:2b:3c:4d:5e:6f')/IP(src="10.10.10.3",dst="1.2.3.4",ttl=(1,1)), iface="tap0")

发送tcp

sendp(Ether(dsendp(Ether(dst='1a:2b:3c:4d:5e:6f')/IP(src="10.10.10.3",dst="1.2.3.4",ttl=(1,1),proto=(6))/TCP(sport=(6),dport=(7)), iface="tap0")

帮助命令

ls(),ls(UDP)





###### isc-dhcp-server



```
推荐用 $ man dhcpd.leases 指令 查看详细含义
```

 cp -f /usr/share/doc/isc-dhcp-server/examples/dhcpd.conf.example /etc/dhcp/dhcpd.conf

安装dhcp

sudo apt-get install isc-dhcp-server

There are two main configuration files for DHCP: /etc/default/isc-dhcp-server and /etc/dhcp/dhcpd.conf

sudo vim /etc/default/isc-dhcp-server 配置使用的网卡

sudo vim /etc/dhcp/dhcpd.conf

```
option domain-name "example.org";

default-lease-time 600;
max-lease-time 7200;
DHCPDARGS=virbr0;
ddns-update-style none;

subnet 192.168.1.0 netmask 255.255.255.0 {
  range 192.168.1.2 192.168.1.244;
  option domain-name-servers 8.8.8.8;
  option subnet-mask 255.255.255.0;
  option routers  192.168.1.1;
  option broadcast-address 192.168.1.255;
  default-lease-time 600;
  max-lease-time 7200;
}
subnet 192.168.0.0 netmask 255.255.255.0 {
  range 192.168.0.2 192.168.0.244;
  option domain-name-servers 8.8.8.8;
  option subnet-mask 255.255.255.0;
  option routers  192.168.0.1;
  option broadcast-address 192.168.0.255;
  default-lease-time 600;
  max-lease-time 7200;
}
```

systemctl restart isc-dhcp-server



出现问题用journalctl解决



###### htop

查看线程的状态，图形化显示

##### 部署场景

###### 安装虚拟机

#1创建虚拟磁盘

qemu-img create -f raw /data/CentOS7.raw 10G

#### 防火墙

##### iptables

```
-t<表>：指定要操纵的表；
-A：向规则链中添加条目；
-D：从规则链中删除条目；
-i：向规则链中插入条目；
-R：替换规则链中的条目；
-L：显示规则链中已有的条目；
-F：清楚规则链中已有的条目；
-Z：清空规则链中的数据包计算器和字节计数器；
-N：创建新的用户自定义规则链；
-P：定义规则链中的默认目标；
-h：显示帮助信息；
-p：指定要匹配的数据包协议类型；
-s：指定要匹配的数据包源ip地址；
-j<目标>：指定要跳转的目标；
-i<网络接口>：指定数据包进入本机的网络接口；
-o<网络接口>：指定数据包要离开本机所使用的网络接口。
```

**iptables命令选项输入顺序：**

```
iptables -t 表名 <-A/I/D/R> 规则链名 [规则号] <-i/o 网卡名> -p 协议名 <-s 源IP/源子网> --sport 源端口 <-d 目标IP/目标子网> --dport 目标端口 -j 动作
```

表名包括：

- **raw**：高级功能，如：网址过滤。
- **mangle**：数据包修改（QOS），用于实现服务质量。
- **net**：地址转换，用于网关路由器。
- **filter**：包过滤，用于防火墙规则。

规则链名包括：

- **INPUT链**：处理输入数据包。
- **OUTPUT链**：处理输出数据包。
- **PORWARD链**：处理转发数据包。
- **PREROUTING链**：用于目标地址转换（DNAT）。
- **POSTOUTING链**：用于源地址转换（SNAT）。

动作包括：

- **[accept](http://man.linuxde.net/accept)**：接收数据包。

- #### **DROP**：丢弃数据包。

- **REDIRECT**：重定向、映射、透明代理。

- **SNAT**：源地址转换。

- **DNAT**：目标地址转换。

- **MASQUERADE**：IP伪装（NAT），用于ADSL。

- **LOG**：日志记录。



查看iptables规则

iptables -L -n

添加

iptables -A INPUT -s 192.168.0.3 -p tcp --dport 22 -j ACCEPT

删除

iptables -D INPUT 3

https://www.cnblogs.com/klb561/p/9011546.html

