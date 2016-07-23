/*************************************************************************
	> File Name: teststat.c
	> Author: 
	> Mail: 
	> Created Time: Thu 21 Jul 2016 16:41:53 GMT
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/unistd.h>
#include<errno.h>

int main(int argc,char *argv[])
{
    struct stat buf;
    if(stat(argv[1],&buf)==-1)
    {
        perror("stat");
      //  printf("%s",filename);
    }
        
}
