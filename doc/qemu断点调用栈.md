

qmp_chardev_open_socket

#0  0x0000555555d6945a in qmp_chardev_open_socket (chr=0x555556afb600, backend=0x555556b10ae0, be_opened=0x7fffffffdcff, errp=0x7fffffffdd00)
    at chardev/char-socket.c:1301
#1  0x0000555555d5e873 in qemu_char_open (chr=0x555556afb600, backend=0x555556b10ae0, be_opened=0x7fffffffdcff, errp=0x7fffffffdd00) at chardev/char.c:248
#2  0x0000555555d6051b in chardev_new (id=0x5555568f53e0 "char1", typename=0x5555568eeb80 "chardev-socket", backend=0x555556b10ae0, gcontext=0x0, errp=0x7fffffffd9
#3  0x0000555555d6065f in qemu_chardev_new (id=0x5555568f53e0 "char1", typename=0x5555568eeb80 "chardev-socket", backend=0x555556b10ae0, gcontext=0x0, errp=0x7fff2
#4  0x0000555555d5fddf in qemu_chr_new_from_opts (opts=0x5555568f5400, context=0x0, errp=0x7fffffffde30) at chardev/char.c:662
#5  0x00005555559934b4 in chardev_init_func (opaque=0x0, opts=0x5555568f5400, errp=0x555556882c00 <error_fatal>) at /home/jerry/qemu-5.1.0/softmmu/vl.c:2115
#6  0x0000555555de967a in qemu_opts_foreach (list=0x5555567f9f00 <qemu_chardev_opts>, func=0x555555993474 <chardev_init_func>, opaque=0x0, errp=0x555556882c00 <er2
#7  0x0000555555997fa8 in qemu_init (argc=28, argv=0x7fffffffe1f8, envp=0x7fffffffe2e0) at /home/jerry/qemu-5.1.0/softmmu/vl.c:4128
#8  0x0000555555d83265 in main (argc=28, argv=0x7fffffffe1f8, envp=0x7fffffffe2e0) at /home/jerry/qemu-5.1.0/softmmu/main.c:48





connect

Thread 4 "qemu-system-x86" hit Breakpoint 1, __libc_connect (fd=11, addr=..., len=110)
    at ../sysdeps/unix/sysv/linux/connect.c:26
26      ../sysdeps/unix/sysv/linux/connect.c: No such file or directory.
(gdb) bt
#0  0x00007ffff5169740 in __libc_connect (fd=11, addr=..., len=110) at ../sysdeps/unix/sysv/linux/connect.c:26
#1  0x0000555555dfe5a2 in unix_connect_saddr (saddr=0x555556b05858, errp=0x7ffff25f08b0) at util/qemu-sockets.c:991
#2  0x0000555555dfea9a in socket_connect (addr=0x555556b05850, errp=0x7ffff25f08b0) at util/qemu-sockets.c:1131
#3  0x0000555555d7bd7b in qio_channel_socket_connect_sync (ioc=0x555556a2da90, addr=0x555556b05850, errp=0x7ffff25f08b0)
    at io/channel-socket.c:145
#4  0x0000555555d68d51 in tcp_chr_connect_client_task (task=0x555556b10f10, opaque=0x555556b05850) at chardev/char-socket.c:1107
#5  0x0000555555d82e42 in qio_task_thread_worker (opaque=0x555556b10f10) at io/task.c:124
#6  0x0000555555de0d86 in qemu_thread_start (args=0x555556b11000) at util/qemu-thread-posix.c:521
#7  0x00007ffff515f6db in start_thread (arg=0x7ffff25f1700) at pthread_create.c:463
#8  0x00007ffff4e88a3f in clone () at ../sysdeps/unix/sysv/linux/x86_64/clone.S:95





Thread 1 "qemu-system-x86" hit Breakpoint 1, qemu_chr_socket_connected (task=0x555556b10f10, opaque=0x555556a642b0)
    at chardev/char-socket.c:1078
