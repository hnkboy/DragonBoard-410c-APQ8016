/*
============================================================================
Name : stree.c
Author : jiali
Version : 0.0.1
Date:  2017-06-27 
Copyright : Your copyright notice
Description : GPIO PWM MODULE
============================================================================
*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/sysfs.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/input.h>
#include <linux/jiffies.h>
#include <linux/mutex.h>
#include <linux/ctype.h>
#include <linux/pm_runtime.h>
#include <linux/device.h>
#include <linux/irq.h>
#include <linux/of_gpio.h>


#define DRIVER_NAME "stree"
#define COMPATIBLE  "thundersoft,stree"

#define PERIOD 20000 //PWM频率固定为50HZ（20毫秒一个周期,20000us）
//TBS2701舵机
#ifdef POWER_SWITCH
#define POWER_GPIO 
#endif
struct tbs2701_data{
	int pwm_enable; //舵机使能标志位
//	int poll_time;
	int pwm_gpio; //舵机角度控制pin
	#ifdef POWER_SWITCH
	int power_gpio;//供电控制
	#endif
	int period;//周期
	int high_us;//高电平脉宽，决定舵机转角，限制在0.5ms到2.5ms之间

	int angle;//舵机转角
    struct mutex data_lock;
    wait_queue_head_t data_queue;
	
	struct work_struct pwm_work;
	struct work_struct pwm_work_enable;
    bool turn_flag;
	//atomic_t counter;/* 一共经历了多少秒？(定义为原子量)*/
	struct timer_list s_timer; /*设备要使用的定时器*/ 
	int value_last;
};
struct tbs2701_data* tbs2701;
/*
============================================================================
Function:    static void pwm_work_func(struct work_struct* work)
Description: 执行初始化
Input: Output: Return: Others: 
============================================================================
*/
static void pwm_work_func(struct work_struct* work){
    //static void second_timer_handle(unsigned long arg){
	//struct timespec now,last;
	//struct tbs2701_data* data = container_of(timer,struct tbs2701_data,s_timer);//这种获取方式是否合理
	int high_us,low_us;
	int period;
	period = tbs2701->period;	
	if(!period||!tbs2701->pwm_enable){
		printk("we can't get period or pwm_enable is not set");
		return;
	}	
	/*high_us = data->high_us;
	
	if(high_us<=500)//限制在0.5ms到2.5ms之间
	{
		high_us=500;
	}
	else if(high_us>=2500)
	{
		high_us=2499;
	}	
	
	if(!period||!high_us)
	{
		printk("we can't period or high_ms");
		return;
	}	
	low_us = period-high_us;
    printk("### high_us = %d, low_us = %d \n ", high_us, low_us);*/
	printk("timer is enabling~~~!");
	//mutex_lock(&tbs2701->data_lock);
	if(tbs2701->pwm_enable){
        high_us = tbs2701->high_us;
		low_us = period-high_us;
		if(tbs2701->turn_flag==true){
			tbs2701->turn_flag=false;
			gpio_direction_output(tbs2701->pwm_gpio,1);
			//usleep(high_us);  //使用sleep的CPU占用率低于1%，但是实测波形极其不稳定,无法满足使用
			udelay(high_us); //理论上出来的波形更精准，但是CPU占用率达到25%，太耗资源
			//usleep_range(high_us,high_us);
		//	mod_timer(&tbs2701->s_timer,jiffies + 3*HZ);//重新设置定时周期,1000*HZ=1ms
		}
		else if(tbs2701->turn_flag==false){
			tbs2701->turn_flag=true;
			gpio_direction_output(tbs2701->pwm_gpio,0);
			// usleep(low_us);
			udelay(low_us);
			//usleep_range(low_us,low_us);
		//	mod_timer(&tbs2701->s_timer,jiffies + 6*HZ);//重新设置定时周期
		}
		
	}
	//mutex_unlock(&tbs2701->data_lock);
}	
/*
============================================================================
Function:    static int parse_dt(struct platform_device* pdev,struct tbs2701_data* data)
Description: 
Input: Output: Return: Others: 
============================================================================
*/
static int parse_dt(struct platform_device* pdev,struct tbs2701_data* data){
	//int rc;
	struct device_node* node = pdev->dev.of_node;
	/*rc = of_property_read_u32(node,"thunder,poll_time",&data->poll_time);
	if(rc){
		pr_warning("%s you should point time",__FUNCTION__);
		data->poll_time = 20;
	}*/
	
	data->pwm_gpio = of_get_named_gpio(node,"thundersoft,gpio_pwm",0);
	if(data->pwm_gpio<0){
		pr_err("%s,pwm gpio error gpio\n",__FUNCTION__);
		return -EINVAL;
	}
	data->period=PERIOD;//周期暂定为20ms
	
	#ifdef POWER_SWITCH
	data->power_gpio = of_get_named_gpio(node,"thundersoft,gpio_irq",0);
	
	if(data->power_gpio<0){
		pr_err("%s,powr gpio error gpio\n",__FUNCTION__);
		return -EINVAL;
	}
	#endif
	
	return 0;
}
static ssize_t tbs2701_show_enable(struct device *dev,
				struct device_attribute *attr, char *buf){
	struct tbs2701_data* data = dev_get_drvdata(dev);
	ssize_t lenth = sprintf(buf,"%d",data->pwm_enable);
	return lenth;
}
static ssize_t tbs2701_store_enable(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t size){
	struct tbs2701_data* data = dev_get_drvdata(dev);
	int enable = simple_strtoul(buf,NULL,10);
	if(enable==data->pwm_enable){
		printk("do not need to change status");
		return size;
	}
	
	mutex_lock(&data->data_lock);
	if(enable){
		//schedule_work(&data->pwm_work);
		data->pwm_enable = 1;
		add_timer(&data->s_timer);
	}
	else{		
		//cancel_work_sync(&data->pwm_work);
		data->pwm_enable = 0;
		del_timer(&data->s_timer);//关闭PWM时删除定时器
	}
	mutex_unlock(&data->data_lock);
	return size;

}
static ssize_t tbs2701_show_value(struct device *dev,
                                  struct device_attribute* attr,char* buf){
    struct tbs2701_data* data = dev_get_drvdata(dev);
    ssize_t lenth;
  //  wait_event_interruptible(data->data_queue,data->data_ready);
  //  data->data_ready = false;
    mutex_lock(&data->data_lock);
   // lenth = sprintf(buf,"%d",data->angle);//这里转化为角度再输出
    lenth = sprintf(buf,"%d",data->high_us);
    printk("%d\n",data->high_us);
    mutex_unlock(&data->data_lock);
    return lenth;
}

