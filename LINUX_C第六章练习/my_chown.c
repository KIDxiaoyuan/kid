/*************************************************************************
	> File Name: my_chown.c
	> Author: 
	> Mail: 
	> Created Time: Wed 20 Jul 2016 08:46:06 GMT
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
int main(int argc,char*argv[])
{
    struct stat buf;
    //检查参数个数
    if(argc!=4)
    {
        printf("argc:%d",argc);
        printf("Usage:my_chown <filename>\n");
        exit(0);
    }
    //获取文件属性
    if(stat(argv[1],&buf)==-1)
    {
        perror("stat:");
        exit(1);
    }
    if(chown(argv[1],(uid_t)atoi(argv[2]),(gid_t)atoi(argv[3]))==0)
    {
        printf("success change uid:%d->%s,pid%d_->%s \n",buf.st_uid,argv[2],buf.st_gid,argv[3]);
        return 0;
    }
    else
    {
        perror("chown:");
        exit(0);
    }
    return -1;
}