1078    {
(gdb) bt
#0  0x0000555555d68baf in qemu_chr_socket_connected (task=0x555556b10f10, opaque=0x555556a642b0) at chardev/char-socket.c:1078
#1  0x0000555555d8315c in qio_task_complete (task=0x555556b10f10) at io/task.c:197
#2  0x0000555555d82df1 in qio_task_thread_result (opaque=0x555556b10f10) at io/task.c:112
#3  0x0000555555d83131 in qio_task_wait_thread (task=0x555556b10f10) at io/task.c:191
#4  0x0000555555d68956 in tcp_chr_wait_connected (chr=0x555556a642b0, errp=0x7fffffffdc80) at chardev/char-socket.c:1017
#5  0x0000555555d5eb05 in qemu_chr_wait_connected (chr=0x555556a642b0, errp=0x7fffffffdc80) at chardev/char.c:323
#6  0x0000555555d620c8 in qemu_chr_fe_wait_connected (be=0x555556b148a8, errp=0x7fffffffdc80) at chardev/char-fe.c:327
#7  0x0000555555c479ea in net_vhost_user_init (peer=0x0, device=0x55555604ab05 "vhost_user", name=0x555556b1b390 "mynet1", chr=03
#8  0x0000555555c47df7 in net_init_vhost_user (netdev=0x555556b145e0, name=0x555556b1b390 "mynet1", peer=0x0, errp=0x7fffffffdf15
#9  0x0000555555c3e176 in net_client_init1 (netdev=0x555556b145e0, is_netdev=true, errp=0x7fffffffdf10) at net/net.c:1008
#10 0x0000555555c3e567 in net_client_init (opts=0x5555568f55f0, is_netdev=true, errp=0x7fffffffdf10) at net/net.c:1113
#11 0x0000555555c3f0a1 in net_init_netdev (dummy=0x0, opts=0x5555568f55f0, errp=0x7fffffffdf10) at net/net.c:1421
#12 0x0000555555de967a in qemu_opts_foreach (list=0x5555567edb40 <qemu_netdev_opts>, func=0x555555c3f078 <net_init_netdev>, opaq2
#13 0x0000555555c3f39f in net_init_clients (errp=0x7fffffffdf10) at net/net.c:1494
#14 0x0000555555998388 in qemu_init (argc=28, argv=0x7fffffffe1e8, envp=0x7fffffffe2d0)
    at /home/jerry/qemu-5.1.0/softmmu/vl.c:4250
#15 0x0000555555d83265 in main (argc=28, argv=0x7fffffffe1e8, envp=0x7fffffffe2d0) at /home/jerry/qemu-5.1.0/softmmu/main.c:48





Continuing.

Thread 1 "qemu-system-x86" hit Breakpoint 3, tcp_chr_write (chr=0x555556a642b0, buf=0x7fffffffb650 "\001", len=12)
    at chardev/char-socket.c:161
161         SocketChardev *s = SOCKET_CHARDEV(chr);
(gdb) bt
#0  0x0000555555d66322 in tcp_chr_write (chr=0x555556a642b0, buf=0x7fffffffb650 "\001", len=12) at chardev/char-socket.c:161
#1  0x0000555555d5e2fb in qemu_chr_write_buffer (s=0x555556a642b0, buf=0x7fffffffb650 "\001", len=12, offset=0x7fffffffb580, wri1
#2  0x0000555555d5e482 in qemu_chr_write (s=0x555556a642b0, buf=0x7fffffffb650 "\001", len=12, write_all=true)
    at chardev/char.c:160
#3  0x0000555555d6158d in qemu_chr_fe_write_all (be=0x555556b148a8, buf=0x7fffffffb650 "\001", len=12) at chardev/char-fe.c:53
#4  0x000055555591aa44 in vhost_user_write (dev=0x555556b3f0e0, msg=0x7fffffffb650, fds=0x0, fd_num=0)
    at /home/jerry/qemu-5.1.0/hw/virtio/vhost-user.c:390
#5  0x000055555591ceed in vhost_user_get_u64 (dev=0x555556b3f0e0, request=1, u64=0x7fffffffb928)
    at /home/jerry/qemu-5.1.0/hw/virtio/vhost-user.c:1243
#6  0x000055555591cfc9 in vhost_user_get_features (dev=0x555556b3f0e0, features=0x7fffffffb928)
    at /home/jerry/qemu-5.1.0/hw/virtio/vhost-user.c:1269
#7  0x000055555591e680 in vhost_user_backend_init (dev=0x555556b3f0e0, opaque=0x7ffff7e23010)
    at /home/jerry/qemu-5.1.0/hw/virtio/vhost-user.c:1812
#8  0x00005555559181e0 in vhost_dev_init (hdev=0x555556b3f0e0, opaque=0x7ffff7e23010, backend_type=VHOST_BACKEND_TYPE_USER, busy4
#9  0x0000555555b53e50 in vhost_net_init (options=0x7fffffffba60) at hw/net/vhost_net.c:186
#10 0x0000555555c4703a in vhost_user_start (queues=5, ncs=0x7fffffffbaf0, be=0x7ffff7e23010) at net/vhost-user.c:88
#11 0x0000555555c476ae in net_vhost_user_event (opaque=0x555556b22e80, event=CHR_EVENT_OPENED) at net/vhost-user.c:271
#12 0x0000555555d5e108 in chr_be_event (s=0x555556a642b0, event=CHR_EVENT_OPENED) at chardev/char.c:61
#13 0x0000555555d5e1a4 in qemu_chr_be_event (s=0x555556a642b0, event=CHR_EVENT_OPENED) at chardev/char.c:81
#14 0x0000555555d61fb4 in qemu_chr_fe_set_handlers_full (b=0x555556b148a8, fd_can_read=0x0, fd_read=0x0, fd_event=0x555555c47571)
    at chardev/char-fe.c:290
#15 0x0000555555d6200d in qemu_chr_fe_set_handlers (b=0x555556b148a8, fd_can_read=0x0, fd_read=0x0, fd_event=0x555555c47571 <net4
#16 0x0000555555c47a37 in net_vhost_user_init (peer=0x0, device=0x55555604ab05 "vhost_user", name=0x555556b1b390 "mynet1", chr=07
#17 0x0000555555c47df7 in net_init_vhost_user (netdev=0x555556b145e0, name=0x555556b1b390 "mynet1", peer=0x0, errp=0x7fffffffdf15
#18 0x0000555555c3e176 in net_client_init1 (netdev=0x555556b145e0, is_netdev=true, errp=0x7fffffffdf10) at net/net.c:1008
#19 0x0000555555c3e567 in net_client_init (opts=0x5555568f55f0, is_netdev=true, errp=0x7fffffffdf10) at net/net.c:1113
#20 0x0000555555c3f0a1 in net_init_netdev (dummy=0x0, opts=0x5555568f55f0, errp=0x7fffffffdf10) at net/net.c:1421
#21 0x0000555555de967a in qemu_opts_foreach (list=0x5555567edb40 <qemu_netdev_opts>, func=0x555555c3f078 <net_init_netdev>, opaq2
#22 0x0000555555c3f39f in net_init_clients (errp=0x7fffffffdf10) at net/net.c:1494
#23 0x0000555555998388 in qemu_init (argc=28, argv=0x7fffffffe1e8, envp=0x7fffffffe2d0)
    at /home/jerry/qemu-5.1.0/softmmu/vl.c:4250
#24 0x0000555555d83265 in main (argc=28, argv=0x7fffffffe1e8, envp=0x7fffffffe2d0) at /home/jerry/qemu-5.1.0/softmmu/main.c:48
(gdb) c
Continuing.

Thread 1 "qemu-system-x86" hit Breakpoint 1, tcp_chr_sync_read (chr=0x555556a642b0, buf=0x7fffffffb650 "\001", len=12)
    at chardev/char-socket.c:547
547         SocketChardev *s = SOCKET_CHARDEV(chr);
(gdb) bt
#0  0x0000555555d67388 in tcp_chr_sync_read (chr=0x555556a642b0, buf=0x7fffffffb650 "\001", len=12) at chardev/char-socket.c:547
#1  0x0000555555d6168a in qemu_chr_fe_read_all (be=0x555556b148a8, buf=0x7fffffffb650 "\001", len=12) at chardev/char-fe.c:72
#2  0x000055555591a727 in vhost_user_read_header (dev=0x555556b3f0e0, msg=0x7fffffffb650)
    at /home/jerry/qemu-5.1.0/hw/virtio/vhost-user.c:279
#3  0x000055555591a7da in vhost_user_read (dev=0x555556b3f0e0, msg=0x7fffffffb650)
    at /home/jerry/qemu-5.1.0/hw/virtio/vhost-user.c:304
#4  0x000055555591cf14 in vhost_user_get_u64 (dev=0x555556b3f0e0, request=1, u64=0x7fffffffb928)
    at /home/jerry/qemu-5.1.0/hw/virtio/vhost-user.c:1247
#5  0x000055555591cfc9 in vhost_user_get_features (dev=0x555556b3f0e0, features=0x7fffffffb928)
    at /home/jerry/qemu-5.1.0/hw/virtio/vhost-user.c:1269
#6  0x000055555591e680 in vhost_user_backend_init (dev=0x555556b3f0e0, opaque=0x7ffff7e23010)
    at /home/jerry/qemu-5.1.0/hw/virtio/vhost-user.c:1812
#7  0x00005555559181e0 in vhost_dev_init (hdev=0x555556b3f0e0, opaque=0x7ffff7e23010, backend_type=VHOST_BACKEND_TYPE_USER, busy4
#8  0x0000555555b53e50 in vhost_net_init (options=0x7fffffffba60) at hw/net/vhost_net.c:186
#9  0x0000555555c4703a in vhost_user_start (queues=5, ncs=0x7fffffffbaf0, be=0x7ffff7e23010) at net/vhost-user.c:88
#10 0x0000555555c476ae in net_vhost_user_event (opaque=0x555556b22e80, event=CHR_EVENT_OPENED) at net/vhost-user.c:271
#11 0x0000555555d5e108 in chr_be_event (s=0x555556a642b0, event=CHR_EVENT_OPENED) at chardev/char.c:61
#12 0x0000555555d5e1a4 in qemu_chr_be_event (s=0x555556a642b0, event=CHR_EVENT_OPENED) at chardev/char.c:81
#13 0x0000555555d61fb4 in qemu_chr_fe_set_handlers_full (b=0x555556b148a8, fd_can_read=0x0, fd_read=0x0, fd_event=0x555555c47571)
    at chardev/char-fe.c:290
#14 0x0000555555d6200d in qemu_chr_fe_set_handlers (b=0x555556b148a8, fd_can_read=0x0, fd_read=0x0, fd_event=0x555555c47571 <net4
#15 0x0000555555c47a37 in net_vhost_user_init (peer=0x0, device=0x55555604ab05 "vhost_user", name=0x555556b1b390 "mynet1", chr=07
#16 0x0000555555c47df7 in net_init_vhost_user (netdev=0x555556b145e0, name=0x555556b1b390 "mynet1", peer=0x0, errp=0x7fffffffdf15
#17 0x0000555555c3e176 in net_client_init1 (netdev=0x555556b145e0, is_netdev=true, errp=0x7fffffffdf10) at net/net.c:1008
#18 0x0000555555c3e567 in net_client_init (opts=0x5555568f55f0, is_netdev=true, errp=0x7fffffffdf10) at net/net.c:1113
#19 0x0000555555c3f0a1 in net_init_netdev (dummy=0x0, opts=0x5555568f55f0, errp=0x7fffffffdf10) at net/net.c:1421
#20 0x0000555555de967a in qemu_opts_foreach (list=0x5555567edb40 <qemu_netdev_opts>, func=0x555555c3f078 <net_init_netdev>, opaq2
#21 0x0000555555c3f39f in net_init_clients (errp=0x7fffffffdf10) at net/net.c:1494
#22 0x0000555555998388 in qemu_init (argc=28, argv=0x7fffffffe1e8, envp=0x7fffffffe2d0)
    at /home/jerry/qemu-5.1.0/softmmu/vl.c:4250
#23 0x0000555555d83265 in main (argc=28, argv=0x7fffffffe1e8, envp=0x7fffffffe2d0) at /home/jerry/qemu-5.1.0/softmmu/main.c:48
(gdb) 

启动linux后

Thread 7 "qemu-system-x86" hit Breakpoint 1, virtio_pci_config_write (opaque=0x5555576a8fe0, addr=18, val=0, size=1)
    at hw/virtio/virtio-pci.c:451
451         VirtIOPCIProxy *proxy = opaque;
(gdb) bt
#0  0x0000555555bfae50 in virtio_pci_config_write (opaque=0x5555576a8fe0, addr=18, val=0, size=1) at hw/virtio/virtio-pci.c:451
#1  0x00005555559831dd in memory_region_write_accessor (mr=0x5555576a98d0, addr=18, value=0x7ffff1c2b6b8, size=1, shift=0, mask=3
#2  0x00005555559833fb in access_with_adjusted_size (addr=18, value=0x7ffff1c2b6b8, size=1, access_size_min=1, access_size_max=4)
    at /home/jerry/qemu-5.1.0/softmmu/memory.c:544
#3  0x00005555559862e0 in memory_region_dispatch_write (mr=0x5555576a98d0, addr=18, data=0, op=MO_8, attrs=...)
    at /home/jerry/qemu-5.1.0/softmmu/memory.c:1465
#4  0x000055555582f8c8 in flatview_write_continue (fv=0x7fffbc2011a0, addr=49298, attrs=..., ptr=0x7ffff7ff6000, len=1, addr1=186
#5  0x000055555582fa0d in flatview_write (fv=0x7fffbc2011a0, addr=49298, attrs=..., buf=0x7ffff7ff6000, len=1)
    at /home/jerry/qemu-5.1.0/exec.c:3216
#6  0x000055555582fd62 in address_space_write (as=0x55555681c1c0 <address_space_io>, addr=49298, attrs=..., buf=0x7ffff7ff6000, 7
#7  0x000055555582fdcf in address_space_rw (as=0x55555681c1c0 <address_space_io>, addr=49298, attrs=..., buf=0x7ffff7ff6000, len7
#8  0x000055555588fb0b in kvm_handle_io (port=49298, attrs=..., data=0x7ffff7ff6000, direction=1, size=1, count=1)
    at /home/jerry/qemu-5.1.0/accel/kvm/kvm-all.c:2262
#9  0x0000555555890274 in kvm_cpu_exec (cpu=0x555556b58750) at /home/jerry/qemu-5.1.0/accel/kvm/kvm-all.c:2508
#10 0x000055555597dd3f in qemu_kvm_cpu_thread_fn (arg=0x555556b58750) at /home/jerry/qemu-5.1.0/softmmu/cpus.c:1188
#11 0x0000555555de0d86 in qemu_thread_start (args=0x555556b7fdf0) at util/qemu-thread-posix.c:521
#12 0x00007ffff515f6db in start_thread (arg=0x7ffff1c2c700) at pthread_create.c:463
#13 0x00007ffff4e88a3f in clone () at ../sysdeps/unix/sysv/linux/x86_64/clone.S:95







Thread 1 "qemu-system-x86" hit Breakpoint 1, virtio_set_status (vdev=0x55555780d230, val=0 '\000')
    at /home/jerry/qemu-5.1.0/hw/virtio/virtio.c:1937
1937        VirtioDeviceClass *k = VIRTIO_DEVICE_GET_CLASS(vdev);
(gdb) bt
#0  0x000055555590f25f in virtio_set_status (vdev=0x55555780d230, val=0 '\000')
    at /home/jerry/qemu-5.1.0/hw/virtio/virtio.c:1937
#1  0x000055555590f446 in virtio_reset (opaque=0x55555780d230) at /home/jerry/qemu-5.1.0/hw/virtio/virtio.c:1990
#2  0x0000555555bf82ec in virtio_bus_reset (bus=0x55555780d1a8) at hw/virtio/virtio-bus.c:95
#3  0x0000555555bfe637 in virtio_pci_reset (qdev=0x5555578050a0) at hw/virtio/virtio-pci.c:1859
#4  0x0000555555a9d90a in device_transitional_reset (obj=0x5555578050a0) at hw/core/qdev.c:1114
#5  0x0000555555aa3c79 in resettable_phase_hold (obj=0x5555578050a0, opaque=0x0, type=RESET_TYPE_COLD)
    at hw/core/resettable.c:182
#6  0x0000555555aa1557 in bus_reset_child_foreach (obj=0x555556d1cf30, cb=0x555555aa3b29 <resettable_phase_hold>, opaque=0x0, ty4
#7  0x0000555555aa393c in resettable_child_foreach (rc=0x555556953fd0, obj=0x555556d1cf30, cb=0x555555aa3b29 <resettable_phase_h6
#8  0x0000555555aa3bff in resettable_phase_hold (obj=0x555556d1cf30, opaque=0x0, type=RESET_TYPE_COLD)
    at hw/core/resettable.c:173
#9  0x0000555555a9b931 in device_reset_child_foreach (obj=0x555556d1bc90, cb=0x555555aa3b29 <resettable_phase_hold>, opaque=0x0,8
#10 0x0000555555aa393c in resettable_child_foreach (rc=0x5555569f8210, obj=0x555556d1bc90, cb=0x555555aa3b29 <resettable_phase_h6
#11 0x0000555555aa3bff in resettable_phase_hold (obj=0x555556d1bc90, opaque=0x0, type=RESET_TYPE_COLD)
    at hw/core/resettable.c:173
#12 0x0000555555aa1557 in bus_reset_child_foreach (obj=0x555556b04620, cb=0x555555aa3b29 <resettable_phase_hold>, opaque=0x0, ty4
#13 0x0000555555aa393c in resettable_child_foreach (rc=0x555556a410a0, obj=0x555556b04620, cb=0x555555aa3b29 <resettable_phase_h6
#14 0x0000555555aa3bff in resettable_phase_hold (obj=0x555556b04620, opaque=0x0, type=RESET_TYPE_COLD)
    at hw/core/resettable.c:173
#15 0x0000555555aa37cf in resettable_assert_reset (obj=0x555556b04620, type=RESET_TYPE_COLD) at hw/core/resettable.c:60
#16 0x0000555555aa3713 in resettable_reset (obj=0x555556b04620, type=RESET_TYPE_COLD) at hw/core/resettable.c:45
#17 0x0000555555aa401e in resettable_cold_reset_fn (opaque=0x555556b04620) at hw/core/resettable.c:269
#18 0x0000555555aa54ab in qemu_devices_reset () at hw/core/reset.c:69
#19 0x00005555559433b1 in pc_machine_reset (machine=0x555556a85c00) at /home/jerry/qemu-5.1.0/hw/i386/pc.c:1898
#20 0x0000555555990892 in qemu_system_reset (reason=SHUTDOWN_CAUSE_NONE) at /home/jerry/qemu-5.1.0/softmmu/vl.c:1403
#21 0x00005555559988b5 in qemu_init (argc=28, argv=0x7fffffffe1e8, envp=0x7fffffffe2d0)
    at /home/jerry/qemu-5.1.0/softmmu/vl.c:4458
#22 0x000





(gdb) bt
#0  0x000055555597f414 in qemu_kvm_start_vcpu (cpu=0x555556b58790) at /home/jerry/qemu-5.1.0/softmmu/cpus.c:1955
#1  0x000055555597f779 in qemu_init_vcpu (cpu=0x555556b58790) at /home/jerry/qemu-5.1.0/softmmu/cpus.c:2032
#2  0x00005555559a4815 in x86_cpu_realizefn (dev=0x555556b58790, errp=0x7fffffffda90)
    at /home/jerry/qemu-5.1.0/target/i386/cpu.c:6743
#3  0x0000555555a9ce99 in device_set_realized (obj=0x555556b58790, value=true, errp=0x7fffffffdb98) at hw/core/qdev.c:864
#4  0x0000555555ca3839 in property_set_bool (obj=0x555556b58790, v=
    0x555556b76640, name=0x555555fe64a9 "realized", opaque=0x5555568f90f0, errp=0x7fffffffdb98) at qom/object.c:2202
