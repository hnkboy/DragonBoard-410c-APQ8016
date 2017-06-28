#include <linux/init.h>  
#include <linux/kernel.h>  
#include <linux/module.h>  
MODULE_LICENSE("Dual BSD/GPL");   
static int __init test_init(void) {  
    printk("init module/n");  
    return 0;  
}  
static void __exit test_exit(void) {  
    printk("exit modules/n");  
}  
   
module_init(test_init);  
module_exit(test_exit);  
