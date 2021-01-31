/**演示fork的作用
 * This program forks a separate process using the fork() system calls.
 *编译命令：$ gcc -o newproc_posix  newproc_posix.c
 *运行命令：$ ./newproc_posix
 * 请观察：1）程序中行A、B和C分别输出了几次？ 2）在行C的输出中，输出的值是多少？ 为什么？
 **/

#include <stdio.h>
#include <stdlib.h> //for malloc
#include <string.h> //for mesmset,strcpy
#include <unistd.h>
#include <sys/types.h>

int main()
{
	pid_t pid;
	pid_t pid2;
	int var=88;     
	char  *str = (char*)malloc(sizeof(char)*10);
	memset(str, 0x00, 10);         

	/* fork a child process */
	pid = fork();

	if (pid < 0) { /* error occurred */
		fprintf(stderr, "Fork Failed\n");
		return 1;
	}
	else if (pid == 0) { /* child process */
		printf("I am the child %d\n",pid); //行A
		pid2 = getpid();
		printf("I am the child %d\n",pid2);
		strcpy(str, "child");               
		var++;
		//sleep(50);
		}

	else { /* parent process */		
		pid2 = getpid();
		printf("I am the parent %d and creae the child %d\n",pid2,pid); //行B
		strcpy(str, "parent");
		//sleep(50); //为了cat /proc/{pid}/maps
		wait(NULL);/* parent will wait for the child to complete */		
		printf("Child Complete\n");		
	}
    printf("str=%s, strAdd=%p, var=%d, varAdd=%p\n", str, str, var, &var);//行C
    return 0;
}
