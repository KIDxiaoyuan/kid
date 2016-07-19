/*************************************************************************
	> File Name: test_umask.c
	> Author: 
	> Mail: 
	> Created Time: Wed 20 Jul 2016 11:07:41 GMT
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
int main()
{
    umask(0);//不屏蔽任何权限
    if(creat("example_681.test",S_IRWXU|S_IRWXG|S_IRWXO)<0)
    {
        perror("creat:");
        exit(0);
    }
    umask(S_IRWXO);
    if(creat("example_682.test",S_IRWXU|S_IRWXG|S_IRWXO)<0)
    {
        perror("creat:");
        exit(1);
    }
    return 0;
}
