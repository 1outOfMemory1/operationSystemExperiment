/**
 * This program forks a separate process using the fork()/exec() system calls.
 *�������$ gcc -o newproc_posix2  newproc_posix2.c
 *�������$ ./newproc_posix2
 * ��ע��۲���A������
 * ���ȥ��ǰ���ע�ͣ���B�ܷ�ִ�У�
 * ֮����
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
		execlp("/bin/cat","cat", buf,NULL); //��A
		//	printf("I am Child %d\n",getpid());	//��B
	}
	else { /* parent process */
		/* parent will wait for the child to complete */
		printf("I am the parent %d and creae the child %d\n",getpid(),pid);
		sleep(40);  // Ϊ������cat /proc/������pid/maps
		wait(NULL);		
		printf("Child Complete\n");		
	}
    
    return 0;
}
