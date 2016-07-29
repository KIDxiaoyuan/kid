/*************************************************************************
	> File Name: fork1.c
	> Author: 
	> Mail: 
	> Created Time: Fri 29 Jul 2016 08:48:51 GMT
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
int main()
{
    pid_t pid;
    int num=3;
    char *msg;
    puts("progress creat");
    pid = fork();  
    switch(pid)
    {
        case 0:
        printf("child : childpid : %d\tpreantpid :%d\n",pid,getpid());
        msg="child process is runing";
        break;

        case -1:
        perror("error:");
        break;
        default:
        msg="parent progress runing";
        printf("preant progress : childpid :%d\tpreantpid%d\n",pid,getppid());
        break;
        
    }
    while (num>0)
    {
        puts(msg);
        sleep(1);
        num--;
    }
    
    return 0;
}
