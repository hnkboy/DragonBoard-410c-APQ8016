VLIB_EARLY_CONFIG_FUNCTION  

各个模块配置初始化函数



##### 编程技巧重点



###### 下标和长度

  if (slot == ~0)
    slot = vec_len (node->next_nodes);

  vec_validate_init_empty (node->next_nodes, slot, ~0);
  /*取消最后的node节点注册，vec_validate是按照索引扩充的，正好是扩充一位*/
  if ((old_next_index = node->next_nodes[slot]) != ~0u)
    

vec_validate_init_empty 是按照下标扩充的，以长度做下标刚好多出最后一个





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







  l2input_intf_bitmap_enable (sw_if_index, L2INPUT_FEAT_FLOOD, enable);

是能二层feature



进入arp节点







00:27:21:229371: vhost-user-input
     VirtualEthernet0/0/0 queue 0
   virtio flags:
    SINGLE_DESC Single descriptor packet
   virtio_net_hdr first_desc_len 374
     flags 0x00 gso_type 0
     num_buff 0
00:27:21:229384: ethernet-input
  frame: flags 0x1, hw-if-index 1, sw-if-index 1
  IP4: 00:00:00:00:00:01 -> ff:ff:ff:ff:ff:ff
00:27:21:229390: ip4-input
  UDP: 0.0.0.0 -> 255.255.255.255
    tos 0xc0, ttl 64, length 348, checksum 0x78d2
    fragment id 0x0000
  UDP: 68 -> 67
    length 328, checksum 0xc3b2
00:27:21:229395: ip4-lookup
  fib 0 dpo-idx 0 flow hash: 0x00000000
  UDP: 0.0.0.0 -> 255.255.255.255
    tos 0xc0, ttl 64, length 348, checksum 0x78d2
    fragment id 0x0000
  UDP: 68 -> 67
    length 328, checksum 0xc3b2
00:27:21:229403: ip4-drop
    UDP: 0.0.0.0 -> 255.255.255.255
      tos 0xc0, ttl 64, length 348, checksum 0x78d2
      fragment id 0x0000
    UDP: 68 -> 67
      length 328, checksum 0xc3b2
00:27:21:229407: error-drop
  rx:VirtualEthernet0/0/0
00:27:21:229411: drop
  null-node: blackholed packets



进入ipv4-input节点

00:27:21:229371: vhost-user-input
     VirtualEthernet0/0/0 queue 0
   virtio flags:
    SINGLE_DESC Single descriptor packet
   virtio_net_hdr first_desc_len 374
     flags 0x00 gso_type 0
     num_buff 0
00:27:21:229384: ethernet-input
  frame: flags 0x1, hw-if-index 1, sw-if-index 1
  IP4: 00:00:00:00:00:01 -> ff:ff:ff:ff:ff:ff
00:27:21:229390: ip4-input
  UDP: 0.0.0.0 -> 255.255.255.255
    tos 0xc0, ttl 64, length 348, checksum 0x78d2
    fragment id 0x0000
  UDP: 68 -> 67
    length 328, checksum 0xc3b2
00:27:21:229395: ip4-lookup
  fib 0 dpo-idx 0 flow hash: 0x00000000
  UDP: 0.0.0.0 -> 255.255.255.255
    tos 0xc0, ttl 64, length 348, checksum 0x78d2
    fragment id 0x0000
  UDP: 68 -> 67
    length 328, checksum 0xc3b2
00:27:21:229403: ip4-drop
    UDP: 0.0.0.0 -> 255.255.255.255
      tos 0xc0, ttl 64, length 348, checksum 0x78d2
      fragment id 0x0000
    UDP: 68 -> 67
      length 328, checksum 0xc3b2
00:27:21:229407: error-drop
  rx:VirtualEthernet0/0/0
00:27:21:229411: drop
  null-node: blackholed packets







策略路由的报文流程

00:20:06:422427: virtio-input
  virtio: hw_if_index 1 next-index 4 vring 0 len 98
    hdr: flags 0x00 gso_type 0x00 hdr_len 0 gso_size 0 csum_start 0 csum_offset 0 num_buffers 1
00:20:06:422440: ethernet-input
  IP4: 02:fe:a6:d9:e3:77 -> 02:fe:ae:f7:cb:61
00:20:06:422446: ip4-input
  ICMP: 192.168.10.2 -> 192.168.57.2
    tos 0x00, ttl 64, length 84, checksum 0x61c9
    fragment id 0x148b, flags DONT_FRAGMENT
  ICMP echo_request checksum 0x9a58
00:20:06:422454: abf-input-ip4
   next 1 index 19
00:20:06:422462: ip4-load-balance
  fib 0 dpo-idx 4 flow hash: 0x00000000
  ICMP: 192.168.10.2 -> 192.168.57.2
    tos 0x00, ttl 64, length 84, checksum 0x61c9
    fragment id 0x148b, flags DONT_FRAGMENT
  ICMP echo_request checksum 0x9a58
00:20:06:422470: ip4-rewrite
  tx_sw_if_index 4 dpo-idx 4 : ipv4 via 192.168.57.3 loop0: mtu:9000 000000000002dead000000000800 flow hash: 0x00000000
  00000000: 000000000002dead00000000080045000054148b40003f0162c9c0a80a02c0a8
  00000020: 390208009a5810b101d49d37f85e00000000f6b80000000000001011
00:20:06:422475: loop0-output
  loop0 l2_hdr_offset_valid l3_hdr_offset_valid 
  IP4: de:ad:00:00:00:00 -> 00:00:00:00:00:02
  ICMP: 192.168.10.2 -> 192.168.57.2
    tos 0x00, ttl 63, length 84, checksum 0x62c9
    fragment id 0x148b, flags DONT_FRAGMENT
  ICMP echo_request checksum 0x9a58
00:20:06:422484: l2-input
  l2-input: sw_if_index 4 dst 00:00:00:00:00:02 src de:ad:00:00:00:00
00:20:06:422487: l2-fwd
  l2-fwd:   sw_if_index 4 dst 00:00:00:00:00:02 src de:ad:00:00:00:00 bd_index 1 result [0x1050000000003, 3] none
00:20:06:422491: l2-output
  l2-output: sw_if_index 3 dst 00:00:00:00:00:02 src de:ad:00:00:00:00 data 08 00 45 00 00 54 14 8b 40 00 3f 01
