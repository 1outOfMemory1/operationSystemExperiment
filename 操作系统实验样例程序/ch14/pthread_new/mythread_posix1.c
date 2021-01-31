/*filename mythread_posix1.c��չʾposix�߳����ɺ�����ʹ�÷���
*����: $ gcc -o mythread_posix1 mythread_posix1.c -lpthread
*ִ�У� ָ��һ���ۼӴ�����Ϊ����ֵ����100000000: $ ./mythread_posix1 100000000
*wirtten by Fang Sheng from SDUST, Oct.10 2014
*tested under Ubuntu 14.04LTS
*/

#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
pthread_t thread;
int counter=0;
int upper;

// �̺߳����������Ƕ�counter��ֵ�����ۼӣ�
//�̺߳���������ɸ����ӵĹ������������һ��ʾ��
void *my_thread()
{
	int	i, temp;
	printf ("I'm thread 1\n");
	for (i = 0; i < upper; i++) 
	{
		temp = counter;
		temp += 1;
		counter = temp;
	}
	printf("thread1 :Is Main function waiting for me ?\n");
	pthread_exit(NULL);

}

void thread_create()
{
	int temp;
	memset(&thread, 0, sizeof(thread)); 
	/*create the new thread*/
	if((temp = pthread_create(&thread, NULL, my_thread, NULL)) != 0) //�����߳�
		printf("Creating thread 1 has failed!\n");
	else
		printf("Thread 1 has been created! \n");	
	
}

void thread_wait(void)
{/*waiting for the thread finished*/	
	if(thread !=0) 
	{ 
		pthread_join(thread,NULL);//�ȴ��߳��˳�
		printf("Theread 1 has exited! \n");
	}	
}

int main(int argc, char *argv[])
{
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
	thread_create();
	printf("I am main function , I am waiting the threads finished! \n");
	thread_wait();
	printf("counter = %d\n",counter);
	return 0;
}
