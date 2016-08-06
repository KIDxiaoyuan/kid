#include "execute.h"
#include "def.h"
#include "externs.h"
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <fcntl.h>
#include <string.h>
void forkexec(int i)
{
	pid_t pid;
    /*如果是kill命令，在链表中删除后台进程*/
         if(strcmp(cmd[i].args[0],"kill")==0)
        {
             NODE *bng=head->next;
             NODE *pre=head;
            while(bng!=NULL)
            {
                if(bng->npid == atoi(cmd[i].args[1]))
                {
                    NODE* nxt=bng->next;
                    pre->next=nxt;
                    free(bng);
                    break;
                }
                pre=bng;
                bng=bng->next;
            }
        }
    pid = fork();
    if (pid == -1)
    ERR_EXIT("fork");

	if (pid > 0)
	{
		/* 父进程 */
		if (backgnd == 1)
        {
            char *pointer;
              /*添加入jobs的链表*/
               NODE *p=(NODE*)malloc(sizeof(NODE));
               p->npid=pid;
               printf("%s",cmd[0].args[0]);
               pointer=p->backcn;
               strcpy(pointer,cmd[0].args[0]);
               NODE* tmp=head->next;
               head->next=p;
               p->next=tmp;
        }
		lastpid = pid;
        usleep(800);

	}
	else if (pid == 0)
	{
                 
		/* backgnd=1时，将第一条简单命令的infd重定向至/dev/null */
    	if (cmd[i].infd == 0 && backgnd == 1)
			cmd[i].infd = open("/dev/null", O_RDONLY);

		/* 将第一个简单命令进程作为进程组组长*/ 
		if (i == 0 && backgnd == 1)
			setpgid(0, 0);
		/* 子进程 */
		if (cmd[i].infd != 0)
		{
			close(0);
			dup(cmd[i].infd);
		}
		if (cmd[i].outfd != 1)
		{
			close(1);
			dup(cmd[i].outfd);
		}
                 
          
		int j;
		for (j=3; j<OPEN_MAX; ++j)
			close(j);


		if (backgnd == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
		}
        /*为ls添加颜色*/
        if(strcmp(cmd[i].args[0],"ls") == 0)
        {
            int temp = 0;
            while(cmd[i].args[temp])
            {
                temp ++;
            }
            cmd[i].args[temp] = "--color=auto";
        }
		if(execvp(cmd[i].args[0], cmd[i].args) == -1)
        {
            perror("execvp ");
        }
		exit(EXIT_FAILURE);
	}
}

int execute_disk_command(void)
{
	if (cmd_count == 0)
		return 0;


	if (infile[0] != '\0')
		cmd[0].infd = open(infile, O_RDONLY);

	if (outfile[0] != '\0')
	{
		if (append)
			cmd[cmd_count-1].outfd = open(outfile, O_WRONLY | O_CREAT
				| O_APPEND, S_IRUSR|S_IWUSR);
		else
			cmd[cmd_count-1].outfd = open(outfile, O_WRONLY | O_CREAT
				| O_TRUNC, S_IRUSR|S_IWUSR);
	}

	if (backgnd == 1)
		signal(SIGCHLD, SIG_IGN);
	else
		signal(SIGCHLD, SIG_DFL);

	int i;
	int fd;
	int fds[2];
	for (i=0; i<cmd_count; ++i)
	{
		/* 如果不是最后一条命令，创建管道 */
		if (i<cmd_count-1)
		{
			if(pipe(fds) == -1)
            {
                perror("pipe ");
                exit(0);
            }
			cmd[i].outfd = fds[1];
			cmd[i+1].infd = fds[0];
		}
                
		forkexec(i);

		if ((fd = cmd[i].infd) != 0)
			close(fd);

		if ((fd = cmd[i].outfd) != 1)
			close(fd);
	}

	if (backgnd == 0)
	{
		while (wait(NULL) != lastpid);
	}
	return 0;
}