00:20:06:422494: VirtualEthernet0/0/1-output
  VirtualEthernet0/0/1 l2_hdr_offset_valid l3_hdr_offset_valid 
  IP4: de:ad:00:00:00:00 -> 00:00:00:00:00:02
  ICMP: 192.168.10.2 -> 192.168.57.2
    tos 0x00, ttl 63, length 84, checksum 0x62c9
    fragment id 0x148b, flags DONT_FRAGMENT
  ICMP echo_request checksum 0x9a58
00:20:06:422496: VirtualEthernet0/0/1-tx
     VirtualEthernet0/0/1 queue 0
   virtio flags:
    SINGLE_DESC Single descriptor packet
   virtio_net_hdr first_desc_len 1536
     flags 0x00 gso_type 0
     num_buff 1





/*二层构造报文*/

00:03:14:856674: virtio-input
  virtio: hw_if_index 3 next-index 4 vring 0 len 60
    hdr: flags 0x00 gso_type 0x00 hdr_len 0 gso_size 0 csum_start 0 csum_offset 0 num_buffers 1
00:03:14:856709: ethernet-input
  IP4: 00:00:00:00:00:02 -> 02:fe:3c:21:b6:84
00:03:14:856715: l2-input
  l2-input: sw_if_index 3 dst 02:fe:3c:21:b6:84 src 00:00:00:00:00:02
00:03:14:856720: l2-fwd
  l2-fwd:   sw_if_index 3 dst 02:fe:3c:21:b6:84 src 00:00:00:00:00:02 bd_index 1 result [0xffffffffffffffff, -1] static age-not bvi filter learn-event learn-move 
00:03:14:856724: l2-flood
  l2-flood: sw_if_index 3 dst 02:fe:3c:21:b6:84 src 00:00:00:00:00:02 bd_index 1
  l2-flood: sw_if_index 3 dst 02:fe:3c:21:b6:84 src 00:00:00:00:00:02 bd_index 1
00:03:14:856732: l2-output
  l2-output: sw_if_index 2 dst 02:fe:3c:21:b6:84 src 00:00:00:00:00:02 data 08 00 45 00 00 1c 12 34 40 00 ff 01
  l2-output: sw_if_index 1 dst 02:fe:3c:21:b6:84 src 00:00:00:00:00:02 data 08 00 45 00 00 1c 12 34 40 00 ff 01
00:03:14:856737: VirtualEthernet0/0/1-output
  VirtualEthernet0/0/1 l2_hdr_offset_valid l3_hdr_offset_valid 
  IP4: 00:00:00:00:00:02 -> 02:fe:3c:21:b6:84
  ICMP: 192.168.10.23 -> 192.168.57.3
    tos 0x00, ttl 255, length 28, checksum 0xa541
    fragment id 0x1234, flags DONT_FRAGMENT
  ICMP echo_request checksum 0xf7fd
00:03:14:856751: VirtualEthernet0/0/0-output
  VirtualEthernet0/0/0 l2_hdr_offset_valid l3_hdr_offset_valid 
  IP4: 00:00:00:00:00:02 -> 02:fe:3c:21:b6:84
  ICMP: 192.168.10.23 -> 192.168.57.3
    tos 0x00, ttl 255, length 28, checksum 0xa541
    fragment id 0x1234, flags DONT_FRAGMENT
  ICMP echo_request checksum 0xf7fd
00:03:14:856753: VirtualEthernet0/0/1-tx
     VirtualEthernet0/0/1 queue 0
   virtio flags:
    SINGLE_DESC Single descriptor packet
   virtio_net_hdr first_desc_len 1536
     flags 0x00 gso_type 0
     num_buff 1
00:03:14:856758: VirtualEthernet0/0/0-tx
     VirtualEthernet0/0/0 queue 0
   virtio flags:
    SINGLE_DESC Single descriptor packet
   virtio_net_hdr first_desc_len 1536
     flags 0x00 gso_type 0
     num_buff 1









