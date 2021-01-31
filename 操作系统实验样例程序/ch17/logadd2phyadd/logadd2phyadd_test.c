/*logadd2phyadd_test.c
本程序用于测试开发的logadd2phyadd内核模块，必须在logadd2phyadd_test内核模块编译、添加后运行。
程序编译命令： $ gcc -o logadd2phyadd_test logadd2phyadd_test.c
*/


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#define BUFSIZE 4096
char buf[BUFSIZE];
typedef struct data {
	unsigned long addr;
	int p;
}mydata;
#define PAGE_MASK 0xfffff000

int main() 
{ 
	unsigned long tmp,addr;
	int fd,len;
	mydata wdata;
	tmp = 0x12345678;
	addr = &tmp;
        printf("tmp value is : 0x%08lX\n", tmp);
	printf("tmp address is: 0x%08lX\n", addr);
	wdata.addr = addr;
	wdata.p = getpid();
	printf("the pid is %d\n",getpid());	
	fd = open("/proc/logadd2phyadd",O_RDWR);	
	write(fd,&wdata,sizeof(mydata));
	len = read(fd, buf, BUFSIZE);
        printf("the read length is %d and the buf content is: \n%s\n",len,buf);
	sleep(2);
	close(fd);
	return 0;
} 

