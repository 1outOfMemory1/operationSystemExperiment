//-------------------------------------------------------------------
//	myshowgdt.c  ��ȡGDT�и����εĶ�����
//	���ں��ļ�����һ��procα�ļ���'/proc/myshowgdt'	
//  ������� $make 
//  �ں�ģ����ӣ�$sudo insmod myshowgdt.ko  
//  ��ʾ��Ϣ��$cat /proc/myshowgdt
//  �ں�ģ��ɾ����$sudo rmmod myshowgdt
// ע�⣺�����ַ�������û��ռ��ַ�������ں˿ռ��ַ֮��Ĺ�ϵ��ת��
// ע�⣺�����ڴ��������ڴ�Ĺ�ϵ
//-------------------------------------------------------------------

#include <linux/module.h>	// for init_module() 
#include <linux/proc_fs.h>	// for create_proc_info_entry() 
#include <linux/seq_file.h>
#include <linux/highmem.h>	// for kmap(), kunmap()
#include <asm/uaccess.h>	// for copy_to_user() 
#define START_KERNEL_map 0xC0000000
char modname[] = "myshowgdt";
loff_t 	ram_size;
unsigned short 	gdtr[3];
unsigned long	gdt_virt_address; 
unsigned long	gdt_phys_address;

// seq_operations -> show
static int my_seqshow(struct seq_file *m, void *v)
{	

	int	n_elts, i;
	int	frame_number, frame_indent;
	struct page	*pp;
	unsigned long long		*from;
	unsigned long long	descriptor;
	// use inline assembly language to get GDTR register-image
	asm(" sgdt gdtr ");

	// extract GDT virtual-address from GDTR register-image
	gdt_virt_address = *(unsigned long*)(gdtr+1);
	// compute GDT physical-address using subtraction
	gdt_phys_address = gdt_virt_address - START_KERNEL_map;

	// extract GDT segment-limit and compute descriptor count
	n_elts = (1 + gdtr[0])/8;
	seq_printf(m,  "GDT segment descriptors' number is %d\n", n_elts );
	// report the GDT virtual and physical memory-addresses 
	seq_printf(m,  "gdt_virt_address=%08lX ", gdt_virt_address );
	seq_printf(m,  "gdt_phys_address=%08lX ", gdt_phys_address );
	seq_printf(m,  "\n" );

	if ( gdt_phys_address >= ram_size ) return 0;
	// determine which physical page to temporarily map
	// and how far into that page to begin reading from 
	frame_number = gdt_phys_address / PAGE_SIZE;
	frame_indent = gdt_phys_address % PAGE_SIZE;
        pp = pfn_to_page( frame_number);

	from = (unsigned long long*)(kmap( pp ) + frame_indent);
	for (i = 0; i < n_elts; i++)
		{
		if ( ( i % 4 ) == 0 ) seq_printf(m, "\n %04X: ", i*8 );
		descriptor = *from;
		seq_printf(m,  "%016llX ", descriptor );
		from++;
		}

	seq_printf(m, "\n" );
	kunmap( pp );
	return 0; //!! must be 0, or will show nothing T.T
}


static int my_open(struct inode *inode, struct file *file)  
{  
    return single_open(file, my_seqshow, NULL);
}  

static const struct file_operations my_proc =   
{  
	.owner      = THIS_MODULE,  
	.open       = my_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release    = single_release,
}; 

static int __init my_init( void )
{
	struct proc_dir_entry* my_proc_entry;
	printk( "<1>\nInstalling \'%s\' module\n", modname );	
	my_proc_entry = proc_create(modname, 0x644, NULL, &my_proc);
	ram_size = (loff_t)totalram_pages << PAGE_SHIFT;
	printk( "<1>ram_size is %lu\n",ram_size  );	
	return	0;  //SUCCESS
}

static void __exit my_exit(void )
{
	remove_proc_entry( modname, NULL );
	printk( "<1>Removing \'%s\' module\n", modname );
}

module_init( my_init );
module_exit( my_exit );
MODULE_LICENSE("GPL"); 

