//-------------------------------------------------------------------
//	cmos.c�� ԭʼ����ALLAN CRUSE�� ��дby fs
//	orgininal programmer: ALLAN CRUSE, written on: 03 SEP 2007
//	NOTE: Written and tested using Linux kernel version 2.6.22.
//	refined by fs on 2014 to be suitable for Linux 3.13.0
//    $ make
//    $ sudo insmod cmos.ko
//    $ sudo  mknod /dev/cmos c 70 0
//    $ sudo chmod a+rw /dev/cmos
//    ����ʹ��cmos_test.c����
//-------------------------------------------------------------------

#include <linux/module.h>	// for printk() 
#include <linux/fs.h>		// for register_chrdev() 
#include <linux/cdev.h>
#include <asm/uaccess.h>	// for put_user(), get_user()
#include <asm/io.h>		// for inb(), outb()

char devname[] = "cmos";	// name for the device's file
int	my_major = 70;		// major ID-number for driver
int	cmos_size = 128;	// total bytes of cmos memory
int	write_max = 9;		// largest 'writable' address

ssize_t my_read( struct file *file, char *buf, size_t len, loff_t *pos )
{
	unsigned char	data;

	if ( *pos >= cmos_size ) return 0;

	outb( *pos, 0x70 );  data = inb( 0x71 );

	if ( put_user( data, buf ) ) return -EFAULT;

	*pos += 1;
	return	1;
}

ssize_t my_write( struct file *file, const char *buf, size_t len, loff_t *pos )
{
	unsigned char	data;

	if ( *pos >= cmos_size ) return 0;

	if ( *pos > write_max ) return -EPERM;

	if ( get_user( data, buf ) ) return -EFAULT;

	outb( *pos, 0x70 );  outb( data, 0x71 );

	*pos += 1;
	return	1;
}

loff_t my_llseek( struct file *file, loff_t pos, int whence )
{
	loff_t	newpos = -1;

	switch ( whence )
		{
		case 0:	newpos = pos; break;			// SEEK_SET
		case 1: newpos = file->f_pos + pos; break;	// SEEK_CUR
		case 2: newpos = cmos_size + pos; break;	// SEEK_END
		}

	if (( newpos < 0 )||( newpos > cmos_size )) return -EINVAL;
	
	file->f_pos = newpos;
	return	newpos;
}


struct file_operations my_fops = {
	owner:	THIS_MODULE,
	llseek:	my_llseek,
	write:	my_write,
	read:	my_read,
};

struct cdev cmos_dev;
static void cmos_setup_cdev(void)
{
    int err, devno;
    devno = MKDEV(my_major, 0);
    cdev_init(&cmos_dev, &my_fops);
    cmos_dev.owner = THIS_MODULE;
    //cmos_dev.ops   = &my_fops;    
    err = cdev_add(&cmos_dev, devno, 1);
    if (err)
        printk("Error %d\n", err);
    else
        printk("have finish add\n");
}


static int __init my_init( void )
{
    int ret; dev_t devno;
    printk( "<1>\nInstalling \'%s\' module ", devname );
    printk( "(major=%d) \n", my_major );
    //return	register_chrdev( my_major, devname, &my_fops );
    devno = MKDEV(my_major, 0);
    ret = register_chrdev_region(devno, 1, devname );   
    cmos_setup_cdev();
    printk ("<1>\n \'%s\'has been Initialized \n",devname); 	
    return 0;
}

static void __exit my_exit(void )
{
	//unregister_chrdev( my_major, devname );
        cdev_del(&cmos_dev);
        unregister_chrdev_region(MKDEV(my_major, 0), 1);
	printk( "<1>Removing \'%s\' module\n", devname );
}

module_init( my_init );
module_exit( my_exit );
MODULE_LICENSE("GPL"); 