#5  0x0000555555ca195a in object_property_set (obj=0x555556b58790, name=0x555555fe64a9 "realized", v=
    0x555556b76640, errp=0x555556882c00 <error_fatal>) at qom/object.c:1349
#6  0x0000555555ca4c9f in object_property_set_qobject (obj=0x555556b58790, name=0x555555fe64a9 "realized", value=0x555556b73ea0,8
#7  0x0000555555ca1ca1 in object_property_set_bool (obj=0x555556b58790, name=0x555555fe64a9 "realized", value=true, errp=0x555556
#8  0x0000555555a9ba4d in qdev_realize (dev=0x555556b58790, bus=0x0, errp=0x555556882c00 <error_fatal>) at hw/core/qdev.c:379
#9  0x000055555593bd19 in x86_cpu_new (x86ms=0x555556a85c00, apic_id=0, errp=0x555556882c00 <error_fatal>)
    at /home/jerry/qemu-5.1.0/hw/i386/x86.c:126
#10 0x000055555593bea4 in x86_cpus_init (x86ms=0x555556a85c00, default_cpu_version=1)
    at /home/jerry/qemu-5.1.0/hw/i386/x86.c:164
#11 0x0000555555944692 in pc_init1 (machine=0x555556a85c00, host_type=0x555555fa8dbc "i440FX-pcihost", pci_type=0x555555fa8db5 "9
#12 0x00005555559452e8 in pc_init_v5_1 (machine=0x555556a85c00) at /home/jerry/qemu-5.1.0/hw/i386/pc_piix.c:438
#13 0x0000555555aaa520 in machine_run_board_init (machine=0x555556a85c00) at hw/core/machine.c:1135
#14 0x00005555559986d6 in qemu_init (argc=28, argv=0x7fffffffe1e8, envp=0x7fffffffe2d0)
    at /home/jerry/qemu-5.1.0/softmmu/vl.c:4355
