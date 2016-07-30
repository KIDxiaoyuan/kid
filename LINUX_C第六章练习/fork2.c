/*************************************************************************
	> File Name: fork2.c
	> Author: 
	> Mail: 
	> Created Time: Fri 29 Jul 2016 09:21:27 GMT
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
    pid_t pid;
    pid = fork();
    switch(pid)
    {
        case 0:
        {
            int i=3;
            while (i-->0)
            {
                printf("A background process ,PID %d\n,ParentID:%d\n",getpid(),getppid());
                sleep(1);
            }
            exit (0);

        }
        case -1:
        {
            perror("error :");
            exit (-1);
        }
        default:
        {
            printf("Parent process : pid is %d\n",getpid());
            exit(0);
        }
    }
    return 0;
}
