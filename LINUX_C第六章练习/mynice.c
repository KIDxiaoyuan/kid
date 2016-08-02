/*************************************************************************
	> File Name: mynice.c
	> Author: 
	> Mail: 
	> Created Time: Sat 30 Jul 2016 16:11:52 GMT
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/resource.h>
#include<sys/time.h>
#include<sys/wait.h>
#include<stdlib.h>
int main(void)
{
    pid_t pid;
    int stat_val;
    int oldpri,newpri;

    printf("nice study\n");

    pid = fork();
    switch(pid)
    {
        case 0 :
        printf("Child is runing curpid = %d , parent pid = %d\n",pid,getppid());
        oldpri = getpriority(PRIO_PROCESS,0);
        printf("old priority = %d\n",oldpri);
        newpri = nice(2);
        printf("New priority = %d\n",newpri);
        exit(0);
        case -1:
        perror("Process creation failed \n");
        break;
        default:

        printf("child pid is %d,parent pid is %d \n",pid ,getpid());
        break;
    }
    wait(&stat_val);
    exit(0);

}
