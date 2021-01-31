/*cmos_test.c
本程序用于测试开发的cmos内核模块，必须在cmos内核模块编译、添加、添加设备节点并修改权限后运行。
程序编译命令： $ gcc -o cmos_test cmos_test.c
*/

#include <unistd.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <fcntl.h>  
#include <linux/fb.h>  
#include <sys/mman.h>  
#include <sys/ioctl.h>   
  
#define PAGE_SIZE 4096  
#define BUFSIZE 8192
  
char *day[] = { "", "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN" };
char *month[] = { "", "JAN", "FEB", "MAR", "APR", "MAY", "JUN",
			"JUL", "AUG", "SEP", "OCT", "NOV", "DEC" };
  
int main(int argc , char *argv[])  
{  
    int fd1;  
    int fd2; char buf[BUFSIZE]; int len; char bufw[10]; unsigned char year;
    char cmos[128];	
    int i=0;   int	month_index; int ch; 
    unsigned char *p_map;  
      
    //打开设备  
    fd1 = open("/dev/cmos",O_RDWR);  
    if(fd1 < 0)  
    {  
        printf("open fail\n");  
        exit(1);  
    }  
    while (i<10) {	 
    	len = read(fd1, &ch, 1);	
	if ( len < 0 ) { perror( "read" ); exit(1); }
	buf[i] = ch;	
	i++;
    }  	
    printf("\n\t CMOS Real-Time Clock/Calendar:" );
    printf(" %02X", buf[4] );	// current hour
    printf(":%02X", buf[2] );	// current minutes
    printf( ":%02X", buf[0] );	// current seconds
    printf(" on" );
    printf( " %s, ", day[ buf[6] ] ); 	// day-name
    printf( "%02X", buf[7] );		// day-number
    month_index = ((buf[ 8 ] & 0xF0)>>4)*10 + (buf[ 8 ] & 0x0F);
    printf( " %s", month[ month_index ] );	// month-name
    printf(  " 20%02X\n ", buf[9] );		// year-number	

    

   
    lseek(fd1, 9, SEEK_SET);   
    year = (unsigned char)6;
    sprintf(bufw,"%s",&year);	
    write(fd1,bufw,strlen(bufw));

    lseek(fd1, 0, SEEK_SET);	
    i=0;
    while (i<10) {	 
    	len = read(fd1, &ch, 1);	
	if ( len < 0 ) { perror( "read" ); exit(1); }
	buf[i] = ch;	
	i++;
    }  	
    printf("\n\t CMOS Real-Time Clock/Calendar:" );
    printf(" %02X", buf[4] );	// current hour
    printf(":%02X", buf[2] );	// current minutes
    printf( ":%02X", buf[0] );	// current seconds
    printf(" on" );
    printf( " %s, ", day[ buf[6] ] ); 	// day-name
    printf( "%02X", buf[7] );		// day-number
    month_index = ((buf[ 8 ] & 0xF0)>>4)*10 + (buf[ 8 ] & 0x0F);
    printf( " %s", month[ month_index ] );	// month-name
    printf(  " 20%02X\n ", buf[9] );		// year-number


    lseek(fd1, 9, SEEK_SET);
    //ssize_t write(int fd, const void *buf, size_t count);
    year = (unsigned char)15;
    sprintf(bufw,"%s",&year);	
    write(fd1,bufw,strlen(bufw));
    lseek(fd1, 0, SEEK_SET);	
    i=0;
    while (i<10) {	 
    	len = read(fd1, &ch, 1);	
	if ( len < 0 ) { perror( "read" ); exit(1); }
	buf[i] = ch;	
	i++;
    }  	
    printf("\n\t CMOS Real-Time Clock/Calendar:" );
    printf(" %02X", buf[4] );	// current hour
    printf(":%02X", buf[2] );	// current minutes
    printf( ":%02X", buf[0] );	// current seconds
    printf(" on" );
    printf( " %s, ", day[ buf[6] ] ); 	// day-name
    printf( "%02X", buf[7] );		// day-number
    month_index = ((buf[ 8 ] & 0xF0)>>4)*10 + (buf[ 8 ] & 0x0F);
    printf( " %s", month[ month_index ] );	// month-name
    printf(  " 20%02X\n ", buf[9] );		// year-number		
		
    close(fd1); 
    return 0;  
}  

