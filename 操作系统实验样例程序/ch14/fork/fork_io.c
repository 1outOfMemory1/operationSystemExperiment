//fork_io.c：本程序用于演示父子进程在io输出时，由于创建子进程时copy出现的现象
//编译命令：$ gcc -o fork_io  fork_io.c
//运行命令：$ ./fork_io
//
////////////////////////////////////////////////////
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
main() 
{ 
	int a; 
	int pid; 
	printf("AAAAAAAA");//print 1; 
	pid=fork(); 
	if(pid==0){
		printf("\n child\n");
	} 
	else if(pid>0){ 
		printf("\n parent\n");
	} 
	printf("BBBBBBB");//print 2; 父子进程都会打印; 
} 

