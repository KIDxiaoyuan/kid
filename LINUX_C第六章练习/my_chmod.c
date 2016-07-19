/*************************************************************************
	> File Name: my_chmod.c
	> Author: 
	> Mail: 
	> Created Time: Wed 20 Jul 2016 22:32:51 GMT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
int main(int argc,char ** argv)
{
    unsigned mode;
    struct stat buf;
    if(stat(argv[1],&buf)==-1)
    {
        perror("stat:");
        return -1;
    }
    mode=buf.st_mode&S_ISUID;
    if(chmod(argv[1],mode)==-1)
    {
        perror("chmod:");
        return -1;
    }
    return 0;
}
