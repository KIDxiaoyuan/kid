/*************************************************************************
	> File Name: spliitline.c
	> Author: 
	> Mail: 
	> Created Time: Mon 01 Aug 2016 14:28:50 GMT
 ************************************************************************/

#include"smsh.h"
/*
 * 参数处理函数：
 * 从输入流中读入一个命令，分配一块内存以接受任意参数个数的命令行。这个数组由NULL表示结束
 */
 char * next_cmd(char * prompt,FILE *fp)
 {
     char *buf;
     int bufspace = 0;
     int pos = 0;
     int c;

     printf("%s",prompt);
     while((c = getc(fp)) != EOF)
     {
         if(pos +1 >= bufspace)
         {
             if(bufspace == 0)
             {
                 buf = emalloc(BUFSIZ);
             }
             else
             {
                 buf = erealloc(buf,bufspace+BUFSIZ);
             }
             bufspace+=BUFSIZ;
         }
         if (c == '\n')
         break;
         buf[pos++] = c;
     }
         
    if(c == EOF && pos == 0)
    {
        return NULL;
    }
    buf[pos] = '\0';
    return buf; 
 }

 /*
  * 将一个字符串分解成字符串数组
  *
  */
char ** splitline(char * line)
{
    char **args;
    int spots = 0;
    int bufspace = 0;
    int argnum = 0;
    char *cp =line;
    char *start;
    int len;

    if (line == NULL)
    return NULL;
    args = emalloc(BUFSIZ);
    bufspace = BUFSIZ;
    spots = BUFSIZ/sizeof(char *);

    while(*cp != '\0')
   {
       while(is_delim(*cp))
       cp ++;
      if (*cp == '\0')
      break;

      if(argnum+1 >= spots)
      {
        args = erealloc(args,bufspace+BUFSIZ);
        bufspace+=BUFSIZ;
        spots +=(BUFSIZ/sizeof(char *));
      }
      start =cp;
      len =1;
      while(*++cp != '\0'&& !(is_delim(*cp)))
      {
        len ++;
      }
        args[argnum++] = newstr(start,len);
   }
    args[argnum] = NULL;
    return args;
}

char *newstr(char *s, int l)
{
    char *rv = emalloc(l+1);
    rv[1]= '\0';
    strncpy(rv,s,l);
    return rv;
}
/*
 * 释放所有字符串
 */
void freelist(char **list)
{
   char **cp = list;
    while(*cp)
    {
        free (*cp++);
    }
    free(list);
}
/*
 * 空间开辟函数
 */
 void *emalloc(size_t n)
 {
     void *rv;
     if((rv = malloc(n)) == NULL)
     {
         fatal("out of memory","",1);
     }
     return rv;
 }
 /*
  * 改变内存大小的指针函数
  */
  void * erealloc(void *p, size_t n)
  {
      void *rv;
      if((rv = realloc(p,n)) == NULL)
      fatal("realloc failed","",1);
      return rv;
  }