![Contiv / VPP架构](https://github.com/contiv/vpp/raw/master/docs/img/contiv-arch.png)



https://docs.ligato.io/en/latest/intro/overview/

![agent-vpp](C:\Users\Jerry\Documents\agent-vpp.png)

![ligato框架](https://docs.ligato.io/en/latest/img/intro/ligato-framework-arch2.svg)



![数据同步](https://docs.ligato.io/en/latest/img/user-guide/datasync_watch.png)

参考风格

https://developer.aliyun.com/article/610476

vpp中的acl实现



先了解几个关键的结构体

acl_rule_t  基础的rule规则数据单元，一个acl可以由多个rule组成；

acl_list_t  vec指针，指向该acl的所有rule；

hash_acl_info_t   存放所引用的lc_index表，hash_ace_info_t



acl的规则下发，主要函数

基础函数

hash_acl_apply(acl_main_t *am, u32 lc_index, int acl_index, u32 acl_position)  acl_position：acl的排序

hash_acl_unapply(acl_main_t *am, u32 lc_index, int acl_index)





一条线是acl模块内部的acl增加修改。

acl_add_list--->acl_plugin_lookup_context_notify_acl_change--->hash_acl_add--->hash_acl_reapply-->hash_acl_unapply

​                                                                                                                                                                                  **hash_acl_apply**

另一条是acl lib供外部的模块调用，只支持全部刷新。

acl_plugin_set_acl_vec_for_context--->apply_acl_vec--->**hash_acl_apply**

​                                                             ---->unapply_acl_vec--->hash_acl_unapply







ACL	匹配支持两种方式：

普通的遍历：拿acl rule表项一个一个和报文if；

HASH遍历： 报文做hash，再冲突检测找出acl rule；遍历前先对报文掩码处理，有几种掩码规格就需要hash几次；



multi_acl_match_get_applied_ace_index  

数据来的时候，查找按照am->hash_applied_mask_info_vec_by_lc_index，生效掩码表项处理。循环hash









疑问关于数据直接操作移动和增加不考虑，线程间的问题，

目前有个解决的希望

acl_set_heap







vlib_process_signal_event_data





vl_api_clnt_process



VLIB_API_INIT_FUNCTION (interface_api_hookup);





新的cli new session的处理



vlib_process_wait_for_event



(gdb) bt

#0  vlib_process_wait_for_event (vm=0x7ffff639c980 <vlib_global_main>) at /home/test/vpp/src/vlib/node_funcs.h:595
#1  0x00007ffff61601d9 in unix_cli_new_session_process (vm=0x7ffff639c980 <vlib_global_main>, rt=0x7fffb5acb000, f=0x0) at /home/test/vpp/src/vlib/unix/cli.c:1283
#2  0x00007ffff60e3624 in vlib_process_bootstrap (_a=140736223297976) at /home/test/vpp/src/vlib/main.c:1502
#3  0x00007ffff5532964 in clib_calljmp () at /home/test/vpp/src/vppinfra/longjmp.S:123
#4  0x00007fffb498c1b0 in ?? ()
#5  0x00007ffff60e31c1 in vlib_process_startup (vm=0x4, p=0x7fffb5a87f80, f=0xe) at /home/test/vpp/src/vlib/main.c:1524
Backtrace stopped: previous frame inner to this frame (corrupt stack?)

Breakpoint 2 at 0x7fffb1279123: vlib_process_get_events. (30 locations)





vlib_process_get_events

#0  vlib_process_get_events (vm=0x7ffff639c980 <vlib_global_main>, data_vector=0x7fffb5387f48) at /home/test/vpp/src/vlib/node_funcs.h:518
#1  0x00007ffff6f8f986 in bfd_process (vm=0x7ffff639c980 <vlib_global_main>, rt=0x7fffb537f000, f=0x0) at /home/test/vpp/src/vnet/bfd/bfd_main.c:1195
#2  0x00007ffff60e3624 in vlib_process_bootstrap (_a=140736223299640) at /home/test/vpp/src/vlib/main.c:1502
#3  0x00007ffff5532964 in clib_calljmp () at /home/test/vpp/src/vppinfra/longjmp.S:123
#4  0x00007fffb498c830 in ?? ()
#5  0x00007ffff60e31c1 in vlib_process_startup (vm=0x11600000000, p=0x77fae775eca60, f=0x0) at /home/test/vpp/src/vlib/main.c:1524



vlib_register_node

#0  vlib_register_node (vm=0x7ffff639c980 <vlib_global_main>, r=0x7fffb498c9d0) at /home/test/vpp/src/vlib/node.c:530
#1  0x00007ffff6b48a56 in vnet_register_interface (vnm=0x7ffff7b52fb8 <vnet_main>, dev_class_index=23, dev_instance=0, hw_class_index=22, hw_instance=0) at /home/test/vpp/src/vnet/interface.c:911
#2  0x00007ffff6b8e2cb in vnet_main_init (vm=0x7ffff639c980 <vlib_global_main>) at /home/test/vpp/src/vnet/misc.c:81
#3  0x00007ffff60bb006 in call_init_exit_functions_internal (vm=0x7ffff639c980 <vlib_global_main>, headp=0x7ffff639cfa8 <vlib_global_main+1576>, call_once=1, do_sort=1) at /home/test/vpp/src/vlib/init.c:350
#4  0x00007ffff60baf28 in vlib_call_init_exit_functions (vm=0x7ffff639c980 <vlib_global_main>, headp=0x7ffff639cfa8 <vlib_global_main+1576>, call_once=1) at /home/test/vpp/src/vlib/init.c:364
#5  0x00007ffff60bb0b1 in vlib_call_all_init_functions (vm=0x7ffff639c980 <vlib_global_main>) at /home/test/vpp/src/vlib/init.c:386
#6  0x00007ffff60e0298 in vlib_main (vm=0x7ffff639c980 <vlib_global_main>, input=0x7fffb498cfa8) at /home/test/vpp/src/vlib/main.c:2171
#7  0x00007ffff61678f5 in thread0 (arg=140737324370304) at /home/test/vpp/src/vlib/unix/main.c:658
#8  0x00007ffff5532964 in clib_calljmp () at /home/test/vpp/src/vppinfra/longjmp.S:123
#9  0x00007fffffffcf30 in ?? ()
#10 0x00007ffff6167487 in vlib_unix_main (argc=84, argv=0x722770) at /home/test/vpp/src/vlib/unix/main.c:730
#11 0x0000000000406888 in main (argc=84, argv=0x722770) at /home/test/vpp/src/vpp/vnet/main.c:291



dispatch_process

#0  dispatch_process (vm=0x7ffff639c980 <vlib_global_main>, p=0x7fffb5302000, f=0x0, last_time_stamp=2151866632861440) at /home/test/vpp/src/vlib/main.c:1547
#1  0x00007ffff60de878 in vlib_main_or_worker_loop (vm=0x7ffff639c980 <vlib_global_main>, is_main=1) at /home/test/vpp/src/vlib/main.c:1764
#2  0x00007ffff60e0c6a in vlib_main_loop (vm=0x7ffff639c980 <vlib_global_main>) at /home/test/vpp/src/vlib/main.c:1990
#3  0x00007ffff60e0a3f in vlib_main (vm=0x7ffff639c980 <vlib_global_main>, input=0x7fffb498cfa8) at /home/test/vpp/src/vlib/main.c:2236
#4  0x00007ffff61678f5 in thread0 (arg=140737324370304) at /home/test/vpp/src/vlib/unix/main.c:658
#5  0x00007ffff5532964 in clib_calljmp () at /home/test/vpp/src/vppinfra/longjmp.S:123
#6  0x00007fffffffcf30 in ?? ()
#7  0x00007ffff6167487 in vlib_unix_main (argc=84, argv=0x722770) at /home/test/vpp/src/vlib/unix/main.c:730
#8  0x0000000000406888 in main (argc=84, argv=0x722770) at /home/test/vpp/src/vpp/vnet/main.c:291







命令行的调用





#0  vlib_start_process (vm=0x7ffff639c980 <vlib_global_main>, process_index=37) at /home/test/vpp/src/vlib/main.c:1619
#1  0x00007ffff614a07b in unix_cli_file_add (cm=0x7ffff639d2f8 <unix_cli_main>, name=0x0, fd=34) at /home/test/vpp/src/vlib/unix/cli.c:2900
#2  0x00007ffff614ab2b in unix_cli_listen_read_ready (uf=0x7fffb5780e48) at /home/test/vpp/src/vlib/unix/cli.c:2935
#3  0x00007ffff6165050 in linux_epoll_input_inline (vm=0x7ffff639c980 <vlib_global_main>, node=0x7fffb53399c0, frame=0x0, thread_index=0) at /home/test/vpp/src/vlib/unix/input.c:314
#4  0x00007ffff61646e9 in linux_epoll_input (vm=0x7ffff639c980 <vlib_global_main>, node=0x7fffb53399c0, frame=0x0) at /home/test/vpp/src/vlib/unix/input.c:364
#5  0x00007ffff60e41f5 in dispatch_node (vm=0x7ffff639c980 <vlib_global_main>, node=0x7fffb53399c0, type=VLIB_NODE_TYPE_PRE_INPUT, dispatch_state=VLIB_NODE_STATE_POLLING, frame=0x0, last_time_stamp=2153111643587460)
    at /home/test/vpp/src/vlib/main.c:1235
#6  0x00007ffff60debd2 in vlib_main_or_worker_loop (vm=0x7ffff639c980 <vlib_global_main>, is_main=1) at /home/test/vpp/src/vlib/main.c:1807
#7  0x00007ffff60e0c6a in vlib_main_loop (vm=0x7ffff639c980 <vlib_global_main>) at /home/test/vpp/src/vlib/main.c:1990
#8  0x00007ffff60e0a3f in vlib_main (vm=0x7ffff639c980 <vlib_global_main>, input=0x7fffb498cfa8) at /home/test/vpp/src/vlib/main.c:2236
#9  0x00007ffff61678f5 in thread0 (arg=140737324370304) at /home/test/vpp/src/vlib/unix/main.c:658
#10 0x00007ffff5532964 in clib_calljmp () at /home/test/vpp/src/vppinfra/longjmp.S:123
#11 0x00007fffffffcf30 in ?? ()
#12 0x00007ffff6167487 in vlib_unix_main (argc=84, argv=0x722770) at /home/test/vpp/src/vlib/unix/main.c:730
#13 0x0000000000406888 in main (argc=84, argv=0x722770) at /home/test/vpp/src/vpp/vnet/main.c:291
(gdb) c
Continuing.

Thread 1 "vpp_main" hit Breakpoint 4, vlib_start_process (vm=0x7ffff639c980 <vlib_global_main>, process_index=38) at /home/test/vpp/src/vlib/main.c:1619
1619      vlib_node_main_t *nm = &vm->node_main;
(gdb) bt
#0  vlib_start_process (vm=0x7ffff639c980 <vlib_global_main>, process_index=38) at /home/test/vpp/src/vlib/main.c:1619
#1  0x00007ffff60f4f72 in vlib_process_create (vm=0x7ffff639c980 <vlib_global_main>, name=0x7ffff6180841 "unix-cli-new-session", f=0x7ffff6160140 <unix_cli_new_session_process>, log2_n_stack_bytes=16)
    at /home/test/vpp/src/vlib/node.c:791
#2  0x00007ffff614af6b in unix_cli_listen_read_ready (uf=0x7fffb5780ee8) at /home/test/vpp/src/vlib/unix/cli.c:2994
#3  0x00007ffff6165050 in linux_epoll_input_inline (vm=0x7ffff639c980 <vlib_global_main>, node=0x7fffb53399c0, frame=0x0, thread_index=0) at /home/test/vpp/src/vlib/unix/input.c:314
#4  0x00007ffff61646e9 in linux_epoll_input (vm=0x7ffff639c980 <vlib_global_main>, node=0x7fffb53399c0, frame=0x0) at /home/test/vpp/src/vlib/unix/input.c:364
#5  0x00007ffff60e41f5 in dispatch_node (vm=0x7ffff639c980 <vlib_global_main>, node=0x7fffb53399c0, type=VLIB_NODE_TYPE_PRE_INPUT, dispatch_state=VLIB_NODE_STATE_POLLING, frame=0x0, last_time_stamp=2153111643587460)
    at /home/test/vpp/src/vlib/main.c:1235
#6  0x00007ffff60debd2 in vlib_main_or_worker_loop (vm=0x7ffff639c980 <vlib_global_main>, is_main=1) at /home/test/vpp/src/vlib/main.c:1807
#7  0x00007ffff60e0c6a in vlib_main_loop (vm=0x7ffff639c980 <vlib_global_main>) at /home/test/vpp/src/vlib/main.c:1990
#8  0x00007ffff60e0a3f in vlib_main (vm=0x7ffff639c980 <vlib_global_main>, input=0x7fffb498cfa8) at /home/test/vpp/src/vlib/main.c:2236
#9  0x00007ffff61678f5 in thread0 (arg=140737324370304) at /home/test/vpp/src/vlib/unix/main.c:658
#10 0x00007ffff5532964 in clib_calljmp () at /home/test/vpp/src/vppinfra/longjmp.S:123
#11 0x00007fffffffcf30 in ?? ()
#12 0x00007ffff6167487 in vlib_unix_main (argc=84, argv=0x722770) at /home/test/vpp/src/vlib/unix/main.c:730
#13 0x0000000000406888 in main (argc=84, argv=0x722770) at /home/test/vpp/src/vpp/vnet/main.c:291







#0  vlib_start_process (vm=0x7ffff639c980 <vlib_global_main>, process_index=37) at /home/test/vpp/src/vlib/main.c:1619
#1  0x00007ffff614a07b in unix_cli_file_add (cm=0x7ffff639d2f8 <unix_cli_main>, name=0x7fffb5adca50 "unix-cli-local:1", fd=34) at /home/test/vpp/src/vlib/unix/cli.c:2900
#2  0x00007ffff614ab2b in unix_cli_listen_read_ready (uf=0x7fffb5780e48) at /home/test/vpp/src/vlib/unix/cli.c:2935
#3  0x00007ffff6165050 in linux_epoll_input_inline (vm=0x7ffff639c980 <vlib_global_main>, node=0x7fffb53399c0, frame=0x0, thread_index=0) at /home/test/vpp/src/vlib/unix/input.c:314
#4  0x00007ffff61646e9 in linux_epoll_input (vm=0x7ffff639c980 <vlib_global_main>, node=0x7fffb53399c0, frame=0x0) at /home/test/vpp/src/vlib/unix/input.c:364
#5  0x00007ffff60e41f5 in dispatch_node (vm=0x7ffff639c980 <vlib_global_main>, node=0x7fffb53399c0, type=VLIB_NODE_TYPE_PRE_INPUT, dispatch_state=VLIB_NODE_STATE_POLLING, frame=0x0, last_time_stamp=2156789178422128)
    at /home/test/vpp/src/vlib/main.c:1235
#6  0x00007ffff60debd2 in vlib_main_or_worker_loop (vm=0x7ffff639c980 <vlib_global_main>, is_main=1) at /home/test/vpp/src/vlib/main.c:1807
#7  0x00007ffff60e0c6a in vlib_main_loop (vm=0x7ffff639c980 <vlib_global_main>) at /home/test/vpp/src/vlib/main.c:1990
#8  0x00007ffff60e0a3f in vlib_main (vm=0x7ffff639c980 <vlib_global_main>, input=0x7fffb498cfa8) at /home/test/vpp/src/vlib/main.c:2236
#9  0x00007ffff61678f5 in thread0 (arg=140737324370304) at /home/test/vpp/src/vlib/unix/main.c:658
#10 0x00007ffff5532964 in clib_calljmp () at /home/test/vpp/src/vppinfra/longjmp.S:123
#11 0x00007fffffffcf30 in ?? ()
#12 0x00007ffff6167487 in vlib_unix_main (argc=84, argv=0x722770) at /home/test/vpp/src/vlib/unix/main.c:730
#13 0x0000000000406888 in main (argc=84, argv=0x722770) at /home/test/vpp/src/vpp/vnet/main.c:291











Thread 1 "vpp_main" hit Breakpoint 9, linux_epoll_input_inline (vm=0x7ffff639c980 <vlib_global_main>, node=0x7fffb53399c0, frame=0x0, thread_index=0) at /home/test/vpp/src/vlib/unix/input.c:137
137       unix_main_t *um = &unix_main;
(gdb) bt
#0  linux_epoll_input_inline (vm=0x7ffff639c980 <vlib_global_main>, node=0x7fffb53399c0, frame=0x0, thread_index=0) at /home/test/vpp/src/vlib/unix/input.c:137
#1  0x00007ffff61646e9 in linux_epoll_input (vm=0x7ffff639c980 <vlib_global_main>, node=0x7fffb53399c0, frame=0x0) at /home/test/vpp/src/vlib/unix/input.c:364
#2  0x00007ffff60e41f5 in dispatch_node (vm=0x7ffff639c980 <vlib_global_main>, node=0x7fffb53399c0, type=VLIB_NODE_TYPE_PRE_INPUT, dispatch_state=VLIB_NODE_STATE_POLLING, frame=0x0, last_time_stamp=2157513659652028)
    at /home/test/vpp/src/vlib/main.c:1235
#3  0x00007ffff60debd2 in vlib_main_or_worker_loop (vm=0x7ffff639c980 <vlib_global_main>, is_main=1) at /home/test/vpp/src/vlib/main.c:1807
#4  0x00007ffff60e0c6a in vlib_main_loop (vm=0x7ffff639c980 <vlib_global_main>) at /home/test/vpp/src/vlib/main.c:1990
#5  0x00007ffff60e0a3f in vlib_main (vm=0x7ffff639c980 <vlib_global_main>, input=0x7fffb498cfa8) at /home/test/vpp/src/vlib/main.c:2236
#6  0x00007ffff61678f5 in thread0 (arg=140737324370304) at /home/test/vpp/src/vlib/unix/main.c:658
#7  0x00007ffff5532964 in clib_calljmp () at /home/test/vpp/src/vppinfra/longjmp.S:123
#8  0x00007fffffffcf30 in ?? ()
#9  0x00007ffff6167487 in vlib_unix_main (argc=84, argv=0x722770) at /home/test/vpp/src/vlib/unix/main.c:730
#10 0x0000000000406888 in main (argc=84, argv=0x722770) at /home/test/vpp/src/vpp/vnet/main.c:291





vpp 注册事件

vl_msg_api_config





vpp-bihash实现分析



bihash是vpp里面少有的保证线程安全的数据结构，acl和l2-fib都用到了bihash（但不是表明acl和l2-fib命令都是线程安全的）。

```
typedef struct
{
  union
  {
    struct
    {
      u64 offset:BIHASH_BUCKET_OFFSET_BITS;
      u64 lock:1;
      u64 linear_search:1;
      u64 log2_pages:8;
      u64 refcnt:16;
    };
    u64 as_u64;
  };
} BVT (clib_bihash_bucket);
```

offseet共有36bit, 能寻址64G bytes的内存空间。这里使用offset而不是一个明确的指针，是因为BIHASH初始时直接从系统获取一块大内存，然后在此基础上再分配。这里的offset指的是相对于大内存首地址的偏移。用来找到 BVT (clib_bihash_value)

lock 主要用于多线程对bucket的互斥访问。

log2_pages主要代表当前hash槽有几页( 1 << log2_pages) 数据。 即offset指向的内存的动态数组的大小( 如log2_pages为4时，此hash槽有16页）。





key-value-pair，HASH表项，数据单元有键值对组成，vpp中

```
typedef struct
{
  u64 key[6];
  u64 value;
} clib_bihash_kv_48_8_t;
```

hash bucket. HASH槽。主要用来组织具有相同hash值的hash表项，通过这个hash槽，可以找到此hash值的所有表项。hash槽，也称为hash冲突链。逻辑上，可以看成具有相同hash值的表项链接在一起。



hash buckets. HASH桶，由多个HASH槽组成。一般定义成一个数组，数组下标即为对应hash槽的HASH值。



```
BVS (clib_bihash)
{
  BVT (clib_bihash_bucket) * buckets;
  volatile u32 *alloc_lock;

  BVT (clib_bihash_value) ** working_copies;
  int *working_copy_lengths;
  BVT (clib_bihash_bucket) saved_bucket;

  u32 nbuckets;
  u32 log2_nbuckets;
  u64 memory_size;
  u8 *name;

  u64 *freelists;

#if BIHASH_32_64_SVM
  BVT (clib_bihash_shared_header) * sh;
  int memfd;
#else
  BVT (clib_bihash_shared_header) sh;
#endif

  u64 alloc_arena;		/* Base of the allocation arena */
  volatile u8 instantiated;

  /**
    * A custom format function to print the Key and Value of bihash_key instead of default hexdump
    */
  format_function_t *fmt_fn;

  /** Optional statistics-gathering callback */
#if BIHASH_ENABLE_STATS
  void (*inc_stats_callback) (BVS (clib_bihash) *, int stat_id, u64 count);

  /** Statistics callback context (e.g. address of stats data structure) */
  void *inc_stats_context;
#endif

} BVT (clib_bihash);
```

每个模块调用BV (clib_bihash_init)会初始化hash表主结构放到clib_all_bihashes（全局数组指针）；

buckets  存放的桶内存由mmap方式申请，

nbuckets，总共的hash槽数，在创建hash表时，调整成2的幂指数形式。如创建hash表时，需要创建含500个hash槽的hash表，则软件调整成含512( 1 << 9) 个hash槽的hash表。这样做的目的是提高寻找hash槽的效率(用位运算&而不是%运算就可以定位hash槽)。在查找时，hash值的低log2_nbuckets bit用来索引桶号， offset = (hash & (h->nbuckets - 1));



alloc_arena 指的是大内存块的首地址，和buckets 的区分出来，是分别申请



```
typedef struct
{
  BVT (clib_bihash) * h;
  char *name;
  u32 nbuckets;
  uword memory_size;
  format_function_t *fmt_fn;
  u8 instantiate_immediately;
  u8 dont_add_to_all_bihash_list;
} BVT (clib_bihash_init2_args);
```



这个结构体是BV (clib_bihash_init)初始化用到的







foreach_udp4_dst_port 定义udp端口



nsh_add_del_proxy_session（）添加nsh-proxy node，这个是在判断封装是vxlan情况下自动添加proxy 



###### vpp 2层flood



classify_and_dispatch（）在这个函数实现逻辑，

feat_mask = feat_mask & bd_config->feature_bitmap; 以桥的配置优先。

如果桥为disable则肯定关闭，

桥为enable，不一样enable（具体分情况，看代码）

#，关闭flood功能，是指广播泛洪，有二层bridge的情况下，只需要关闭bridge

set interface l2 flood VirtualEthernet0/0/0 disable

set  bridge-domain flood 2 disable

#，关闭uu-flood功能，是指未知单播泛洪

set bridge-domain uu-flood



###### vpp,nsh处理

Packet 1

00:31:30:071031: virtio-input
  virtio: hw_if_index 2 next-index 4 vring 0 len 34
    hdr: flags 0x00 gso_type 0x00 hdr_len 0 gso_size 0 csum_start 0 csum_offset 0 num_buffers 1
00:31:30:071052: ethernet-input
  IP4: 82:1f:cc:9d:e4:19 -> 1a:2b:3c:4d:5e:6f
00:31:30:071072: l2-input
  l2-input: sw_if_index 2 dst 1a:2b:3c:4d:5e:6f src 82:1f:cc:9d:e4:19
00:31:30:071083: l2-input-classify
  l2-classify: sw_if_index 2, table 2, offset 4b0, next 27
00:31:30:071096: nsh-classifier

  nsh ver 0 C-set ttl 40 len 43 (172 bytes) md_type 60 next_protocol 77
  service path 6188930 service index 31

00:31:30:071117: vxlan-gpe-encap
  VXLAN-GPE-ENCAP: tunnel 0
00:31:30:071133: ip4-lookup
  fib 0 dpo-idx 1 flow hash: 0x00000000
  UDP: 10.10.10.1 -> 10.10.10.3
    tos 0x00, ttl 254, length 70, checksum 0x948f dscp CS0 ecn NON_ECN
    fragment id 0x0000
  UDP: 4790 -> 4790
    length 50, checksum 0x0000
00:31:30:071160: ip4-rewrite
  tx_sw_if_index 4 dpo-idx 1 : ipv4 via 10.10.10.3 loop0: mtu:9000 next:3 02fe6bd556121a2b3c4d5e6f0800 flow hash: 0x00000000
  00000000: 02fe6bd556121a2b3c4d5e6f08004500004600000000fd11958f0a0a0a010a0a
  00000020: 0a0312b612b6003200000c000004000002001a2b3c4d5e6f821fcc9d
00:31:30:071171: loop0-output
  loop0 
  IP4: 1a:2b:3c:4d:5e:6f -> 02:fe:6b:d5:56:12
  UDP: 10.10.10.1 -> 10.10.10.3
    tos 0x00, ttl 253, length 70, checksum 0x958f dscp CS0 ecn NON_ECN
    fragment id 0x0000
  UDP: 4790 -> 4790
    length 50, checksum 0x0000
00:31:30:071187: l2-input
  l2-input: sw_if_index 4 dst 02:fe:6b:d5:56:12 src 1a:2b:3c:4d:5e:6f
00:31:30:071189: l2-fwd
  l2-fwd:   sw_if_index 4 dst 02:fe:6b:d5:56:12 src 1a:2b:3c:4d:5e:6f bd_index 1 result [0x1000000000002, 2] none
00:31:30:071197: l2-output
  l2-output: sw_if_index 2 dst 02:fe:6b:d5:56:12 src 1a:2b:3c:4d:5e:6f data 08 00 45 00 00 46 00 00 00 00 fd 11
00:31:30:071204: tap0-output
  tap0 
  IP4: 1a:2b:3c:4d:5e:6f -> 02:fe:6b:d5:56:12
  UDP: 10.10.10.1 -> 10.10.10.3
    tos 0x00, ttl 253, length 70, checksum 0x958f dscp CS0 ecn NON_ECN
    fragment id 0x0000
  UDP: 4790 -> 4790
    length 50, checksum 0x0000



vxlan-input 解封后的处理

 vxlan_gpe_register_decap_protocol (VXLAN_GPE_PROTOCOL_NSH, next_node);



###### 接口相关

ethernet_register_input_type  



vnet_sw_interface_t *sw;

vnet_hw_interface_t *hw;







###### vhost代码阅读

vrings数组  

奇odd是接收

偶even是发送



设置接收关键函数

vnet_hw_interface_set_rx_mode

跟线程有关系

vhost_user_rx_thread_placement



DBGvpp# show vhost-user 
Virtio vhost-user interfaces
Global:
  coalesce frames 32 time 1e-3
  Number of rx virtqueues in interrupt mode: 0
  Number of GSO interfaces: 0
Interface: VirtualEthernet0/0/0 (ifindex 3)
virtio_net_hdr_sz 12
 features mask (0xfffffffbffffa27c): 
 features (0x150600000): 
   VIRTIO_NET_F_GUEST_ANNOUNCE (21)
   VIRTIO_NET_F_MQ (22)
   VIRTIO_F_INDIRECT_DESC (28)
   VHOST_USER_F_PROTOCOL_FEATURES (30)
   VIRTIO_F_VERSION_1 (32)
  protocol features (0x3)
   VHOST_USER_PROTOCOL_F_MQ (0)
   VHOST_USER_PROTOCOL_F_LOG_SHMFD (1)

 socket filename /home/test/vhost1.sock type server errno "Success"

 rx placement: 
   thread 0 on vring 1, polling
   thread 0 on vring 3, polling
   thread 0 on vring 5, polling
   thread 0 on vring 7, polling
 tx placement: lock-free
   thread 0 on vring 0

 Memory regions (total 3)
 region fd    guest_phys_addr    memory_size        userspace_addr     mmap_offset        mmap_addr
 ====== ===== ================== ================== ================== ================== ==================
  0     56    0x0000000000000000 0x00000000000a0000 0x00007f19b7000000 0x0000000000000000 0x00007fff81400000
  1     57    0x00000000000c0000 0x00000000bff40000 0x00007f19b70c0000 0x00000000000c0000 0x00007fa4ff0c0000
  2     58    0x0000000100000000 0x0000000040000000 0x00007f1a77000000 0x00000000c0000000 0x00007fa4bf000000

 Virtqueue 0 (TX)
  qsz 1024 last_avail_idx 18 last_used_idx 18
  avail.flags 0 avail.idx 1024 used.flags 1 used.idx 18
  kickfd 59 callfd 60 errfd -1

 Virtqueue 1 (RX)
  qsz 256 last_avail_idx 47 last_used_idx 47
  avail.flags 0 avail.idx 47 used.flags 1 used.idx 47
  kickfd 46 callfd 61 errfd -1

 Virtqueue 2 (TX)
  qsz 1024 last_avail_idx 0 last_used_idx 0
  avail.flags 0 avail.idx 1024 used.flags 1 used.idx 0
  kickfd 47 callfd 62 errfd -1

 Virtqueue 3 (RX)
  qsz 256 last_avail_idx 62 last_used_idx 62
  avail.flags 0 avail.idx 62 used.flags 1 used.idx 62
  kickfd 48 callfd 63 errfd -1

 Virtqueue 4 (TX)
  qsz 1024 last_avail_idx 0 last_used_idx 0
  avail.flags 0 avail.idx 1024 used.flags 1 used.idx 0
  kickfd 49 callfd 64 errfd -1

 Virtqueue 5 (RX)
  qsz 256 last_avail_idx 52 last_used_idx 52
  avail.flags 0 avail.idx 52 used.flags 1 used.idx 52
  kickfd 50 callfd 65 errfd -1

 Virtqueue 6 (TX)
  qsz 1024 last_avail_idx 0 last_used_idx 0
  avail.flags 0 avail.idx 1024 used.flags 1 used.idx 0
  kickfd 67 callfd 68 errfd -1

 Virtqueue 7 (RX)
  qsz 256 last_avail_idx 32 last_used_idx 32
  avail.flags 0 avail.idx 32 used.flags 1 used.idx 32
  kickfd 52 callfd 69 errfd -1

 Virtqueue 8 (TX disabled)
  qsz 1024 last_avail_idx 0 last_used_idx 0
  avail.flags 0 avail.idx 1024 used.flags 1 used.idx 0
  kickfd 53 callfd 70 errfd -1

 Virtqueue 9 (RX disabled)
  qsz 256 last_avail_idx 0 last_used_idx 0
  avail.flags 0 avail.idx 0 used.flags 1 used.idx 0
  kickfd 54 callfd 71 errfd -1

log

if 3 msg VHOST_USER_GET_FEATURES - reply 0x000000015c628000
if 3 msg VHOST_USER_GET_PROTOCOL_FEATURES - reply 0x0000000000000003
if 3 msg VHOST_USER_SET_PROTOCOL_FEATURES features 0x0000000000000003
if 3 msg VHOST_USER_GET_QUEUE_NUM - reply 16
if 3 msg VHOST_USER_SET_OWNER   vhost协议中，内核模式此消息会触发创建vhost 内核线程
if 3 msg VHOST_USER_GET_FEATURES - reply 0x000000015c628000
if 3 msg VHOST_USER_SET_VRING_CALL 0
if 3 msg VHOST_USER_SET_VRING_CALL 1
if 3 msg VHOST_USER_SET_FEATURES features 0x0000000150208000
if 3 msg VHOST_USER_SET_MEM_TABLE nregions 3   设置mmap
map memory region 0 addr 0 len 0x200000 fd 48 mapped 0x7fff81400000 page_sz 0x200000
map memory region 1 addr 0 len 0xc0000000 fd 49 mapped 0x7fa4ff000000 page_sz 0x200000
map memory region 2 addr 0 len 0x100000000 fd 50 mapped 0x7fa3ff000000 page_sz 0x200000
if 3 msg VHOST_USER_SET_VRING_NUM idx 0 num 256   设置vring 大小
if 3 msg VHOST_USER_SET_VRING_BASE idx 0 num 0xa  设置vring，数据位置
if 3 msg VHOST_USER_SET_VRING_ADDR idx 0          设置vrings 地址
if 3 msg VHOST_USER_SET_VRING_KICK 0 
if 3 msg VHOST_USER_SET_VRING_CALL 0   添加vring的fd到 epoll
if 0 KICK queue 0
if 3 msg VHOST_USER_SET_VRING_NUM idx 1 num 256
if 3 msg VHOST_USER_SET_VRING_BASE idx 1 num 0x5b
if 3 msg VHOST_USER_SET_VRING_ADDR idx 1
if 3 msg VHOST_USER_SET_VRING_KICK 1
if 3 msg VHOST_USER_SET_VRING_CALL 1
if 0 KICK queue 1
interface 3 ready
if 3 VHOST_USER_SET_VRING_ENABLE: enable queue 0
if 3 VHOST_USER_SET_VRING_ENABLE: enable queue 1







发送的gdb



(gdb) bt
#0  vhost_user_log_dirty_pages_2 (vui=0x7f1ecfd903c0, addr=5274576320, len=12, is_host_address=1 '\001') at /home/test/vpp/src/vnet/devices/virtio/vhost_user_inline.h:178
#1  0x00007f1f10a10759 in vhost_user_tx_copy (vui=0x7f1ecfd903c0, cpy=0x7f1ecfd58e40, copy_len=2, map_hint=0x7f1ecebf46fc)
    at /home/test/vpp/src/vnet/devices/virtio/vhost_user_output.c:228
#2  0x00007f1f10a0de26 in vhost_user_device_class_tx_fn_hsw (vm=0x7f1f10604980 <vlib_global_main>, node=0x7f1ecfe4bf40, frame=0x7f1ecfda25c0)
    at /home/test/vpp/src/vnet/devices/virtio/vhost_user_output.c:1006
#3  0x00007f1f1034c1f5 in dispatch_node (vm=0x7f1f10604980 <vlib_global_main>, node=0x7f1ecfe4bf40, type=VLIB_NODE_TYPE_INTERNAL, dispatch_state=VLIB_NODE_STATE_POLLING, 
    frame=0x7f1ecfda25c0, last_time_stamp=12078039460343984) at /home/test/vpp/src/vlib/main.c:1235
#4  0x00007f1f1034cac7 in dispatch_pending_node (vm=0x7f1f10604980 <vlib_global_main>, pending_frame_index=7, last_time_stamp=12078039460343984)
    at /home/test/vpp/src/vlib/main.c:1403
#5  0x00007f1f10346f21 in vlib_main_or_worker_loop (vm=0x7f1f10604980 <vlib_global_main>, is_main=1) at /home/test/vpp/src/vlib/main.c:1862
#6  0x00007f1f10348c6a in vlib_main_loop (vm=0x7f1f10604980 <vlib_global_main>) at /home/test/vpp/src/vlib/main.c:1990
#7  0x00007f1f10348a3f in vlib_main (vm=0x7f1f10604980 <vlib_global_main>, input=0x7f1ecebf4fa8) at /home/test/vpp/src/vlib/main.c:2236
#8  0x00007f1f103cf8f5 in thread0 (arg=139771395459456) at /home/test/vpp/src/vlib/unix/main.c:658
#9  0x00007f1f0f79a964 in clib_calljmp () at /home/test/vpp/src/vppinfra/longjmp.S:123
#10 0x00007ffe999daef0 in ?? ()
#11 0x00007f1f103cf487 in vlib_unix_main (argc=89, argv=0x22a1500) at /home/test/vpp/src/vlib/unix/main.c:730
#12 0x0000000000406888 in main (argc=89, argv=0x22a1500) at /home/test/vpp/src/vpp/vnet/main.c:291
(gdb) c



接收

#0  vhost_user_input_copy (vui=0x7f1ecfd903c0, cpy=0x7f1ecfd58e40, copy_len=1, map_hint=0x7f1ecebf4708) at /home/test/vpp/src/vnet/devices/virtio/vhost_user_input.c:145
#1  0x00007f1f10a058e2 in vhost_user_if_input (vm=0x7f1f10604980 <vlib_global_main>, vum=0x7f1f11db2680 <vhost_user_main>, vui=0x7f1ecfd903c0, qid=0, node=0x7f1ecf594880, 
    mode=VNET_HW_INTERFACE_RX_MODE_POLLING, enable_csum=0 '\000') at /home/test/vpp/src/vnet/devices/virtio/vhost_user_input.c:732
#2  0x00007f1f10a0357f in vhost_user_input_node_fn_hsw (vm=0x7f1f10604980 <vlib_global_main>, node=0x7f1ecf594880, frame=0x0)
    at /home/test/vpp/src/vnet/devices/virtio/vhost_user_input.c:1449
#3  0x00007f1f1034c1f5 in dispatch_node (vm=0x7f1f10604980 <vlib_global_main>, node=0x7f1ecf594880, type=VLIB_NODE_TYPE_INPUT, dispatch_state=VLIB_NODE_STATE_POLLING, 
    frame=0x0, last_time_stamp=12080428594479180) at /home/test/vpp/src/vlib/main.c:1235
#4  0x00007f1f10346c95 in vlib_main_or_worker_loop (vm=0x7f1f10604980 <vlib_global_main>, is_main=1) at /home/test/vpp/src/vlib/main.c:1815
#5  0x00007f1f10348c6a in vlib_main_loop (vm=0x7f1f10604980 <vlib_global_main>) at /home/test/vpp/src/vlib/main.c:1990
#6  0x00007f1f10348a3f in vlib_main (vm=0x7f1f10604980 <vlib_global_main>, input=0x7f1ecebf4fa8) at /home/test/vpp/src/vlib/main.c:2236
#7  0x00007f1f103cf8f5 in thread0 (arg=139771395459456) at /home/test/vpp/src/vlib/unix/main.c:658
#8  0x00007f1f0f79a964 in clib_calljmp () at /home/test/vpp/src/vppinfra/longjmp.S:123
#9  0x00007ffe999daef0 in ?? ()
#10 0x00007f1f103cf487 in vlib_unix_main (argc=89, argv=0x22a1500) at /home/test/vpp/src/vlib/unix/main.c:730
#11 0x0000000000406888 in main (argc=89, argv=0x22a1500) at /home/test/vpp/src/vpp/vnet/main.c:291





region_mmap_addr







设置发送

#0  vhost_user_tx_thread_placement (vui=0x7f771e6c5440) at /home/test/vpp/src/vnet/devices/virtio/vhost_user.c:124
#1  0x00007f775ed98b21 in vhost_user_thread_placement (vui=0x7f771e6c5440, qid=0) at /home/test/vpp/src/vnet/devices/virtio/vhost_user.c:249
#2  0x00007f775ed98a74 in vhost_user_kickfd_read_ready (uf=0x7f771e1a1418) at /home/test/vpp/src/vnet/devices/virtio/vhost_user.c:270
#3  0x00007f775db94050 in linux_epoll_input_inline (vm=0x7f775ddcb980 <vlib_global_main>, node=0x7f771cdc4140, frame=0x0, thread_index=0)
    at /home/test/vpp/src/vlib/unix/input.c:314
#4  0x00007f775db936e9 in linux_epoll_input (vm=0x7f775ddcb980 <vlib_global_main>, node=0x7f771cdc4140, frame=0x0) at /home/test/vpp/src/vlib/unix/input.c:364
#5  0x00007f775db131f5 in dispatch_node (vm=0x7f775ddcb980 <vlib_global_main>, node=0x7f771cdc4140, type=VLIB_NODE_TYPE_PRE_INPUT, dispatch_state=VLIB_NODE_STATE_POLLING, 
    frame=0x0, last_time_stamp=12482898174075168) at /home/test/vpp/src/vlib/main.c:1235
