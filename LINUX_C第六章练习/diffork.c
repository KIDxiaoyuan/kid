/*************************************************************************
	> File Name: diffork.c
	> Author: 
	> Mail: 
	> Created Time: Fri 29 Jul 2016 14:10:07 GMT
 ************************************************************************/
#include<stdlib.h>
#include<sys/wait.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
int globVar = 5;
int main()
{
    pid_t pid;
    int var =1,i;
    printf("fork is different with vfork\n");
    pid=vfork();
    switch(pid)
    {
     case 0:
    {
        i=3;
        while(i-- >0)
        {
            printf("Child process is runing \n");
            globVar++;
            var++;
            sleep(5);
        }
        printf("Child : globVar = %d ,var = %d \n",globVar,var);
        break;
    }
     case -1:
        perror("process Faild \n");
        exit(0);
     default:
        {
            int wait_pid;
            wait_pid=wait(NULL);
            printf("wait_pid:%d\n",wait_pid);
            i=5;
            while (i-- >0)
            {
                printf("parent process runing\n");
                globVar++;
                var++;
                sleep(5);
            }
            
            printf("parent : globVar = %d, var = %d\n",globVar,var);
        }

    }
    //printf("final : globVar = %d, var = %d\n",globVar,var);
    exit(0);

}
