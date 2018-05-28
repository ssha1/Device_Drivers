#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<asm/uaccess.h>
#include<linux/device.h>
#include"my_ioctl.h"

MODULE_LICENSE("GPL");

static int device_open(struct inode*,struct file*);
static int device_close(struct inode*,struct file*);
static ssize_t device_read(struct file*,char __user*,size_t,loff_t*); 
static ssize_t device_write(struct file*,const char __user*,size_t,loff_t*);
static int device_ioctl(struct file*,unsigned int,unsigned long);


static struct file_operations fops =
{
	.open = device_open,
	.release = device_close,
	.read = device_read,
	.write = device_write,
	.unlocked_ioctl = device_ioctl
};
static char* device_name = "My char device";
static dev_t dev;
static struct cdev* cdev;
static struct class* cl;

static int __init char_dev_init(void)
{

	int err = 0;
	printk(KERN_INFO "Char device loaded \n");
	// Function registers major number dynamically
	if((err = alloc_chrdev_region(&dev,0,10,device_name))<0)
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

static ssize_t device_read(struct file* filep,char __user* data,size_t count,loff_t* offset)
{
	unsigned long data_read;
	char* device_data = "Device read data";	
	data_read = copy_to_user(data,device_data,strlen(device_data));
	return data_read;
}

static ssize_t device_write(struct file* filep,const char __user* user_data,size_t count,loff_t* offset)
{
	unsigned long data_wrote;
	char* data = {0};
	data_wrote = copy_from_user(data,user_data,count);
	return data_wrote;
}

static int device_ioctl(struct file* filep,unsigned int cmd,unsigned long arg)
{
	static device_arg darg;
	int power;
	int speed;
	switch(cmd)
	{
		case GET_VARIABLES:
			darg.power = power;
			darg.speed = speed;
			if (copy_to_user((device_arg *)arg, &darg, sizeof(device_arg)))
			{
				return -EACCES;
			}
			break;
		case SET_VARIABLES:
			if (copy_from_user(&darg, (device_arg *)arg, sizeof(device_arg)))
			{
				return -EACCES;
			}
			power = darg.power;
			speed = darg.speed;
			break;
		case CLR_VARIABLES: 
			power = 0;
			speed = 0;
	}
	return 0;
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


