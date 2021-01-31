#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// 缓冲区
typedef struct Buffer{
    int *buf;
    int read_ptr;
    int write_ptr;
}Buffer;


Buffer buffer;
// 生产者n，消费者m，缓冲区大小b
int n, m, b;
// 产品编号
int product_id = 1;
int *p_array, *c_array;
int *thread_procuder_id;
int *thread_consumer_id;
// 互斥锁
pthread_mutex_t lock;
// 条件变量
pthread_cond_t not_full, not_empty;


// 生产者
void *p(void *args) {
    int idx = *(int*)args;
    int num = p_array[idx];
    int i;
    for (i = 0; i < num; i++) {
        pthread_mutex_lock(&lock);
        while ((buffer.write_ptr + 1) % b == buffer.read_ptr) {
            printf("生产者p%d阻塞\n", idx+1);
            pthread_cond_wait(&not_full, &lock);
            printf("生产者p%d被唤醒\n", idx+1);
        }
        printf("生产者p%d将第%d个产品放入仓库\n", idx+1, product_id);
        buffer.buf[buffer.write_ptr] = product_id++;
        buffer.write_ptr++;
        if (buffer.write_ptr >= b) {
            buffer.write_ptr = 0;
        }
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&lock);
    }
}


// 消费者
void *c(void *args) {
    int idx = *(int*)args;
    int num = c_array[idx];
    int i;
    for (i = 0; i < num; i++) {
        pthread_mutex_lock(&lock);
        while (buffer.write_ptr == buffer.read_ptr) {
            printf("消费者c%d阻塞\n", idx+1);
            pthread_cond_wait(&not_empty, &lock);
            printf("消费者c%d被唤醒\n", idx+1);
        }
        printf("消费者c%d从仓库消费第%d个产品\n", idx+1, buffer.buf[buffer.read_ptr]);
        buffer.read_ptr++;
        if (buffer.read_ptr >= b) {
            buffer.read_ptr = 0;
        }
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&lock);
    }
}

int main() {
    // 初始化
    buffer.read_ptr =  0;
    buffer.write_ptr = 0;
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&not_empty, NULL);
    pthread_cond_init(&not_full, NULL);
    scanf("%d%d%d", &n, &m, &b);
    b += 1;
    buffer.buf = (int*)malloc(sizeof(int) * b);
    p_array = (int*)malloc(sizeof(int) * n);
    c_array = (int*)malloc(sizeof(int) * m);
    thread_procuder_id = (int*)malloc(sizeof(int)*n);
    thread_consumer_id = (int*)malloc(sizeof(int)*m);
    int i;
    for (i = 0; i < n; i++) {
        thread_procuder_id[i] = i;
    }
    for (i = 0; i < m; i++) {
        thread_consumer_id[i] = i;
    }
    for (i = 0; i < n; i++) {
        scanf("%d", p_array+i);
    }
    for (i = 0; i < m; i++) {
        scanf("%d", c_array+i);
    }
    pthread_t *thread_producer = (pthread_t*)malloc(sizeof(pthread_t)*n);
    pthread_t *thread_consumer = (pthread_t*)malloc(sizeof(pthread_t)*m);
    // 创建生产者和消费者线程
    for (i = 0; i < n; i++) {
        pthread_create(thread_producer+i, NULL, p, (void*)(thread_procuder_id + i));
    }
    for (i = 0; i < m; i++) {
        pthread_create(thread_consumer+i, NULL, c, (void*)(thread_consumer_id + i));
    }
    // 主线程等待子线程执行完
    for (i = 0; i < n; i++) {
        pthread_join(thread_producer[i], NULL);
    }
    for (i = 0; i < m; i++) {
        pthread_join(thread_consumer[i], NULL);
    }
    // 释放资源
    free(thread_consumer_id);
    free(thread_procuder_id);
    free(thread_consumer);
    free(thread_producer);
    free(p_array);
    free(c_array);
    free(buffer.buf);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);
    pthread_mutex_destroy(&lock);
    return 0;
}