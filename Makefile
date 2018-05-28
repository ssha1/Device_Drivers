ifneq ($(KERNELRELEASE),)
	obj-m := my_driver.o
	my_driver-objs := char_driver_1.o
else
	KERNEL_SRC_DIR := /lib/modules/$(shell uname -r)/build
all:
	$(MAKE) -C $(KERNEL_SRC_DIR) M=$(PWD) modules
load:
	insmod ./my_driver.ko
unload:
	rmmod ./my_driver.ko
clean:
	$(MAKE) -C $(KERNEL_SRC_DIR) M=$(PWD) clean
endif
