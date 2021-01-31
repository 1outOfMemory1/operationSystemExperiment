#include <unistd.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <fcntl.h>  
#include <linux/fb.h>  
#include <sys/mman.h>  
#include <sys/ioctl.h>   
  
#define PAGE_SIZE 4096
#define MAX_SIZE 1024   

void show_vma() {
    FILE *fp;
	char buf[MAX_SIZE];
	if (!(fp = fopen("/proc/vma", "r"))) {
		printf("Open File Error!\n");
		exit(-1);
	}
	while (fgets(buf, MAX_SIZE, fp) != NULL) {
		printf("%s", buf);
	}
}
int main(int argc , char *argv[])  
{  
    int fd1;  
    unsigned char *p_map;  

    //打开设备  
    fd1 = open("/dev/mymap",O_RDWR);  
    if(fd1 < 0)  
    {  
        printf("open fail\n");  
        exit(1);  
    }
    show_vma();
    printf("\n\n----------\n\n");
    //内存映射  
    p_map = (unsigned char *)mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,fd1, 0);  
    if(p_map == MAP_FAILED)  
    {  
        printf("mmap fail\n");  
        goto out;  
    }  
  
    //打印映射后的内存中的内容      
    printf("%s\n",p_map);  
    printf("\n----------\n\n");
    show_vma();
  
out:  
    munmap(p_map, PAGE_SIZE);  
    return 0;  
}  