#15 0x0000555555d83265 in main (argc=28, argv=0x7fffffffe1e8, envp=0x7fffffffe2d0) at /home/jerry/qemu-5.1.0/softmmu/main.c:48









#0  0x0000559563361da3 in event_notifier_init (e=0x7fd786ef2118, active=0)
    at util/event_notifier-posix.c:68
#1  0x0000559563180125 in virtio_pci_set_guest_notifier (d=0x559565f229f0, n=1, assign=true, with_irqfd=false) at hw/virtio/virtio-pci.c:947
#2  0x00005595631803a3 in virtio_pci_set_guest_notifiers (d=0x559565f229f0, nvqs=2, assign=true) at hw/virtio/virtio-pci.c:1005
#3  0x00005595630d8518 in vhost_net_start (dev=0x559565f2ab80, ncs=0x559565f3f180, total_queues=1) at hw/net/vhost_net.c:343
#4  0x0000559562e664dd in virtio_net_vhost_status (n=0x559565f2ab80, status=15 '\017') at /home/jerry/qemu-5.1.0/hw/net/virtio-net.c:276
#5  0x0000559562e667bc in virtio_net_set_status (vdev=0x559565f2ab80, status=15 '\017') at /home/jerry/qemu-5.1.0/hw/net/virtio-net.c:357
#6  0x0000559562e93351 in virtio_set_status (vdev=0x559565f2ab80, val=15 '\017')
    at /home/jerry/qemu-5.1.0/hw/virtio/virtio.c:1957
