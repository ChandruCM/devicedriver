#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/timer.h>
#include<linux/param.h>

MODULE_LICENSE("GPL");

static struct timer_list my_timer;

void my_timer_callback( unsigned long data )
{
int i=0;
	for(i;i<=data;i++)
    printk(KERN_INFO "hi chandru..\n");
}
/*void my_timer_call(unsigned long data)
{
Printk(KERN_INFO "bye chandru...\n");
}*/
int timer_init(void)
{
  /* setup your timer to call my_timer_callback */
  /* setup timer interval to 200 msecs */
  setup_timer(&my_timer, my_timer_callback, 10);
  mod_timer(&my_timer, jiffies + msecs_to_jiffies(200));
  return 0;
}

void timer_exit(void)
{
  /* remove kernel timer when unloading module */
  del_timer(&my_timer);
  return;
}
module_init(timer_init);
module_exit(timer_exit);