#6  0x00007f775db0dbd2 in vlib_main_or_worker_loop (vm=0x7f775ddcb980 <vlib_global_main>, is_main=1) at /home/test/vpp/src/vlib/main.c:1807
#7  0x00007f775db0fc6a in vlib_main_loop (vm=0x7f775ddcb980 <vlib_global_main>) at /home/test/vpp/src/vlib/main.c:1990
#8  0x00007f775db0fa3f in vlib_main (vm=0x7f775ddcb980 <vlib_global_main>, input=0x7f771c3bbfa8) at /home/test/vpp/src/vlib/main.c:2236
#9  0x00007f775db968f5 in thread0 (arg=140150652582272) at /home/test/vpp/src/vlib/unix/main.c:658
#10 0x00007f775cf61964 in clib_calljmp () at /home/test/vpp/src/vppinfra/longjmp.S:123
#11 0x00007ffc50f62fb0 in ?? ()
#12 0x00007f775db96487 in vlib_unix_main (argc=91, argv=0x2253500) at /home/test/vpp/src/vlib/unix/main.c:730
#13 0x0000000000406888 in main (argc=91, argv=0x2253500) at /home/test/vpp/src/vpp/vnet/main.c:291



###### 发送函数触发中断有三处

vhost_user_send_call (vm, rxvq); 

input收报的时候每次都比一下是否需要send通知guest

output的时候，拼装完数据send通知guest

interrupt_process，协程用来一直判断通信