#7  0x0000559563180da8 in virtio_pci_common_write (opaque=0x559565f229f0, addr=20, val=15, size=1) at hw/virtio/virtio-pci.c:1257
#8  0x0000559562f071dd in memory_region_write_accessor (mr=0x559565f233d0, addr=20, value=0x7fd7877bc6f8, size=1, shift=0, mask=255, attrs=...)
    at /home/jerry/qemu-5.1.0/softmmu/memory.c:483
#9  0x0000559562f073fb in access_with_adjusted_size (addr=20, value=0x7fd7877bc6f8, size=1, access_size_min=1, access_size_max=4, access_fn=
    0x559562f070fe <memory_region_write_accessor>, mr=0x559565f233d0, attrs=...)
    at /home/jerry/qemu-5.1.0/softmmu/memory.c:544
#10 0x0000559562f0a2e0 in memory_region_dispatch_write (mr=0x559565f233d0, addr=20, data=15, op=MO_8, attrs=...) at /home/jerry/qemu-5.1.0/softmmu/memory.c:1465
#11 0x0000559562db38c8 in flatview_write_continue (fv=0x7fd75456b1d0, addr=4273963028, attrs=..., ptr=0x7fd791b92028, len=1, addr1=20, l=1, mr=0x559565f233d0)
    at /home/jerry/qemu-5.1.0/exec.c:3176
