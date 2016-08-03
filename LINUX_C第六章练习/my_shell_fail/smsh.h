/*************************************************************************
	> File Name: smsh.h
	> Author: 
	> Mail: 
	> Created Time: Mon 01 Aug 2016 15:23:29 GMT
 ************************************************************************/

#ifndef _SMSH_H
#define _SMSH_H

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>
#include<sys/wait.h>
#include<ctype.h>

#define DFL_PROMPT ">"
#define is_delim(x) ((x) == ' '|| (x) == '\t')
#define YES   1
#define NO    0
#define MAXVARS 200

struct var{
    char *str;
    int global;
};
static struct var tab[MAXVARS];
static char * new_string(char *,char *);
static struct var *find_item(char *,int );
static char ** environ = NULL;

void    setup(void);
void    fatal(char *s1,char *s2,int n);
int     execute(char *argv[]);
char *  next_cmd(char *prompt,FILE *fp);
char ** splitline(char *line);
char *  newstr(char *s , int l);
void    freelist(char ** list);
void *  emalloc(size_t n);
void *  erealloc(void *p,size_t n);
int     is_control_command(char *);
int     do_control_command(char **);
int     ok_to_execute(void);
int     process(char **);
int     syn_err(char *);
int     builtin_command(char **args,int *resultp);
int     assign(char *str);
int     okname(char *str);
int     VLstore(char *name ,char *val);
char *  new_string(char *,char *);
char *  VLlookup(char *);
int     VLexport(char *);
static  struct var * find_item(char * ,int );
void    VLlist(void);
int     VLenviron2table(char *env[]);
char ** VLtable2environ(void);

#endif
