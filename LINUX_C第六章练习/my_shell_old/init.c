#include "init.h"
#include "externs.h"
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <pwd.h>
#include<unistd.h>
#define MAX_LEN 256
void sigint_handler(int sig);
void Sig(void);

void setup(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void sigint_handler(int sig)
{
	Sig();
	//printf("\n[minishell]$ ");
	fflush(stdout);
}

void init(void)
{
	memset(cmd, 0, sizeof(cmd));
	int i;
	for (i=0; i<PIPELINE; ++i)
	{
		cmd[i].infd = 0;
		cmd[i].outfd = 1;
	}
	memset(cmdline, 0, sizeof(cmdline));
	memset(avline, 0, sizeof(avline));
	lineptr = cmdline;
	avptr = avline;
	memset(infile, 0, sizeof(infile));
	memset(outfile, 0, sizeof(outfile));
	cmd_count = 0;
	backgnd = 0;
	append = 0;
	lastpid = 0;
        Sig();
 //   printf("?");
	//printf("[minishell]$ ");
	fflush(stdout);
}
void Sig(void)
{
    int len;
    char user [32];
    char pc[32];
    char lod[MAX_LEN];
    getcwd(lod,MAX_LEN);
    char output[MAX_LEN];
    char welcome[MAX_LEN];
    struct passwd *pwd;
    char *p;
    char *input;
    pwd=getpwuid(getuid());
    strcpy(welcome,pwd->pw_name);
    len = strlen(welcome);
    welcome[len]='@';
    p=welcome+len+1;
    gethostname(p,32);
    strcpy(output,welcome);
    len = strlen(output);
    output[len] = ':';
    p=output+len +1;
    strcpy(p,lod);
    len = strlen(output);
    output[len]='>';
    output[len+1]='\0';
   // input=readline(output);
    //if(input) {
     //   add_history(input);
    //}
    //return input;
    printf("%s",output);
}
