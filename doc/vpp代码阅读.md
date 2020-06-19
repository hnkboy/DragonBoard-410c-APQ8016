VLIB_EARLY_CONFIG_FUNCTION  

各个模块配置初始化函数







###### VPP的NODE数据结构

![img](https://www.pianshen.com/images/231/66e01e19821149fc0cc91ee46372f3e7.png)







 node的基础数据结构如上图所示，__bootstrap_vlib_main_vector是最顶级的全局变量，此变量下的vm是一个vec结构，每thread一个vlib_main_t，对于node结构来讲，比较重要的是node_main，对应vlib_node_main_t结构体，在vlib_node_main_t中，nodes成员以vec数据结构组织，用来真正保存一个个注册在VPP系统中的node，vlib_node_t结构中存储了node节点的所有信息。









![img](https://www.pianshen.com/images/84/28003411617e7ca98b1d256333f4adfc.png)

函数流程如上图，vlib_unix_main是初始化入口，函数默认会启动一个线程，在thread0中完成node结构的注册和node graph的创建，vlib_register_all_static_nodes函数用来将vlib_node_registration_t结构下node_registrations链表中的node真正放到vlib_node_main_t结构下的node池中。



[]: 

[]: https://blog.csdn.net/weixin_42265069/article/details/85780560	"VPP高性能从何而来之一：cache优化"









[]: https://segmentfault.com/a/1190000019400623?utm_source=tag-newest	"node数据结构介绍"

节点全局管理结构vlib_node_main_t

节点类型 vlib_node_type_t

节点功能函数描述结构 vlib_node_fn_registration_t

运行数据vlib_node_runtime_t





![](https://img-blog.csdn.net/20170727175817372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3kxNTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





<!--单向箭头表示被包含，双向表示被引用-->











vlib_get_next_frame，vlib_put_next_frame
几乎每个node中必定出现的一对好基友。vlib_get_next_frame获取传递给下一个node的数据包将驻留的内存结构。vlib_put_next_frame把传递给下一个node的数据包写入特定位置。这样下一个node将正式可以被调度框架调度，并处理传给他的数据包



dpdk_device_input   dpdk 报文入口





















Feature结构体
Feature和插件的关系：

插件是结合一些节点信息提供业务功能，在VPP启动时通过加载动态so库来加载

Feature是在VPP启动时通过INIT链表进行加载

在VPP的实现中，每个feature包含一个node，每个node归属于一个ARC的集合

通过feature和插件方式，可以实现不改变源代码，在VPP中动态加载业务节点
————————————————
版权声明：本文为CSDN博主「CNLucius」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/lucky154/article/details/76214144



https://blog.51cto.com/zhangchixiang/1975575  解释feature



/* *INDENT-OFF* */
VLIB_REGISTER_NODE (ip4_input_node) = {
  .function = ip4_input,
  .name = "ip4-input",
  .vector_size = sizeof (u32),
  .n_errors = IP4_N_ERROR,
  .error_strings = ip4_error_strings,
  .n_next_nodes = IP4_INPUT_N_NEXT,
  .next_nodes = {
    [IP4_INPUT_NEXT_DROP] = "error-drop",
    [IP4_INPUT_NEXT_PUNT] = "error-punt",
    [IP4_INPUT_NEXT_LOOKUP] = "ip4-lookup",
    [IP4_INPUT_NEXT_LOOKUP_MULTICAST] = "ip4-lookup-multicast",
    [IP4_INPUT_NEXT_ICMP_ERROR] = "ip4-icmp-error",
  },

  .format_buffer = format_ip4_header,
  .format_trace = format_ip4_input_trace,
};

节点通过函数  .function执行决定下一个节点是.next_nodes中的哪一个，然后通过名字关联下一个节点。当处理流程到ip4-input节点时执行ip4_input函数：ip4_input->ip4_input_inline->vnet_feature_arc_start (arc0, sw_if_index0, &next0, p0);vnet_feature_arc_start函数从ip4-input所在的ip4-unicast类feature里找到优先级最高的feature 节点并执行。







数据调用逻辑

#0  vlib_frame_vector_args (f=0x7fffb5dc24c0) at /root/vpp-20.05/vpp/src/vlib/node_funcs.h:246
#1  0x00007ffff6a90cb5 in virtio_device_input_inline (vm=0x7fffb59031c0, node=0x7fffb5907d40, frame=0x0, vif=0x7fffb5dc0340, qid=0, gso_enabled=0)
    at /root/vpp-20.05/vpp/src/vnet/devices/virtio/node.c:262
#2  0x00007ffff6a913c1 in virtio_input_node_fn_avx2 (vm=0x7fffb59031c0, node=0x7fffb5907d40, frame=0x0) at /root/vpp-20.05/vpp/src/vnet/devices/virtio/node.c:389
#3  0x00007ffff63f7f03 in dispatch_node (vm=0x7fffb59031c0, node=0x7fffb5907d40, type=VLIB_NODE_TYPE_INPUT, dispatch_state=VLIB_NODE_STATE_POLLING, frame=0x0, 
    last_time_stamp=94530433028936) at /root/vpp-20.05/vpp/src/vlib/main.c:1205
#4  0x00007ffff63fa0b7 in vlib_main_or_worker_loop (vm=0x7fffb59031c0, is_main=0) at /root/vpp-20.05/vpp/src/vlib/main.c:1783
#5  0x00007ffff63fabec in vlib_worker_loop (vm=0x7fffb59031c0) at /root/vpp-20.05/vpp/src/vlib/main.c:1937
#6  0x00007ffff643a2b6 in vlib_worker_thread_fn (arg=0x7fffb65ff500) at /root/vpp-20.05/vpp/src/vlib/threads.c:1751
#7  0x00007ffff5e97c90 in clib_calljmp () from /root/vpp-20.05/vpp/build-root/install-vpp_debug-native/vpp/lib/libvppinfra.so.19.08.2
#8  0x00007fffa583bd10 in ?? ()
#9  0x00007ffff64349d3 in vlib_worker_thread_bootstrap_fn (arg=0x7fffb65ff500) at /root/vpp-20.05/vpp/src/vlib/threads.c:573
Backtrace stopped: previous frame inner to this frame (corrupt stack?)





#0  vlib_frame_vector_args (f=0x7fffb5905340) at /root/vpp-20.05/vpp/src/vlib/node_funcs.h:246
#1  0x00007fffb2f2b21b in jer_node_fn_avx2 (vm=0x7fffb59031c0, node=0x7fffb5dc6cc0, frame=0x7fffb5905340) at /root/vpp-20.05/vpp/src/plugins/jer/node.c:102
#2  0x00007ffff63f7f03 in dispatch_node (vm=0x7fffb59031c0, node=0x7fffb5dc6cc0, type=VLIB_NODE_TYPE_INTERNAL, dispatch_state=VLIB_NODE_STATE_POLLING, frame=0x7fffb5905340, 
    last_time_stamp=94711244814371) at /root/vpp-20.05/vpp/src/vlib/main.c:1205
#3  0x00007ffff63f86c4 in dispatch_pending_node (vm=0x7fffb59031c0, pending_frame_index=0, last_time_stamp=94711244814371) at /root/vpp-20.05/vpp/src/vlib/main.c:1373
#4  0x00007ffff63fa369 in vlib_main_or_worker_loop (vm=0x7fffb59031c0, is_main=0) at /root/vpp-20.05/vpp/src/vlib/main.c:1830
#5  0x00007ffff63fabec in vlib_worker_loop (vm=0x7fffb59031c0) at /root/vpp-20.05/vpp/src/vlib/main.c:1937
#6  0x00007ffff643a2b6 in vlib_worker_thread_fn (arg=0x7fffb65ff500) at /root/vpp-20.05/vpp/src/vlib/threads.c:1751
#7  0x00007ffff5e97c90 in clib_calljmp () from /root/vpp-20.05/vpp/build-root/install-vpp_debug-native/vpp/lib/libvppinfra.so.19.08.2
#8  0x00007fffa583bd10 in ?? ()
#9  0x00007ffff64349d3 in vlib_worker_thread_bootstrap_fn (arg=0x7fffb65ff500) at /root/vpp-





 `from = vlib_frame_vector_args (frame);`

根据`vlib_frame_t * frame`获取包的向量`u32 form`, 然后用 `p2 = vlib_get_buffer (vm, from[2]);`得到`vlib_buffer_t * p2`具体的包内容就已经得到了



`vlib_get_next_frame (vm, node, next_index, to_next, n_left_to_next);`

这个函数是为了得到`to_next ` ，表示所指下一个节点的收包缓存的空闲位置首地址





![img](https://img-blog.csdn.net/20170727175817372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3kxNTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



<!--感觉每次节点处理完，需要给下一节点传递的时候，再把数据给赋值到下一次节点，数据是同一块数据-->



`vlib_put_next_frame (vlib_main_t * vm, vlib_node_runtime_t * r,u32 next_index,u32 n_vectors_left)`

主要是处理pending ,处理框架动作方便下一节点需要处理的数据，





  `*next0 = vnet_l2_feature_next (b0, msm->feat_next_node_index, L2INPUT_FEAT_LEARN);`

处理报文处理的下一个节点



`l2input_feat_masks_t` 枚举表示l2-features的特征掩码



`n_cloned = vlib_buffer_clone (vm, bi0, msm->clones[thread_index],n_clones,VLIB_BUFFER_CLONE_HEAD_SIZE);`

复制一个包







`VNET_FEATURE_ARC_INIT`   注册了弧，规定了开始哪个节点。

`VNET_FEATURE_INIT`   注册节点，属于哪条弧。

`VLIB_REGISTER_NODE`



vnet_feature_enable_disable  启用和禁止feature



feature一个特征，有arc弧和node节点组成







结构体里的指针，



```
typedef struct
{

  /* Devices */
...
  dpdk_per_thread_data_t *per_thread_data;

...
} dpdk_main

/** \brief Get vector value at index i checking that i is in bounds. */
#define vec_elt_at_index(v,i)			\
({						\
  ASSERT ((i) < vec_len (v));			\
  (v) + (i);					\
})


```

dpdk_main_t * dm,

dpdk_per_thread_data_t *ptd = vec_elt_at_index (dm->per_thread_data, thread_index);

直接指针，但不带*, 指内存地址，本函数是指针偏移，dm->per_thread_data存的是数组的头地址；











acl 节点 tracelog



Packet 1

00:04:17:384571: vhost-user-input
     VirtualEthernet0/0/0 queue 0
   virtio flags:
    SINGLE_DESC Single descriptor packet
   virtio_net_hdr first_desc_len 374
     flags 0x00 gso_type 0
     num_buff 0
00:04:17:384589: ethernet-input
  frame: flags 0x1, hw-if-index 1, sw-if-index 1
  IP4: 00:00:00:00:00:01 -> ff:ff:ff:ff:ff:ff
00:04:17:384595: l2-input
  l2-input: sw_if_index 1 dst ff:ff:ff:ff:ff:ff src 00:00:00:00:00:01
00:04:17:384600: l2-input-feat-arc
  IN-FEAT-ARC: head 1 feature_bitmap 400405 ethertype 800 sw_if_index 1, next_index 15
00:04:17:384604: acl-plugin-in-ip4-l2
  acl-plugin: lc_index: 0, sw_if_index 1, next index 1, action: 1, match: acl 0 rule 0 trace_bits 00000000
  pkt info 0000000000000000 0000000000000000 0000000000000000 ffffffff00000000 0001011100430044 0200ffff00000000
   lc_index 0 l3 ip4 0.0.0.0 -> 255.255.255.255 l4 lsb_of_sw_if_index 1 proto 17 l4_is_input 1 l4_slow_path 0 l4_flags 0x01 port 68 -> 67 tcp flags (invalid) 00 rsvd 0
00:04:17:384612: l2-input-feat-arc-end
  IN-FEAT-ARC: head 0 feature_bitmap 405 ethertype 0 sw_if_index -1, next_index 7
00:04:17:384613: l2-learn
  l2-learn: sw_if_index 1 dst ff:ff:ff:ff:ff:ff src 00:00:00:00:00:01 bd_index 1
00:04:17:384618: l2-flood
  l2-flood: sw_if_index 1 dst ff:ff:ff:ff:ff:ff src 00:00:00:00:00:01 bd_index 1
  l2-flood: sw_if_index 1 dst ff:ff:ff:ff:ff:ff src 00:00:00:00:00:01 bd_index 1
00:04:17:384626: l2-output
  l2-output: sw_if_index 3 dst ff:ff:ff:ff:ff:ff src 00:00:00:00:00:01 data 08 00 45 c0 01 5c 00 00 00 00 40 11
  l2-output: sw_if_index 2 dst ff:ff:ff:ff:ff:ff src 00:00:00:00:00:01 data 08 00 45 c0 01 5c 00 00 00 00 40 11
00:04:17:384631: tap0-output
  tap0 l2_hdr_offset_valid l3_hdr_offset_valid 
  IP4: 00:00:00:00:00:01 -> ff:ff:ff:ff:ff:ff
  UDP: 0.0.0.0 -> 255.255.255.255
    tos 0xc0, ttl 64, length 348, checksum 0x78d2
    fragment id 0x0000
  UDP: 68 -> 67
    length 328, checksum 0x8875
00:04:17:384635: VirtualEthernet0/0/1-output
  VirtualEthernet0/0/1 l2_hdr_offset_valid l3_hdr_offset_valid 
  IP4: 00:00:00:00:00:01 -> ff:ff:ff:ff:ff:ff
  UDP: 0.0.0.0 -> 255.255.255.255
    tos 0xc0, ttl 64, length 348, checksum 0x78d2
    fragment id 0x0000
  UDP: 68 -> 67
    length 328, checksum 0x8875
00:04:17:384663: error-drop
  rx:VirtualEthernet0/0/0
00:04:17:384669: drop
  VirtualEthernet0/0/1-output: interface is down











根绝vppctl 查看节点顺序，发现这个对得上



/* Mappings from feature ID to graph node name in reverse order */
#define foreach_l2input_feat                    \
 _(DROP,          "feature-bitmap-drop")        \
 _(XCONNECT,      "l2-output")                  \
 _(FLOOD,         "l2-flood")                   \
 _(ARP_UFWD,      "l2-uu-fwd")                  \
 _(ARP_TERM,      "arp-term-l2bd")              \
 _(UU_FLOOD,      "l2-flood")                   \
 _(GBP_FWD,       "gbp-fwd")                    \
 _(UU_FWD,        "l2-uu-fwd")                  \
 _(FWD,           "l2-fwd")                     \
 _(RW,            "l2-rw")                      \
 _(LEARN,         "l2-learn")                   \
 _(L2_EMULATION,  "l2-emulation")               \
 _(GBP_LEARN,     "gbp-learn-l2")               \
 _(GBP_LPM_ANON_CLASSIFY, "l2-gbp-lpm-anon-classify") \
 _(GBP_NULL_CLASSIFY, "gbp-null-classify")      \
 _(GBP_SRC_CLASSIFY,  "gbp-src-classify")       \
 _(GBP_LPM_CLASSIFY,  "l2-gbp-lpm-classify")    \
 _(VTR,           "l2-input-vtr")               \
 _(L2_IP_QOS_RECORD, "l2-ip-qos-record")        \
 _(VPATH,         "vpath-input-l2")             \
 _(ACL,           "l2-input-acl")               \
 _(POLICER_CLAS,  "l2-policer-classify")	\
 _(INPUT_FEAT_ARC, "l2-input-feat-arc")         \
 _(INPUT_CLASSIFY, "l2-input-classify")         \
 _(SPAN,          "span-l2-input")









VNET_FEATURE_INIT  

VNET_FEATURE_ARC_ORDER   这两个函数，把



_vlib_init_function_list_elt_t

https://blog.csdn.net/jqh9804/article/details/54772764  对node节点很详细的讲解









#1  0x00007ffff5a92801 in __GI_abort () at abort.c:79
#2  0x000055555555c05d in os_panic () at /root/vpp-20.05/vpp/src/vpp/vnet/main.c:364
#3  0x00007ffff5e76cbd in debugger () at /root/vpp-20.05/vpp/src/vppinfra/error.c:84
#4  0x00007ffff5e7708c in _clib_error (how_to_die=2, function_name=0x0, line_number=0, fmt=0x7ffff6477138 "%s:%d (%s) assertion `%s' fails") at /root/vpp-20.05/vpp/src/vppinfra/error.c:143
#5  0x00007ffff63f2cec in vlib_node_runtime_get_next_frame (vm=0x7fffb59205c0, n=0x7fffb5b6ebc0, next_index=449) at /root/vpp-20.05/vpp/src/vlib/node_funcs.h:270
#6  0x00007ffff63f49a6 in vlib_get_next_frame_internal (vm=0x7fffb59205c0, node=0x7fffb5b6ebc0, next_index=449, allocate_new_next_frame=0) at /root/vpp-20.05/vpp/src/vlib/main.c:363
#7  0x00007fffb2f2602a in flowtable_node_fn_avx2 (vm=0x7fffb59205c0, node=0x7fffb5b6ebc0, frame=0x7fffb5b7ebc0) at /root/vpp-20.05/vpp/src/plugins/jer/node.c:1044
#8  0x00007ffff63f7f03 in dispatch_node (vm=0x7fffb59205c0, node=0x7fffb5b6ebc0, type=VLIB_NODE_TYPE_INTERNAL, dispatch_state=VLIB_NODE_STATE_POLLING, frame=0x7fffb5b7ebc0, last_time_stamp=159310906802999)
    at /root/vpp-20.05/vpp/src/vlib/main.c:1207
#9  0x00007ffff63f86c4 in dispatch_pending_node (vm=0x7fffb59205c0, pending_frame_index=0, last_time_stamp=159310906802999) at /root/vpp-20.05/vpp/src/vlib/main.c:1375
#10 0x00007ffff63fa369 in vlib_main_or_worker_loop (vm=0x7fffb59205c0, is_main=0) at /root/vpp-20.05/vpp/src/vlib/main.c:1832
#11 0x00007ffff63fabec in vlib_worker_loop (vm=0x7fffb59205c0) at /root/vpp-20.05/vpp/src/vlib/main.c:1939
#12 0x00007ffff643a2b6 in vlib_worker_thread_fn (arg=0x7fffb6194ec0) at /root/vpp-20.05/vpp/src/vlib/threads.c:1751
#13 0x00007ffff5e97c90 in clib_calljmp () from /root/vpp-20.05/vpp/build-root/install-vpp_debug-native/vpp/lib/libvppinfra.so.19.08.2
#14 0x00007fffa5833d10 in ?? ()
#15 0x00007ffff64349d3 in vlib_worker_thread_bootstrap_fn (arg=0x7fffb6194ec0) at /root/vpp-20.05/vpp/src/vlib/threads.c:573
Backtrace stopped: previous frame inner to this frame (corrupt stack?)





###### vpp api编程

```c
    +---------------------+    
    |                    | 
    | VPP API TEST + 
    |                    |
    +----------+---------+           
               |                   
binary API     |                   
  (shmem)      |       
               |   
    +----------+-------+
    |                  |   
    |         VPP      |  
    |                  |   
    +------------------+
```

http://www.mamicode.com/info-detail-2333595.html



![clipboard.png](https://segmentfault.com/img/bVbusBT?w=630&h=318)







round_pow2 什么意思





https://blog.csdn.net/sjin_1314/article/details/103117299  mhash







![img](https://img-blog.csdn.net/20160421143217220?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







![img](https://img-blog.csdn.net/20160421143239421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





multi_acl_match_get_applied_ace_index  

acl 匹配函数的核心    



 applied_hash_ace_entry_t *pae  acl的规则

lc_index ？







clib_bitmap_get 

bit节点，根据索引获取





![avatar](https://zhaozhanxu.com//images/VPP/section17-1.jpg)









acl_plugin_match_5tuple_inline  （）

该函数为查找五元组，由多个模块调用

  acl_plugin_methods_t acl_plugin;   每个模块定义这个