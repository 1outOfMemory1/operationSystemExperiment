
ifneq	($(KERNELRELEASE),)
obj-m	:= miscdev_map.o 

else
KDIR	:= /lib/modules/$(shell uname -r)/build
PWD	:= $(shell pwd)
default:	
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules 
	rm -r -f .tmp_versions *.mod.c .*.cmd *.o *.symvers 

endif

