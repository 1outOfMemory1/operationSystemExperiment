//-------------------------------------------------------------------
//	vma.c
//	本内核文件创建一个proc伪文件，'/proc/vma'
//	可以将一个测试进程的虚拟内存区域信息进行输出。
//	本内核代码的关键是：Linux中的struct vm_area_struct结构体
//  编译命令： $ make 
//  内核模块添加：$sudo insmod vma.ko  
//  内核模块删除：$sudo rmmod vma
//	NOTE: Written and tested with Linux kernel version 3.13.0 and.3.2.0
//  测试：请使用vma_test.c文件。
//strace函数可用于追踪系统调用
//----------------------------------
//-------------------------------------------------------------------

#include <linux/module.h>	// for init_module() 
#include <linux/proc_fs.h>	// for create_proc_read_entry() 
#include <linux/seq_file.h>	// for sequence files
#include <linux/mm.h>		// for 'struct vm_area_struct'
#include <linux/sched.h>	// for 'struct task_struct'
#include <linux/mount.h>
#include <linux/slab.h>
#include <linux/dcache.h>

char modname[] = "vma";

static void * my_seq_start(struct seq_file *m, loff_t *pos)
{
  if (0 == *pos)  
  {  
     ++*pos;  
     return (void *)1; 
   }  

        return NULL;
}
static void * my_seq_next(struct seq_file *m, void *p, loff_t *pos)
{
        // do nothing
        return NULL;
}
static void my_seq_stop(struct seq_file *m, void *p)
{
				//// do nothing        
}


static int my_seq_show(struct seq_file *m, void *p)
{	
	struct task_struct	*tsk = current;
	struct vm_area_struct	*vma;
	unsigned long 		ptdb;
	int i=0;
	struct mm_struct	*mm = tsk->mm;
	unsigned long		stack_size = (mm->stack_vm << PAGE_SHIFT);
	unsigned long		down_to = mm->start_stack - stack_size;	
	
	seq_printf( m, "\nInfo from the Memory Management structure " );
	seq_printf( m, "for task \'%s\' ", tsk->comm );
	seq_printf( m, "(pid=%d) \n", tsk->pid );
	seq_printf( m, "   pgd=%08lX  ", (unsigned long)mm->pgd );
	seq_printf( m, "mmap=%08lX  ", (unsigned long)mm->mmap );
	seq_printf( m, "map_count=%d  ", mm->map_count );
	seq_printf( m, "mm_users=%d  ", mm->mm_users.counter );
	seq_printf( m, "mm_count=%d  ", mm->mm_count.counter );
	seq_printf( m, "\n" );
	seq_printf( m, "    start_code=%08lX  ", mm->start_code );
	seq_printf( m, " end_code=%08lX\n", mm->end_code );
	seq_printf( m, "    start_data=%08lX  ", mm->start_data );
	seq_printf( m, " end_data=%08lX\n", mm->end_data );
	seq_printf( m, "     start_brk=%08lX  ", mm->start_brk );
	seq_printf( m, "      brk=%08lX\n", mm->brk );
	seq_printf( m, "     arg_start=%08lX  ", mm->arg_start );
	seq_printf( m, "  arg_end=%08lX\n", mm->arg_end );
	seq_printf( m, "     env_start=%08lX  ", mm->env_start );
	seq_printf( m, "  env_end=%08lX\n", mm->env_end );
	seq_printf( m, "   start_stack=%08lX  ", mm->start_stack );
	seq_printf( m, "  down_to=%08lX ", down_to );
	seq_printf( m, " <--- stack grows downward \n" );

	seq_printf( m, "List of the Virtual Memory Areas " );
	seq_printf( m, "for task \'%s\' ", tsk->comm );
	seq_printf( m, "(pid=%d)", tsk->pid );
	// loop to traverse the list of the task's vm_area_structs
	vma = tsk->mm->mmap;
	while ( vma )	{		
		char	ch;	
		seq_printf( m, "\n%3d ", ++i );
		seq_printf( m, " vm_start=%08lX ", vma->vm_start );
		seq_printf( m, " vm_end=%08lX  ", vma->vm_end );

		ch = ( vma->vm_flags & VM_READ ) ? 'r' : '-';
		seq_printf( m, "%c", ch );
		ch = ( vma->vm_flags & VM_WRITE ) ? 'w' : '-';
		seq_printf( m, "%c", ch );
		ch = ( vma->vm_flags & VM_EXEC ) ? 'x' : '-';
		seq_printf( m, "%c", ch );
		ch = ( vma->vm_flags & VM_SHARED ) ? 's' : 'p';
		seq_printf( m, "%c", ch );
		vma = vma->vm_next;
	}
	//seq_printf( m, "\n" );

	// display additional information about tsk->mm
	asm(" movl %%cr3, %%ecx \n movl %%ecx, %0 " : "=m" (ptdb) );
	seq_printf( m, "\nCR3=%08lX ", ptdb );
	seq_printf( m, " mm->pgd=%p ", tsk->mm->pgd );
	seq_printf( m, " mm->map_count=%d ", tsk->mm->map_count );
	seq_printf( m, "\n" );
  	return	0;
}

static struct seq_operations my_seq_fops = {
        .start  = my_seq_start,
        .next   = my_seq_next,
        .stop   = my_seq_stop,
        .show   = my_seq_show
};

static int my_open(struct inode *inode, struct file *file)  
{  
    return seq_open(file, &my_seq_fops);  
}  

static const struct file_operations my_proc =   
{  
	    .owner      = THIS_MODULE,  
	    .open       = my_open,
	    .read       = seq_read,      
      .llseek     = seq_lseek,
      .release    = seq_release  	    
}; 

static int __init my_init( void )
{
struct proc_dir_entry* my_proc_entry;
	printk( "<1>\nInstalling \'%s\' module\n", modname );
	my_proc_entry = proc_create(modname, 0, NULL, &my_proc);
	if (NULL == my_proc_entry)
	{
	    return -ENOMEM;
	}
	return	0; // SUCCESS
}

static void __exit my_exit(void )
{
	remove_proc_entry( modname, NULL );
	printk( "<1>Removing \'%s\' module\n", modname );
}

MODULE_LICENSE("GPL"); 
module_init( my_init );
module_exit( my_exit );

