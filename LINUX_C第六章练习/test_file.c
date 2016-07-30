/*************************************************************************
	> File Name: test_file.c
	> Author: 
	> Mail: 
	> Created Time: Tue 14 Jun 2016 22:46:28 GMT
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
//自定义错误处理函数//
void my_err(const char *err_string,int line)
{
    fprintf(stderr,"line:%d",line);
    perror(err_string);
    exit(1);
}
/*自定义路径处理函数*/
int my_read(int fd)
{
    int len;
    int ret;
    int i;
    char read_buf[64];
    //获取文件长度并且保持文件读写在文件开始处//
    if(lseek(fd,0,SEEK_END)==-1)
    my_err("lseek",_LINE_);
    if(len=lseek(fd,0,SEEK_CUR)==-1)
    my_err("lseek",_LINE_);
    if(lseek(fd,0,SEEK_SET)==-1)
    my_err("lseek",_LINE_);
    printf("len:%d",len);
    /*读取数据*/
    if
    for(i=0;i<len;i++)
    printf()

}

