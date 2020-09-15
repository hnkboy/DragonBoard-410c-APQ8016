

devstack 的官网安装过程

https://docs.openstack.org/devstack/latest/#start-the-install

https://docs.openstack.org/cyborg/latest/contributor/devstack_setup.html

博客指导

https://blog.csdn.net/hunjiancuo5340/article/details/85005995



代码位置

https://opendev.org/openstack/devstack







3、执行stack.sh文件后提示

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
+ tools/install_pip.sh:install_get_pip:87  :   curl -f --retry 6 --retry-delay 5 -o /opt/devstack/files/get-pip.py https://bootstrap.pypa.io/get-pip.py
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
  0     0    0     0    0     0      0      0 --:--:--  0:00:01 --:--:--     0Warning: Failed to create the file /opt/devstack/files/get-pip.py: Permission 
Warning: denied
  0 1733k    0  2228    0     0   1295      0  0:22:50  0:00:01  0:22:49  1295
curl: (23) Failed writing body (0 != 2228)
+ tools/install_pip.sh:install_get_pip:89  :   die 89 'Download of get-pip.py failed'
+ functions-common:die:195                 :   local exitcode=23
[Call Trace]
/opt/devstack/tools/install_pip.sh:140:install_get_pip
/opt/devstack/tools/install_pip.sh:89:die
[ERROR] /opt/devstack/tools/install_pip.sh:89 Download of get-pip.py failed
++./stack.sh:main:799                       err_trap
++./stack.sh:err_trap:577                   local r=23
stack.sh failed: full log in /opt/stack/logs/stack.sh.log.2019-08-09-123356
Error on exit
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

是由于已经安装了pip，可以直接在devstack/tools目录下，找到install_pip.py，注释掉install_get_pip这个函数调用。



local.conf









