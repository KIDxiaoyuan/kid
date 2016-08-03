/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: Wed 03 Aug 2016 08:28:23 GMT
 ************************************************************************/
#include<unistd.h>
#include<stdio.h>
#include<pwd.h>
#include<string.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<signal.h>

#define NORMAL  1
#define OUT_RED 2
#define IN_RED  3
#define PIPE    5
#define BCGRD   7

#define MAX_LEN 256

typedef struct node
{

    char command[MAX_LEN];
   // char argument[MAX_LEN];
    int flag;
    struct node *next;
}Node;

void setup(void);
char *get_line(void);

int main()
{
    while(1)
    {
    char *Input;
    Input=get_line();
    if(strcmp(Input,"exit") == 0||strcmp(Input,"logout") == 0)
        break;
    else
        {
            setup();

            
        }

    free(Input);
    }
    return 0;
}
void explain_input(char *buf,char arg[100][256])
{
    int i;
    char *p=buf;
    while(*p++ !='\0')
    {
        i=0
        while(*p == ' ')
        p++;
        while(*p !='<' && *p != '>' && *p != '|'&& *p != '&' && *p != ' ')
        {
            
        }

    }

}

void setup()
{
    signal(SIGINT,SIG_IGN);
    signal(SIGQUIT,SIG_IGN);
}

char * get_line(void)
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
    input=readline(output);
    if(input) {
        add_history(input);
    }
    return input;
}

