#include <unistd.h>  
#include <stdio.h>  
int main(void)  
{  
   int i=0;  
   printf("i son/pa ppid pid  fpid\n");  
   //ppid指当前进程的父进程pid  
   //pid指当前进程的pid,  
   //fpid指fork返回给当前进程的值  
   for(i=0;i<2;i++){  
       pid_t fpid=fork();  
       if(fpid==0)  
           printf("%d child  %4d %4d %4d\n",i,getppid(),getpid(),fpid);  
       else  
           printf("%d parent %4d %4d %4d\n",i,getppid(),getpid(),fpid);  
   }  
   return 0;  
}  

/* 输出样例
i son/pa ppid pid  fpid
0 parent 17959 18329 18330
1 parent 17959 18329 18331
0 child  18329 18330    0
1 child  18329 18331    0
1 parent 18329 18330 18332
1 child     1 18332    0


更改为

i son/pa ppid pid  fpid
0 parent 17959 18329 18330
0 child  18329 18330    0
1 parent 17959 18329 18331
1 parent 18329 18330 18332
1 child  18329 18331    0
1 child     1 18332    0


 */