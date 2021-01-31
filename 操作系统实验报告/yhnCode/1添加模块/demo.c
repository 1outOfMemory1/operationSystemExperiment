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
