#include<linux/module.h> // This header should be included in every module
#include<linux/kernel.h> // This header for printk 
#include<linux/init.h> // This header for macros such as module_init,module_exit

MODULE_DESCRIPTION("THIS IS BASIC MODULE");
MODULE_LICENSE("GPL");

static int __init my_init(void)
{
	printk(KERN_INFO "Module loaded \n");
	return 0;
}

static void __exit my_exit(void)
{
	printk(KERN_INFO "Module unloaded \n");
}

module_init(my_init);
module_exit(my_exit);


