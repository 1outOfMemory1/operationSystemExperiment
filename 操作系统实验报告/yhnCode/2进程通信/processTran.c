#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1
#define N 21

typedef struct Student {
    int no;
    char name[N];
    char introduction[N];
}Student;

// 使用无名管道实现两个进程之间的通信
int main() {
    Student stu1, stu2;
    int length = sizeof(Student);
    pid_t pid;
    int fd[2];
    // 创建管道
    if (pipe(fd) == -1) {
        fprintf(stderr, "Pipe failed!\n");
        return 1;
    }
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork failed!\n");
        return 1;
    }
    // 父进程
    if (pid > 0) {
        // 关闭不使用的读端
        close(fd[READ_END]);
        printf("父进程（pid=%d）：请输入数据 学号 姓名与简历\n", getpid());
        // 从键盘读入数据
        scanf("%d %s", &stu1.no, &stu1.name);
        getchar();
        fgets(stu1.introduction, N, stdin);
        // 向管道中写入数据
        write(fd[WRITE_END], &stu1, length);
        // 关闭管道的写端
        close(fd[WRITE_END]);
        // 等待子进程
        wait(NULL);
    } else {
        // 子进程
        // 关闭不使用的写端
        close(fd[WRITE_END]);
        // 从管道中读取数据
        read(fd[READ_END], &stu2, length);
        printf("子进程（pid=%d）：接收到数据\n", getpid());
        printf("学号：%d\n", stu2.no);
        printf("姓名：%s\n", stu2.name);
        printf("个人简历：%s\n", stu2.introduction);
        
        close(fd[READ_END]);
    }
    return 0;
}