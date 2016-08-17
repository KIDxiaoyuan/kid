/*************************************************************************
	> File Name: client.h
	> Author: 
	> Mail: 
	> Created Time: Tue 16 Aug 2016 08:54:09 GMT
 ************************************************************************/

#ifndef _CLIENT_H
#define _CLIENT_H
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<signal.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
#include<errno.h>
#include<pthread.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<time.h>
#include<arpa/inet.h>

#define PORT 6666
#define MAXLEN 1000
#define OK 1
#define FAULT 0
//#define log 1
//#define reg 2
//#define ex  3
struct message//消息结构体
{
    char flag[15];
    char name[20];
    char msg [MAXLEN];
    char addressee[20];
    int size;
};
int qid = -1,fd = -1;
int sockfd = -1;
int savefilefd = -1;//保存文件描述符
char filefromname[20];//文件来源
char chat_log[100];//聊天记录名
pthread_mutex_t lock;//线程锁
char filefromuser[20];//文件发送者用户名
char locname[20];//本客户端用户名
#endif
