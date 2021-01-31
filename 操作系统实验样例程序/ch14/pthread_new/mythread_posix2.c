/*filename mythread_posix2.c：功能1）如何生成多个线程 2）演示多线程的并发；3）多线程的竞争访问；4）线程函数传递参数
*编译: $ gcc -o mythread_posix2 mythread_posix2.c -lpthread
*执行： 指定一个累加次数作为参数值: $ ./mythread_posix2 100000000
*wirtten by Fang Sheng from SDUST, Oct.10 2014
*tested under Ubuntu 14.04LTS
* 本程序用于说明两个问题：1）多个线程并发运行时，资源竞争会造成运行数据错误
* 2) 在线程间传递参数时，要考虑到执行上下文环境的改变，否则参数传递会出现错误。
*/
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#define  THREADNO 4
pthread_t thread[THREADNO];
int counter=0; //the shared variable among threads
int upper;
//线程函数
void *my_thread(void *args)
{	
	int thread_arg;
	int	i, temp;
	thread_arg = *(int *)args;//获取线程函数的参数
	printf ("I'm thread %d\n",thread_arg);
	for (i = 0; i < upper; i++) 
	{
		temp = counter;
		temp += 1;
		counter = temp;
	}
	printf("thread %d :Is Main function waiting for me ?\n",thread_arg);
	pthread_exit(NULL);

}
void thread_create(void)
{
	int i,temp;
	memset(&thread, 0, sizeof(thread)); 
	/*create new threads*/
	for(i=0; i<THREADNO;i++)
	{	
		//注意创建多个线程时使用的方法
		if((temp = pthread_create(&thread[i], NULL, my_thread, &i)) != 0) //注意传递线程参数的方法
			printf("Creating thread %d has failed!\n",i);
		else
			printf("Thread %d has been created! \n",i);	
	}
}
void thread_wait(void)
{
	/*waiting for the thread finished*/
	int i;
	for(i=0; i<THREADNO;i++)
	{	//注意等待多个线程退出的方法
		if(thread[i] !=0) 
		{ 
			pthread_join(thread[i],NULL);
			printf("Theread %d has exited! \n",i);
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc != 2) 
	{
		fprintf(stderr,"usage: mythrd-posix2 integer_value\n");			
		return -1;
	}
	if (atoi(argv[1]) < 0) 
	{
		fprintf(stderr,"Argument %d must be non-negative\n",atoi(argv[1]));		
		return -1;
	}
	upper = atoi(argv[1]);
	printf("I am main function, I am creating the threads! \n");
	thread_create();
	printf("I am main function , I am waiting the threads finished! \n");
	thread_wait();
	printf("counter = %d\n",counter);
	return 0;
}
