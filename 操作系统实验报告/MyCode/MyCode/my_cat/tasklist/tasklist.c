#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/slab.h>

char modname[] = "tasklist";
struct task_struct  *task;
int  taskcounts=0;

static void * my_seq_start(struct seq_file *m, loff_t *pos)
{
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
  seq_printf( m,  "#%-3d ", taskcounts++ );  
  seq_printf( m,  "%5d ", task->pid );
  seq_printf( m,  "%lu ", task->state );
  seq_printf( m,  "%-15s ", task->comm );
  seq_puts( m, "\n" );	  				  
  return 0; 
}

static void * my_seq_next(struct seq_file *m, void *v, loff_t *pos)
{ 
  (*pos)++;    
  task= next_task(task);  //指向下一个进程
  return NULL;

}
static void my_seq_stop(struct seq_file *m, void *v)
{
     
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
	my_proc_entry = proc_create(modname, 0x644, NULL, &my_proc);
	if (NULL == my_proc_entry)
	{
	    return -ENOMEM;
	}
	return	0;
}

void __exit my_exit( void )
{
	remove_proc_entry( modname, NULL );
	printk( "<1>Removing \'%s\' module\n", modname );	
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL"); 

