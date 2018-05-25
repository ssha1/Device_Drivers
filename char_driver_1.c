#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/kernel.h>
#include<linux/init.h>

MODULE_LICENSE("GPL");

static struct file_operations fops;
char* device_name = "My char device";
dev_t dev;
struct cdev* cdev;

static int __init char_dev_init(void)
{

	int err = 0;
	printk(KERN_INFO "Char device loaded \n");
	// Function registers major number dynamically
	err = alloc_chrdev_region(&dev,0,10,device_name);
	if(err!=0)
	{
		goto end;
	}
	printk(KERN_INFO "Major and minor numbers allocated \n");
	//Allocates memory for cdev structure
	//cdev is used to specify file operations
	cdev = cdev_alloc();
	cdev->ops = &fops;
	err = cdev_add(cdev,dev,10);
	if(err!=0)
		goto end1;
end:
	printk(KERN_INFO "Error registering device numbers \n");
end1:
	printk(KERN_INFO "Error registering device \n");
	return err;
}

static void __exit char_dev_exit(void)
{
	printk(KERN_INFO "Module unloaded");
	unregister_chrdev_region(dev,10);
	cdev_del(cdev);
}

module_init(char_dev_init);
module_exit(char_dev_exit);


