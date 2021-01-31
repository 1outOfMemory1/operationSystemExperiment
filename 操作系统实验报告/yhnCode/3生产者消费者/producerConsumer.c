
#include <stdio.h>
#include <pthread.h>
#include <unistd.h> 
#define BUFFER_SIZE 16 // 缓冲区数量
#define PRO_NO 30 // PRODUCING NO
#define OVER ( - 1) //生产结束标志
#define PPNO 3  // 生产者数量
#define CPNO 2  // 消费者数量
#define PSLEEP 100000  // 生产者随机睡眠时间
#define CSLEEP 100000  // 消费者随机睡眠时间

int thread_id1[PPNO] = {1,2,3};
int thread_id2[CPNO] = {4,5};


pthread_mutex_t lock; /* 互斥体lock 用于对缓冲区的互斥操作 */
pthread_cond_t notempty; /* 缓冲区非空的条件变量 */
pthread_cond_t notfull; /* 缓冲区未满的条件变量 */

struct prodcons
{// 缓冲区相关数据结构
	int buf[BUFFER_SIZE]; /* 实际数据存放的数组*/
	int readpos, writepos; /* 读写指针*/
};

struct prodcons buffer;

/* 初始化缓冲区结构 */
void init(struct prodcons *b)
{
	b->readpos = 0;
	b->writepos = 0;
}

/* 测试:生产者线程将0 到 PRO_NO的整数送入缓冲区,消费者线
程从缓冲区中获取整数,两者都打印信息*/

void *producer(void *data)
{
	int n=0;
	while(1){	
		pthread_mutex_lock(&lock);
		/* 等待缓冲区未满,应该用while判断，因为有可能发送虚假唤醒：期待的条件尚不成立的唤醒。*/
		while ((buffer.writepos + 1) % BUFFER_SIZE == buffer.readpos)
		// if ((buffer.writepos + 1) % BUFFER_SIZE == buffer.readpos) //会产生虚假唤醒
		{	
			pthread_cond_wait(&notfull, &lock);
		}
		/* 写数据,并移动指针 */
		buffer.buf[buffer.writepos] = n;
		printf("%d --->\n", n);
		n++;
		usleep(PSLEEP);
		buffer.writepos++;
		if (buffer.writepos >= BUFFER_SIZE)
			buffer.writepos = 0;
		/* 设置缓冲区非空的条件变量*/
		pthread_cond_signal(&notempty);
		pthread_mutex_unlock(&lock);
	} 
	return NULL;
}

void *consumer(void *data)
{
	int d;
	while (1)
	{	
		pthread_mutex_lock(&lock);
		while(buffer.writepos == buffer.readpos)
		{
			pthread_cond_wait(&notempty, &lock);
		}
		/* 读数据,移动读指针*/
		d = buffer.buf[buffer.readpos];
		//usleep(CSLEEP);
		buffer.readpos++;
		if (buffer.readpos >= BUFFER_SIZE)
			buffer.readpos = 0;
		/* 设置缓冲区未满的条件变量*/
		pthread_cond_signal(&notfull);
		pthread_mutex_unlock(&lock);
		printf("--->%d \n", d);
	}
	return NULL;
}




int main(void)
{
	//pthread_t th_c, th_p;
    pthread_t producerList[PPNO];
    pthread_t consumerList[CPNO];
	void *retval;
	int i;
	init(&buffer);
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&notempty, NULL);
	pthread_cond_init(&notfull, NULL);
	/* 创建生产者和消费者线程*/
    for (int i = 0 ;i < PPNO;i++){
        pthread_create(&producerList[i], NULL, producer, (void*)(thread_id1+i));
    }
    for (int j = 0 ;j < CPNO;j++){
        pthread_create(&consumerList[j], NULL, producer, (void*)(thread_id2+i));
    }
	
	//pthread_create(&th_p, NULL, consumer, 0);
	// /* 等待两个线程结束*/
    for (int i = 0 ;i < PPNO;i++){
        pthread_join(producerList[i], &retval);
    }
    for (int j = 0 ;j < CPNO;j++){
        pthread_join(consumerList[i], &retval);
    }

	
	// pthread_join(th_p, &retval);
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&notempty);
	pthread_cond_destroy(&notfull);
	return 0;
}