#12 0x0000559562db3a0d in flatview_write (fv=0x7fd75456b1d0, addr=4273963028, attrs=..., buf=0x7fd791b92028, len=1) at /home/jerry/qemu-5.1.0/exec.c:3216
#13 0x0000559562db3d62 in address_space_write (as=0x559563da0220 <address_space_memory>, addr=4273963028, attrs=..., buf=0x7fd791b92028, len=1)
    at /home/jerry/qemu-5.1.0/exec.c:3307





#0  0x000055555588e64e in kvm_irqchip_assign_irqfd (s=0x555556b336f0, event=0x7ffff11d9080, resample=0x0, virq=2, assign=true) at /home/jerry/qemu-5.1.0/accel/kvm/kvm-all.c:1717
#1  0x000055555588e99e in kvm_irqchip_add_irqfd_notifier_gsi (s=0x555556b336f0, n=0x7ffff11d9080, rn=0x0, virq=2) at /home/jerry/qemu-5.1.0/accel/kvm/kvm-all.c:1866
#2  0x0000555555bfb73a in kvm_virtio_pci_irqfd_use (proxy=0x555557766ac0, queue_no=0, vector=1) at hw/virtio/virtio-pci.c:700
#3  0x0000555555bfbc6c in virtio_pci_vq_vector_unmask (proxy=0x555557766ac0, queue_no=0, vector=1, msg=...) at hw/virtio/virtio-pci.c:825
#4  0x0000555555bfbdd6 in virtio_pci_vector_unmask (dev=0x555557766ac0, vector=1, msg=...) at hw/virtio/virtio-pci.c:861
#5  0x0000555555b7839a in msix_set_notifier_for_vector (dev=0x555557766ac0, vector=1) at hw/pci/msix.c:568
#6  0x0000555555b7849c in msix_set_vector_notifiers (dev=0x555557766ac0, use_notifier=0x555555bfbd25 <virtio_pci_vector_unmask>, release_notifier=0x555555bfbe7a <virtio_pci_vector_m5
#7  0x0000555555bfc487 in virtio_pci_set_guest_notifiers (d=0x555557766ac0, nvqs=1, assign=true) at hw/virtio/virtio-pci.c:1022
#8  0x00005555558c1c4b in virtio_blk_data_plane_start (vdev=0x55555776ec50) at /home/jerry/qemu-5.1.0/hw/block/dataplane/virtio-blk.c:193
#9  0x0000555555bf88bd in virtio_bus_start_ioeventfd (bus=0x55555776ebc8) at hw/virtio/virtio-bus.c:222
#10 0x0000555555bfa8cb in virtio_pci_start_ioeventfd (proxy=0x555557766ac0) at hw/virtio/virtio-pci.c:287
#11 0x0000555555bfcdf6 in virtio_pci_common_write (opaque=0x555557766ac0, addr=20, val=15, size=1) at hw/virtio/virtio-pci.c:1260
#12 0x0000555555983213 in memory_region_write_accessor (mr=0x5555577674a0, addr=20, value=0x7ffff25f06f8, size=1, shift=0, mask=255, attrs=...)
    at /home/jerry/qemu-5.1.0/softmmu/memory.c:483
