## 添加模块

### 模块的意义

由于LINUX设备驱动以内核模块的形式而存在，在具体的设备驱动开发中，将驱动编译为内核模块也有很强的工程意义，因为如果将正在开发中的驱动直接编译入内核，而开发过程中会**不断修改驱动的代码**，则需要**不断地编译内核**并重启内核，但是如果编译为**模块**，则只需要**rmmod**并**insmod**即可，开发效率大为提高。下面说明如何添加、编译并允许LINUX模块。

### 模块的组成

  LINUX的模块主要由6部分组成：

1. 模块的加载函数（必须）

当通过insmod或modprobe命令加载内核模块时，模块的加载函数会自动被内核执行，完成本模块的相关初始化工作。

2. 模块的卸载函数（必须）

当通过rmmod命令卸载某模块时，模块的卸载函数会自动被内核执行，完成与模块加载函数相反的功能。

3. 模块许可证声明

模块许可证（LICENSE）声明描述内核模块的的许可权限，如果不声明LICENSE,模块被加载时，将接到内核被污染的警告。

4. 模块参数（可选）

模块参数是模块被加载的时候可以被传递给它的值，它本身对应模块内部的全局变量。

5. 模块导出符号（可选）

 内核模块可以导出符号(symbol,对应于函数或者是变量），这样其他模块就可以使用本模块中的变量或者是函数。

6. 模块作者等信息声明（可选）



### demo

#### 模块代码  kello.c

此模块的主要功能就是分别在模块装载和模块卸载的时候打印输出内容

```c
#include <linux/module.h>		
int kello_init( void )
{
	printk( "\n   Hello, 尹浩男 \n" );
	return	0;
}

void kello_exit( void )
{
	printk( "\n   Goodbye now... 尹浩男 \n" );	
}
MODULE_AUTHOR("尹浩男");
MODULE_LICENSE("GPL");
module_init(kello_init);
module_exit(kello_exit);

```



#### Makefile

`$(MAKE) -C $(KDIR) M=$(PWD)`与`$(MAKE) -C $(KDIR) SUBDIRS =$(PWD)`的作用是等效的，后者是较老的使用方法。推荐使用M而不是SUBDIRS，前者更明确。

**旧版本的Ubuntu14 运行 `$(MAKE) -C $(KDIR) M=$(PWD)`没有问题 但是新版本不行**

```makefile
ifneq	($(KERNELRELEASE),)
obj-m	:= kello.o 

else
KDIR	:= /lib/modules/$(shell uname -r)/build
PWD	:= $(shell pwd)
default:	
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules 
	rm -r -f .tmp_versions *.mod.c .*.cmd *.o *.symvers 

endif
```





#### 编译执行过程

```shell
> cd /home/yhn/os/ch12/vexp1  
> make  # 编译模块代码
make -C /lib/modules/5.4.0-42-generic/build M=/home/yhn/os/ch12/vexp1 modules 
make[1]: Entering directory '/usr/src/linux-headers-5.4.0-42-generic'
  CC [M]  /home/yhn/os/ch12/vexp1/kello.o
  Building modules, stage 2.
  MODPOST 1 modules
  CC [M]  /home/yhn/os/ch12/vexp1/kello.mod.o
  LD [M]  /home/yhn/os/ch12/vexp1/kello.ko
make[1]: Leaving directory '/usr/src/linux-headers-5.4.0-42-generic'
rm -r -f .tmp_versions *.mod.c .*.cmd *.o *.symvers 
> insmod kello.ko   # 安装模块
> dmesg | tail -1   #  显示最后一行
                     Hello, 尹浩男  
> rmmod kello.ko 
> dmesg | tail -1  # 显示最后一行
                    Goodbye now... 尹浩男
```



#### dmesg命令

dmesg命令用于显示开机信息。

kernel会将开机信息存储在ring buffer中。您若是开机时来不及查看信息，可利用dmesg来查看。开机信息亦保存在/var/log目录中，名称为dmesg的文件里。

持续跟踪dmesg日志文件  

` 添加模块和删除模块并不会改变/var/log/dmesg文件的内容，可能是系统还没有写进去`

>  tail -f /var/log/dmesg

