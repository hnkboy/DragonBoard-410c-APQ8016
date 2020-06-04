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
route add -host 10.20.30.148 gw 10.20.30.40
```

```
route add default gw 192.168.1.1 添加默认路由
```



##### 配置特定场景



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

###### cmake

```
cmake .
```

###### 镜像转化

> https://www.cnblogs.com/EasonJim/p/9582202.html





###### git clone进度卡住

> https://blog.csdn.net/weixin_38705903/article/details/100120285



###### Samba 配置

> https://jingyan.baidu.com/article/3a2f7c2ed314ef26afd611a1.html



##### 工具的使用

###### tcpdump

`tcpdump src host 127.0.0.1 and port 4321 -i lo -w socket_test.pcap`