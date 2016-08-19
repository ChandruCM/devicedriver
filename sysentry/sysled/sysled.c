#include<linux/module.h>
#include<linux/init.h>
#include<linux/version.h>
#include<linux/types.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/sched.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<linux/gpio.h>
#include<linux/uaccess.h>
#include<linux/io.h>
#include<linux/printk.h>
#include<linux/kobject.h>
#include<linux/sysfs.h>
#include<linux/string.h>
#include<linux/kernel.h>

#define gpio 48
#define led_addr 0x4804c000

/* for device entry
static dev_ first;
static struct cdev c_dev;
static struct class *cl;*/
/* for sys file system*/

static struct kobject *led_object;
static int led; 

//int ledvalue;
unsigned int ledaddr;

static ssize_t led_show(struct kobject *kobj,struct kobj_attribute *attr,char *buf)
{
	
	return sprintf(buf,"%d\n",led);
}
static ssize_t led_store(struct kobject *kobj,struct kobj_attribute *attr,char *buf,ssize_t count)
{
	sscanf(buf,"%du",&led);
	
	ledaddr=ioremap(led_addr,0x400);
	iowrite32(~(1<<16),ledaddr+0x134);
	printk(KERN_INFO "ledaddr is %d\n",ledaddr);
	switch(buf[0])
	{
	case '0' : iowrite32(1<<16,ledaddr+0x190);
		break;
	case '1' : iowrite32(1<<16,ledaddr+0x194);
		break;
	}
	return count;
} 
static struct kobj_attribute led_attribute=__ATTR(led,0660,led_show,led_store);

static int __init led_init(void)
{
  int error=0;
	
	  pr_debug("module initialized successfully..\n");
	led_object=kobject_create_and_add("gpioled",kernel_kobj);
	
	if(led_object)
	return -ENOMEM;
	
	error=sysfs_create_file(led_object,&led_attribute.attr);
	if(error)
	{
		pr_debug("failed to create the foo file in sys/kernel/gpioled\n");
	}
	return error;
}
	static void __exit led_exit(void)
{
	pr_debug("Module un initialized..\n");
	kobject_put(led_object);
	iounmap(ledaddr);
}
module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");

