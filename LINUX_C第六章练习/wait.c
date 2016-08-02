/*************************************************************************
	> File Name: wait.c
	> Author: 
	> Mail: 
	> Created Time: Sat 30 Jul 2016 14:22:34 GMT
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
    pid_t pid;
    char *msg;
    int   k;
    int  exit_code;
    printf("Study how to get exit code \n");
    pid = fork();
    switch(pid)
    {
        case 0:
        msg = "Child process is running";
        k = 5;
        exit_code = 37;
        break;
        case -1:
        perror("error!\n");
        exit(1);
        default :
        exit_code = 0;
        break;
    }
    
    if (pid !=0)//父进程等待子进程结束
    {
        int stat_val;
        pid_t child_pid;
        child_pid = wait (&stat_val);
        printf("Child procee has exited ,pid = %d\n",child_pid);
        if (WIFEXITED(stat_val))
        {
            printf("Child exited with code %d\n",WEXITSTATUS(stat_val));
        }
        else 
        {
            puts("Child exited abnormally");
        }
    }
    else //子进程暂停5秒 在shell中查看父进程状态
    {
        while(k-- > 0)
        {
            puts(msg);
            sleep(1);
        }
    }
    exit(exit_code);
}
