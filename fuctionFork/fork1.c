#include <unistd.h>
#include <stdio.h> 
int main () 
{ 
	pid_t fpid; //fpid表示fork函数返回的值
	int count=0;
	fpid=fork(); 
	if (fpid < 0)  //如果返回值小于0 说明创建进程出错  错误原因有 内存空间不够 进程数超出上限
		printf("error in fork!"); 
	else if (fpid == 0) { //如果返回值是0 说明是子进程
		printf("i am the child process, my process id is %d\n",getpid()); 
		count++;
	}
	else { //如果返回值大于0 说明是父进程
		printf("i am the parent process, my process id is %d\n",getpid()); 
		count++;
	}
	printf("count的值为: %d\n",count);  //这里两个进程的count的值都是1 因为fork函数拷贝了父进程的所有状态
	return 0;
}
/* 样例输出
i am parent process, my process id is 50561
count的值为: 1
i am the child process, my process id is 50562
count的值为: 1
*/
