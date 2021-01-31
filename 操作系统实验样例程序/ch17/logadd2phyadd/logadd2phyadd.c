//-------------------------------------------------------------------
//	logadd2phyadd.c： 本内核文件创建一个proc伪文件，'/proc/logadd2phyadd'
//	可以将一个测试进程中的某个变量的线性地址的物理地址输出。
//	本内核代码的关键是：Linux四级页表映射机制
//  编译命令： $ make 
//  内核模块添加：$sudo insmod logadd2phyadd.ko  
//  内核模块删除：$sudo rmmod logadd2phyadd
//	NOTE: Written and tested with Linux kernel version 3.13.0 and.3.2.0
//  测试：请使用logadd2phyadd_test.c文件。
//strace函数可用于追踪系统调用
//-------------------------------------------------------------------

#include <linux/module.h>	// for init_module() 
#include <linux/proc_fs.h>	// for create_proc_info_entry() 
#include <linux/sched.h>	// for 'struct task_struct'
#include <linux/seq_file.h>	// for sequence files
#include <linux/mm.h>		// for 'struct mm_struct'
#include <linux/slab.h>       // for kzalloc, kfree  
char modname[] = "logadd2phyadd";
typedef struct data {
	unsigned long addr;
	int p;
}mydata;
static mydata indata;
static void get_pgtable_macro(struct seq_file *m)
{

   seq_printf( m,"PAGE_OFFSET = 0x%lx\n", PAGE_OFFSET);
   seq_printf( m,"PGDIR_SHIFT = %d\n", PGDIR_SHIFT);
   seq_printf( m,"PUD_SHIFT = %d\n", PUD_SHIFT);
   seq_printf( m,"PMD_SHIFT = %d\n", PMD_SHIFT);
   seq_printf( m,"PAGE_SHIFT = %d\n", PAGE_SHIFT);

   seq_printf( m,"PTRS_PER_PGD = %d\n", PTRS_PER_PGD);
   seq_printf( m,"PTRS_PER_PUD = %d\n", PTRS_PER_PUD);
   seq_printf( m,"PTRS_PER_PMD = %d\n", PTRS_PER_PMD);
   seq_printf( m,"PTRS_PER_PTE = %d\n", PTRS_PER_PTE);

   seq_printf( m,"PAGE_MASK = 0x%lx\n", PAGE_MASK);


}

static unsigned long vaddr2paddr(struct seq_file *m, unsigned long vaddr,int pid)
{
    pte_t *pte_tmp = NULL;  
    pmd_t *pmd_tmp = NULL;  
    pud_t *pud_tmp = NULL;  
    pgd_t *pgd_tmp = NULL;  
    struct task_struct *pcb_tmp = NULL;
    unsigned long paddr = 0;
   /*
    if(!(pcb_tmp = find_task_by_pid(pid))) {
         printk(KERN_INFO"Can't find the task %d .\n",pid);
         return 0;
    }
   */
    printk(KERN_INFO"in vaddr2paddr try to find the task %d .\n",pid);	
    pcb_tmp =pid_task(find_get_pid(pid),PIDTYPE_PID); 
    if(!pcb_tmp) {
         printk(KERN_INFO"Can't find the task %d .\n",pid);
         return 0;
    }
	
  pgd_tmp = pgd_offset(pcb_tmp->mm, vaddr);
  if (pgd_none(*pgd_tmp)) {
      printk("not mapped in pgd\n");
      return -1;
  }
 seq_printf( m,"pgd_tmp = 0x%p\n",pgd_tmp);
 seq_printf( m,"pgd_val(*pgd_tmp) = 0x%lx\n",pgd_val(*pgd_tmp));
	
  pud_tmp = pud_offset(pgd_tmp, vaddr);  
  if (pud_none(*pud_tmp)) {
      printk("not mapped in pud\n");
      return -1;
  }
 seq_printf( m,"pud_tmp = 0x%p\n",pud_tmp);
 seq_printf( m,"pud_val(*pud_tmp) = 0x%lx\n",pud_val(*pud_tmp));	

  pmd_tmp = pmd_offset(pud_tmp, vaddr);
  if (pmd_none(*pmd_tmp)) {
      printk("not mapped in pmd\n");
      return -1;
  }
 seq_printf( m,"pmd_tmp = 0x%p\n",pmd_tmp);
 seq_printf( m,"pmd_val(*pmd_tmp) = 0x%lx\n",pmd_val(*pmd_tmp));	


  //pte = pte_offset_kernel(pmd, vaddr);
  pte_tmp = pte_offset_kernel(pmd_tmp, vaddr);    
  if (pte_none(*pte_tmp)) {
      printk("not mapped in pte\n");
      return -1;
  }
 seq_printf( m,"pte_tmp = 0x%p\n",pte_tmp);
 seq_printf( m,"pte_val(*pte_tmp) = 0x%lx\n",pte_val(*pte_tmp));

    //ÎïÀíµØÖ·µÈÓÚ £šÖ¡µØÖ· | Æ«ÒÆÁ¿£©
  paddr = (pte_val(*pte_tmp) & PAGE_MASK) | (vaddr & ~PAGE_MASK); 
  seq_printf( m, "frame_addr = %lx\n", pte_val(*pte_tmp) & PAGE_MASK);
  seq_printf( m, "frame_offset = %lx\n", vaddr & ~PAGE_MASK);
  seq_printf( m, "the input logic address is = 0x%08lX\n", vaddr);
  seq_printf( m, "the corresponding physical address is= 0x%08lX\n",paddr);
  //seq_printf( m, "the content in the corresponding physical address is= %08lX\n",*(unsigned long*)paddr);
  seq_printf( m, "the content in the corresponding physical address is =0x%08lX\n",*(unsigned long *)((char *)paddr + PAGE_OFFSET));
  return paddr;
}

