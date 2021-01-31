#include <linux/module.h>

int hello_init(void)
{
	printk("\n Hello, students from SDUST! This is in kernel space! \n");
	return 0;
}

void hello_exit(void)
{
	printk("\n Goodbye now, students from SDUST! \n");
}

MODULE_AUTHOR("lyh@sdust");
MODULE_LICENSE("GPL");
module_init(hello_init);
module_exit(hello_exit);