#13 0x0000555555983431 in access_with_adjusted_size (addr=20, value=0x7ffff25f06f8, size=1, access_size_min=1, access_size_max=4, access_fn=
    0x555555983134 <memory_region_write_accessor>, mr=0x5555577674a0, attrs=...) at /home/jerry/qemu-5.1.0/softmmu/memory.c:544
#14 0x0000555555986316 in memory_region_dispatch_write (mr=0x5555577674a0, addr=20, data=15, op=MO_8, attrs=...) at /home/jerry/qemu-5.1.0/softmmu/memory.c:1465
#15 0x000055555582f8c8 in flatview_write_continue (fv=0x7fffbc25e550, addr=4273979412, attrs=..., ptr=0x7ffff7ff5028, len=1, addr1=20, l=1, mr=0x5555577674a0)
    at /home/jerry/qemu-5.1.0/exec.c:3176
#16 0x000055555582fa0d in flatview_write (fv=0x7fffbc25e550, addr=4273979412, attrs=..., buf=0x7ffff7ff5028, len=1) at /home/jerry/qemu-5.1.0/exec.c:3216
#17 0x000055555582fd62 in address_space_write (as=0x55555681c220 <address_space_memory>, addr=4273979412, attrs=..., buf=0x7ffff7ff5028, len=1) at /home/jerry/qemu-5.1.0/exec.c:3307
#18 0x000055555582fdcf in address_space_rw (as=0x55555681c220 <address_space_memory>, addr=4273979412, attrs=..., buf=0x7ffff7ff5028, len=1, is_write=true)
    at /home/jerry/qemu-5.1.0/exec.c:3317
#19 0x00005555558902c1 in kvm_cpu_exec (cpu=0x555556b4b070) at /home/jerry/qemu-5.1.0/accel/kvm/kvm-all.c:2518
#20 0x000055555597dd75 in qemu_kvm_cpu_thread_fn (arg=0x555556b4b070) at /home/jerry/qemu-5.1.0/softmmu/cpus.c:1188
#21 0x0000555555de0dde in qemu_thread_start (args=0x555556b72b20) at util/qemu-thread-posix.c:521
---Type <return> to continue, or q <return> to quit---