static int my_seq_show(struct seq_file *m, void *p)
{
        unsigned int	_cr0,_cr3, _cr4;
	get_pgtable_macro(m);
	printk("the get_pgtable_macro has been output\n");
        if (indata.p !=0 && indata.addr !=0)
		vaddr2paddr(m, indata.addr,indata.p);
        printk("the vaddr2paddr has been done\n");		

	asm( " mov %%cr0, %%eax \n mov %%eax, %0 " : "=m" (_cr0) :: "ax" );
	asm( " mov %%cr3, %%eax \n mov %%eax, %0 " : "=m" (_cr3) :: "ax" );
	asm( " mov %%cr4, %%eax \n mov %%eax, %0 " : "=m" (_cr4) :: "ax" );
	seq_printf(m, "    CR0=0x%08lX", _cr0 );
	seq_printf(m, "    CR3=0x%08lX", _cr3 );
	seq_printf(m, "    CR4=0x%08lX", _cr4 );
	seq_printf(m, "    PE=%X",  (_cr0 >> 1)&1 );
	seq_printf(m, "    PG=%X",  (_cr0 >> 31)&1 );
	seq_printf(m, "    PAE=%X", (_cr4 >> 5)&1 );
	seq_printf(m, "    PSE=%X", (_cr4 >> 4)&1 );
	seq_printf( m, "\n" );
	
	return	0;

}

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

//ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
// file_operations -> write  
static ssize_t my_write(struct file *file, const char __user *buffer, size_t count, loff_t *pos )  
{  
    
    //·ÖÅäÁÙÊ±»º³åÇø  
    char *tmp = kzalloc((count+1), GFP_KERNEL);  
    if (!tmp)  
        return -ENOMEM;  

    //œ«ÓÃ»§Ì¬writeµÄ×Ö·ûŽ®¿œ±ŽµœÄÚºË¿ÕŒä  
    //copy_to|from_user(to,from,cnt)  
    if (copy_from_user(tmp, buffer, count)) {  
        kfree(tmp);  
        return -EFAULT;  
    } 
    indata = *(mydata*)tmp;     	
    printk("the useraddr is %lu\n",indata.addr);
    printk("the pid is %d\n",indata.p);	  

    kfree(tmp);  

    return count;  
} 
static const struct file_operations my_proc =   
{  
      .owner      = THIS_MODULE,  
      .open       = my_open,
      .write       = my_write, 	
      .read       = seq_read,      
      .llseek     = seq_lseek,
      .release    = seq_release  	    
};
 
static int __init my_init( void )
{
	struct proc_dir_entry* my_proc_entry;
	printk( "<1>\nInstalling \'%s\' module\n", modname );
	my_proc_entry = proc_create(modname, 0x0666, NULL, &my_proc);
	if (NULL == my_proc_entry)
	{
	    return -ENOMEM;
	}
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
