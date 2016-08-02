/*************************************************************************
	> File Name: smsh1.c
	> Author: 
	> Mail: 
	> Created Time: Mon 01 Aug 2016 14:07:20 GMT
 ************************************************************************/

#include"smsh.h"

//extern char ** environ;

int main()
{
    char *cmdline, *prompt, **arglist;
    int result;
    prompt = DFL_PROMPT;
    setup();

    while((cmdline = next_cmd(prompt, stdin))!=NULL)
    {
        if((strcmp(cmdline,"exit")==0)||(strcmp(cmdline,"logout")==0))
        {
            break;
        }
        if((arglist = splitline(cmdline))!=NULL)
        {
            result = process(arglist);
            freelist(arglist);
        }
        free(cmdline);
        fflush(stdin);
    }
    return 0;
}

/*
 * 信号处理函数
 */
 void setup()
 {
     //extern char **environ;
     VLenviron2table(environ);
     signal(SIGINT, SIG_IGN);
     signal(SIGQUIT, SIG_IGN);
 }

 /*
  * 程序错误处理函数
  */
  void fatal(char *s1,char *s2, int n)
  {
      printf("Error: %s :%s\n",s1,s2);
      exit(n);
  }
  
