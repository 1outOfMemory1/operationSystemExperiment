/**
 * This program forks a separate process using the fork()/exec() system calls.
 *编译命令：$ gcc -o newproc_posix2  newproc_posix2.c
 *运行命令：$ ./newproc_posix2
 * 请注意观察行A的作用
 * 如果去掉前面的注释，行B能否执行？
 * 之所以
 **/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
pid_t pid;
pid_t pid2;
char buf[128];

	/* fork a child process */
	pid = fork();

	if (pid < 0) { /* error occurred */
		fprintf(stderr, "Fork Failed\n");
		return 1;
	}
	else if (pid == 0) { /* child process */
		printf("I am the child %d\n",pid);
		pid2 = getpid();
		printf("I am the child %d\n",pid2);
		sprintf(buf, "/proc/%d/maps",pid2);
		execlp("/bin/cat","cat", buf,NULL); //行A
		//	printf("I am Child %d\n",getpid());	//行B
	}
	else { /* parent process */
		/* parent will wait for the child to complete */
		printf("I am the parent %d and creae the child %d\n",getpid(),pid);
		sleep(40);  // 为了运行cat /proc/父进程pid/maps
		wait(NULL);		
		printf("Child Complete\n");		
	}
    
    return 0;
}
