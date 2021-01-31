/**��ʾfork������
 * This program forks a separate process using the fork() system calls.
 *�������$ gcc -o newproc_posix  newproc_posix.c
 *�������$ ./newproc_posix
 * ��۲죺1����������A��B��C�ֱ�����˼��Σ� 2������C������У������ֵ�Ƕ��٣� Ϊʲô��
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
		printf("I am the child %d\n",pid); //��A
		pid2 = getpid();
		printf("I am the child %d\n",pid2);
		strcpy(str, "child");               
		var++;
		//sleep(50);
		}

	else { /* parent process */		
		pid2 = getpid();
		printf("I am the parent %d and creae the child %d\n",pid2,pid); //��B
		strcpy(str, "parent");
		//sleep(50); //Ϊ��cat /proc/{pid}/maps
		wait(NULL);/* parent will wait for the child to complete */		
		printf("Child Complete\n");		
	}
    printf("str=%s, strAdd=%p, var=%d, varAdd=%p\n", str, str, var, &var);//��C
    return 0;
}
