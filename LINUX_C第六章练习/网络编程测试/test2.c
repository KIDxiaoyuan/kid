/*************************************************************************
	> File Name: test2.c
	> Author: 
	> Mail: 
	> Created Time: Wed 10 Aug 2016 10:42:14 GMT
 ************************************************************************/

#include<stdio.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
void display_time(const char *string )
{
    int seconds;
    seconds = time((time_t *) NULL);
    printf("%s ,%d\n",string,seconds);
}
int main(void)
{
    fd_set readfds;
    struct timeval timeout;
    int ret;
    /*监视文件描述符是否从0开始输入，文件描述符0表示标准输入，*/
    FD_ZERO(&readfds);//使用一个文件描述符集合前通常先将其清空
    FD_SET(0,&readfds);
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    while(1)
    {
        display_time("before select");
        ret = select(1,&readfds,NULL,NULL,&timeout);
        display_time("after select");
        switch(ret)
        {
            case 0:
            printf("no data in ten seconds .\n");
            exit(0);
            break;
            case -1:
            perror("select");
            exit(0);
            break;
            default:
            getchar();
            printf("Data is available now \n");
        }
    }
    return 0;

}