static ssize_t tbs2701_store_value(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t size){
	struct tbs2701_data* data = dev_get_drvdata(dev);
	int value = 0;
	
	value = simple_strtoul(buf,NULL,10);
    if(value<=500){//限制在0.5ms到2.5ms之间
		value=501;
	}
	else if(value>=2500){
		value=2499;
	}
	
	if(data->value_last==value){
		return size; //与上一次转角相同，无需改变脉宽
	}
	mutex_lock(&data->data_lock);
	if(data->pwm_enable){		
	//	cancel_work_sync(&data->pwm_work);
		data->high_us=value;
		//mod_timer(&data->s_timer,jiffies + low_us*1000*HZ);
		//add_timer(&data->s_timer);
		schedule_work(&data->pwm_work);		
	}
	else{	
		//del_timer(&data->s_timer);//关闭PWM时删除定时器
		cancel_work_sync(&data->pwm_work);
		
	}
	data->value_last=value;//记录最后改变的值
	mutex_unlock(&data->data_lock);
	return size;

}

static DEVICE_ATTR(enable,0644,tbs2701_show_enable,tbs2701_store_enable);
static DEVICE_ATTR(value,0644,tbs2701_show_value,tbs2701_store_value);



static int tbs2701_probe(struct platform_device *pdev){
	struct tbs2701_data* data;
	int result;
	tbs2701 = kmalloc(sizeof(struct tbs2701_data),GFP_KERNEL);
	if(!tbs2701){
		pr_err("%s kmalloc error\n",__FUNCTION__);
		return -ENOMEM;
	}
	dev_set_drvdata(&pdev->dev,tbs2701);
	result = parse_dt(pdev,tbs2701);
	if(result<0){
		pr_err("%s error when parse dt\n",__FUNCTION__);
		result = -EINVAL;
		goto err_parse_dt;
	}
	if(gpio_is_valid(tbs2701->pwm_gpio)){
		result = gpio_request(tbs2701->pwm_gpio,"pwm_gpio");
		if(result<0){
			pr_err("Unable to request pwm gpio\n");
			goto err_parse_dt;
		}
		//gpio_direction_output(data->pwm_gpio,1);
	}
	#ifdef POWER_SWITCH
	if(gpio_is_valid(tbs2701->power_gpio)){
		result = gpio_request(tbs2701->power_gpio,"power_gpio");
		if(result<0){
			pr_err("uanble to request power_gpio\n");
			goto err_parse_dt;
		}
		//gpio_direction_output(data->power_gpio,1);
	}
	#endif
	
	INIT_WORK(&data->pwm_work,pwm_work_func);
	/*初始化定时器*/ 
	//init_timer(&tbs2701->s_timer);
	//tbs2701->s_timer.function = &second_timer_handle;
	//tbs2701->s_timer.expires = jiffies + HZ;
    mutex_init(&tbs2701->data_lock);
	//tbs2701->turn_flag=true;//初始化high
    //init_waitqueue_head(&data->data_queue);
	
	
	result=sysfs_create_file(&pdev->dev.kobj,&dev_attr_enable.attr);
	result=sysfs_create_file(&pdev->dev.kobj,&dev_attr_value.attr);
	printk("tbs2701_probe sucess\n");
	return 0;
err_parse_dt:
	kfree(tbs2701);
	printk("tbs2701_probe failed\n");
	return result;
}
static int tbs2701_remove(struct platform_device *pdev){
	kfree(tbs2701);
	
	return 0;
}

static struct of_device_id tbs2701_match_table[] = {
	{ .compatible = COMPATIBLE,},
	{ },
};

static struct platform_driver tbs2701_driver = {
	.probe = tbs2701_probe,
	.remove = tbs2701_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name =  DRIVER_NAME,
		.of_match_table = tbs2701_match_table,
	},
};

module_platform_driver(tbs2701_driver);
MODULE_LICENSE("GPL v2");
