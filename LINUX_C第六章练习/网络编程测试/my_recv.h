/*************************************************************************
	> File Name: my_recv.h
	> Author: 
	> Mail: 
	> Created Time: Wed 10 Aug 2016 11:07:23 GMT
 ************************************************************************/

#ifndef _MY_RECV_H
#define _MY_RECV_H
#define BUFSIZE 1024
void my_err(const char *err_string,int line);
int my_recv(int conn_fd,char *data_buf,int len);
#endif
void my_err(const char *err_string , int line)
{
    fprintf(stderr,"line %d ",line);
    perror("err_string");
    exit(1);
}

int my_recv(int conn_fd ,char *data_buf , int len)
{
    static char recv_buf[BUFSIZE];
    static char *pread;
    static int len_remain = 0;
    int i;

    //若自定义缓冲区没有数据，则从套接字读取数据
    if(len_remain <= 0)
    {
        if((len_remain = recv(conn_fd,recv_buf,sizeof(recv_buf),0)) < 0)
        my_err("recv",__LINE__);
        else if (len_remain == 0)
        return 0;
        pread =recv_buf;
    }

    //从自定义缓冲区读取一次数据
    for(i = 0;*pread != '\n';i++)
    {
        if(i > len)
        return -1;//防止指针越界
        data_buf[i] = *pread++;
        len_remain --;
    }
    //除去结束标志
    len_remain --;
    pread ++;

    return i;//读取成功
}
