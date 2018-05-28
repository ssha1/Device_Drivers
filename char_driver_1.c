#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include"my_ioctl.h"

MODULE_LICENSE("GPL");

int (*device_open)(struct inode*,struct file*);
int (*device_close)(struct inode*,struct file*);
ssize_t (*device_read)(struct file*,char __user*,ssize_t,loff_t*); 
ssize_t (*device_write)(struct file*,char __user*,ssize_t,loff_t*);
int (*device_ioctl)(struct file*,unsigned int,unsigned long);

static struct file_operations fops =
{
	.open = device_open,
	.close = device_close,
	.read = device_read,
	.write = device_write,
	.ioctl = device_ioctl;
}
;
static char* device_name = "My char device";
static dev_t dev;
static struct cdev* cdev;
static struct class* cl;

static int __init char_dev_init(void)
{

	int err = 0;
	printk(KERN_INFO "Char device loaded \n");
	// Function registers major number dynamically
	if((err = alloc_chrdev_region(&dev,0,10,device_name)<0)
	{
		printk(KERN_INFO "Error registering device numbers \n");
		return err;
	}
	printk(KERN_INFO "Major and minor numbers allocated \n");
	//Allocates memory for cdev structure
	//cdev is used to specify file operations
	if((cl = class_create(THIS_MODULE,"my_device"))==NULL)
	{
		unregister_chrdev_region(dev,10);
		return -1;
	}
	if((device_create(cl,NULL,dev,NULL,"mynull"))==NULL)
	{
		class_destroy(cl);
		unregister_chrdev_region(dev,10);
		return -1;
	}
	cdev = cdev_alloc();
	cdev->ops = &fops;
	if((err = cdev_add(cdev,dev,10))<0)
	{
		printk(KERN_INFO "Error registering device \n");
		return err;
	}
	return err;
}

static int  device_open(struct inode* inodep,struct file* filep)
{
	printk(KERN_INFO "Device opened");
	return 0;
}

static ssize_t device_read(struct file* filep,char __user* data,ssize_t count,loff_t* offset)
{
	unsigned long data_read;
	char* device_data = "Device read data";	
	data_read = copy_to_user(data,device_data,1);
	return data_read;
}

static ssize_t device_write(struct file* filep,char __user* user_data,ssize_t count,loff_t* offset)
{
	unsigned long data_wrote;
	char* data;
	data_read = copy_to_user(data,user_data,1);
	return data_read;
}

static int device_ioctl(struct file* filep,unsigned int cmd,unsigned long arg)
{
	struct device_arg darg;
	switch(cmd)
	{
		case GET_VARIABLES:
		
		case SET_VARIABLES:
		case CLR_VARIABLES: 
	} 
}

static int device_close(struct inode* inodep,struct file* filep)
{
	printk(KERN_INFO "Device close");
	return 0;
}

static void __exit char_dev_exit(void)
{
	printk(KERN_INFO "Module unloaded");
	cdev_del(cdev);
	unregister_chrdev_region(dev,10);
}

module_init(char_dev_init);
module_exit(char_dev_exit);


