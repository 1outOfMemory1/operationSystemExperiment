/*filename mythread_posix2m.c： 正确的线程参数传递方法。请对比mythread_posix2.c程序
*usage: $ gcc -o mythread_posix2m mythread_posix2m.c -lpthread
*usage: $ ./mythread_posix2m 100000000
*wirtten by Fang Sheng from SDUST, Oct.10 2014
*tested under Ubuntu 14.04LTS
* 本程序用于说明 在线程间传递参数时，要考虑到执行上下文环境的改变，否则参数传递会出现错误。
* 修正了mythread_posix2.c线程参数传递的错误
*/
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#define  THREADNO 4
pthread_t thread[THREADNO]; 
int counter=0; //the shared variable among threads
int upper;
int thread_id[THREADNO]; //线程参数数组

void *my_thread(void *args)
{	
	int thread_arg;
	int	i, temp;
	thread_arg = *(int *)args;
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
		if((temp = pthread_create(&thread[i], NULL, my_thread, (void*)(thread_id+i))) != 0) //注意参数传递使用的方法
		//if((temp = pthread_create(&thread[i], NULL, my_thread, (void*)(&i))) != 0)  
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
	{	
		if(thread[i] !=0) 
		{ 
			pthread_join(thread[i],NULL);
			printf("Theread %d has exited! \n",i);
		}
	}
}

int main(int argc, char *argv[])
{
	int i;
	if (argc != 2) 
	{
		fprintf(stderr,"usage: mythrd-posix1 integer_value\n");			
		return -1;
	}
	if (atoi(argv[1]) < 0) 
	{
		fprintf(stderr,"Argument %d must be non-negative\n",atoi(argv[1]));		
		return -1;
	}
	upper = atoi(argv[1]);
	printf("I am main function, I am creating the threads! \n");
        for(i=0; i<THREADNO; i++)
    	    thread_id[i] = i;
	thread_create();
	printf("I am main function , I am waiting the threads finished! \n");
	thread_wait();
	printf("counter = %d\n",counter);
	return 0;
}
