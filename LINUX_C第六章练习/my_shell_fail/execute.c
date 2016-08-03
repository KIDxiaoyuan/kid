/*************************************************************************
	> File Name: execute.c
	> Author: 
	> Mail: 
	> Created Time: Mon 01 Aug 2016 14:18:22 GMT
 ************************************************************************/

#include"smsh.h"
/*
 *命令运行函数，返回值为命令的结束状态
 *当返回-1时，程序执行错误，可能是fork()错误或wait()
 */
int execute (char *argv[])
{
    int pid;
    int child_info = -1;
    if(argv[0] == NULL)
    {
        return 0;
    }

    if((pid = fork()) == -1)
    {
        perror("fork");
    }
    else if (pid == 0)
    {
        environ = VLtable2environ();
        signal(SIGINT,SIG_DFL);
        signal(SIGQUIT,SIG_DFL);
        execvp(argv[0],argv);
        perror("cannot execute command");
        exit(1);
    }
    else
    {
        if(wait(&child_info )== -1)
        perror("wait");
    }
    return child_info;
}
