###### KVM虚拟机Nat方式上网

```
# 查看当前活跃的网络
virsh net-list 
# 查看该网络的详细配置
virsh net-dumpxml default
```

客户机的XML配置文件中interface内容如下即可使用NAT，注意红色字样为关键配置：

```
<interface type='network'>
    <mac address='52:54:00:c7:18:b5'/>
    <source network='default'/>
    <model type='virtio'/>
    <address type='pci' domain='0x0000' bus='0x00' slot='0x03' function='0x0'/>
</interface>

# 编辑修改default网络的配置
virsh net-edit default
# 将default网络干掉，然后再重新定义：
virsh net-undefine default 
# 重新创建一个default.xml文件，自定义其中的内容，可以指定某个mac地址对应某个ip，指定某个ip段。touch default.xml
virsh net-define default.xml
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
# 例如下面的内容，name对应的是客户机的名字。
<?xml version="1.0" encoding="utf-8"?>

<network> 
  <name>default</name>  
  <uuid>dc69ff61-6445-4376-b940-8714a3922bf7</uuid>  
  <forward mode="nat"/>  
  <bridge name="virbr0" stp="on" delay="0"/>  
  <mac address="52:54:00:81:14:18"/>  
  <ip address="192.168.122.1" netmask="255.255.255.0"> 
    <dhcp> 
      <range start="192.168.122.2" end="192.168.122.254"/>  
      <host mac="00:25:90:eb:4b:bb" name="guest1" ip="192.168.5.13"/>  
      <host mac="00:25:90:eb:34:2c" name="guest2" ip="192.168.7.206"/>  
      <host mac="00:25:90:eb:e5:de" name="guest3" ip="192.168.7.207"/>  
      <host mac="00:25:90:eb:7e:11" name="guest4" ip="192.168.7.208"/>  
      <host mac="00:25:90:eb:b2:11" name="guest5" ip="192.168.7.209"/> 
    </dhcp> 
  </ip> 
</network>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
# 生效
virsh net-start default
```





```
virsh net-list
virsh net-destroy default
virsh net-undefine default
virsh net-define default.xml
virsh net-autostart default
virsh net-start default
brctl show
```



配置vhost方式

virsh edit kvm-name

    
    
     <devices>
          <interface type='vhostuser'>
            <mac address='52:54:00:3b:83:1a'/>
            <source type='unix' path='/tmp/vhost1.sock' mode='server'/>
            <model type='virtio'/>
          </interface>
          <interface type='vhostuser'>
            <mac address='52:54:00:3b:83:1b'/>
            <source type='unix' path='/tmp/vhost2.sock' mode='client'>
            <reconnect enabled='yes' timeout='10'/>
            </source>
            <model type='virtio'/>
            <driver queues='5'/>
          </interface>
        </devices>


​                     



###### xml配置文档

在同目录里有配置文件



[redhat配置vhost-user]: https://www.redhat.com/en/blog/hands-vhost-user-warm-welcome-dpdk
[官方配置xml]: https://libvirt.org/formatdomain.html#elementsVirtio
[参考这片文章才成功]: https://blog.csdn.net/shaoyunzhe/article/details/65443766



###### 指定qemu挂在hugepage



在/etc/libvirt/qemu.conf添加

memory_backing_dir = "/mnt/huge" 