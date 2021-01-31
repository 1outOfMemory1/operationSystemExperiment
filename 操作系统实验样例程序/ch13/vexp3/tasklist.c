//-------------------------------------------------------------------
//	tasklist.c：//显示task_struct结构体更多信息。
//	本内核文件创建一个proc伪文件，'/proc/tasklist'
//	通过如下命令可以显示系统中所有进程的部分信息
//  编译命令： make 
//  内核模块添加：$sudo insmod tasklist.ko
//  添加内核模块后读取并信息tasklist内核信息： $ cat /proc/tasklist
//  内核模块删除：$sudo rmmod tasklist
//	NOTE: Written and tested with Linux kernel version 3.13.0 and.3.2.0
//-------------------------------------------------------------------

#include <linux/module.h>	// for init_module() 
#include <linux/proc_fs.h>	// for create_proc_info_entry() 
#include <linux/sched.h>	// for init_task
#include <linux/seq_file.h>	// for sequence files
#include <linux/slab.h>       // for kzalloc, kfree  

char modname[] = "tasklist";
struct task_struct  *task;
int  taskcounts=0;			// 'global' so value will be retained

static void * my_seq_start(struct seq_file *m, loff_t *pos)
{
   ///printk(KERN_INFO"Invoke start\n");   //可以输出调试信息
   if ( *pos == 0 )  // 表示遍历开始
   {
	   task = &init_task;	//遍历开始的记录地址
	   return &task;   //返回一个非零值表示开始遍历
  }
  else //遍历过程中
  { 
  	if (task == &init_task ) 	//重新回到初始地址，退出
  		return NULL;
  	return (void*)pos	;//否则返回一个非零值
  }  
}
static int my_seq_show(struct seq_file *m, void *v)
{//获取进程的相关信息
  //printk(KERN_INFO"Invoke show\n");					
  seq_printf( m,  "#%-3d ", taskcounts++ );
  seq_printf( m,  "%5d ", task->pid );
  seq_printf( m,  "%lu ", task->state );
  seq_printf( m,  "%-15s ", task->comm );
  seq_printf( m,  "%-5d ", task->prio); //优先级
  seq_printf( m,  "%-5d ", task->static_prio);
  seq_printf( m,  "%-5d ", task->normal_prio);
  seq_printf( m,  "%-5d ", task->rt_priority);
  seq_printf( m,  "%u ", task->policy);//调度策略
  seq_printf( m,  "%lu ", task->nvcsw);//上下文切换
	seq_printf( m,  "%lu ", task->nivcsw);
	seq_printf( m,  "%lu ", task->utime);//运行时间
	seq_printf( m,  "%lu ", task->stime);
	seq_printf( m,  "%lu ", task->real_start_time.tv_sec);
  seq_puts( m, "\n" );	  				  
  return 0; 
}

static void * my_seq_next(struct seq_file *m, void *v, loff_t *pos)
{
  //printk(KERN_INFO"Invoke next\n");  
  (*pos)++;    
  task= next_task(task);  //指向下一个进程
  return NULL;

}
static void my_seq_stop(struct seq_file *m, void *v)
{
	//printk(KERN_INFO"Invoke stop\n");		
	// do nothing        
}

static struct seq_operations my_seq_fops = {//序列文件记录操作函数集合
        .start  = my_seq_start,
        .next   = my_seq_next,
        .stop   = my_seq_stop,
        .show   = my_seq_show
};

static int my_open(struct inode *inode, struct file *file)  
{  
    return seq_open(file, &my_seq_fops); //打开序列文件并关联my_seq_fops
}  


static const struct file_operations my_proc =   
{  //proc文件操作函数集合
	.owner      = THIS_MODULE,  
	.open       = my_open,
	.read       = seq_read,     
	.llseek     = seq_lseek,
	.release    = seq_release  	    
}; 

int __init my_init( void )
{
	struct proc_dir_entry* my_proc_entry;
	printk( "<1>\nInstalling \'%s\' module\n", modname );
	my_proc_entry = proc_create(modname, 0x644, NULL, &my_proc);//生成proc文件
	if (NULL == my_proc_entry)
	{
	    return -ENOMEM;
	}
	return	0;  //SUCCESS
}

void __exit my_exit( void )
{
	remove_proc_entry( modname, NULL );//删除proc文件
	printk( "<1>Removing \'%s\' module\n", modname );	
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL"); 

