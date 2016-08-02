/*************************************************************************
	> File Name: studyuid.c
	> Author: 
	> Mail: 
	> Created Time: Sat 30 Jul 2016 15:07:13 GMT
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>
extern int errno ;

int main()
{
    int fd;
    printf("uid = %d ,euid = %d\n",getuid(),geteuid());
    if(fd=open("test.c",O_RDWR) == -1)
    {
        printf("error %d: %s",errno,strerror(errno));
        exit(1);
    }
    else 
    {
        printf("open successfullly\n");
    }
    close(fd);
    exit(0);
}
