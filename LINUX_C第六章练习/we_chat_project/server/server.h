/*************************************************************************
	> File Name: server.h
	> Author: 
	> Mail: 
	> Created Time: 2016年08月15日 星期一 09时22分19秒
 ************************************************************************/

#ifndef _SERVER_H
#define _SERVER_H
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sqlite3.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<ctype.h>
#include<pthread.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<time.h>

#define MAXLEN 1000
#define MAX_LINE 80
#define MAX_LEN 20
#define portnumber 6666//端口号
#define REG_OK 1
#define REG_FAULT 0
#define LOG_USER 0
#define LOG_ADMIN 1
#define LOG_FAULT -1

/*用户结构体*/
typedef  struct struct_clientinf
{
    char name[MAX_LEN];
    char passwd[MAX_LEN];
    int decr;
    struct sockaddr_in add_in;
    pthread_t pid;
    int speak;
    struct User_list *friend_list;
    struct Group_list *group_list;
}clientinf;
typedef clientinf datatype;

/*在线成员链表结构体*/
typedef struct _LNode
{
    datatype data;
    struct _LNode *next;
}LNode,*LinkList;

/*用户链表结构体*/
typedef struct u_node
{
    char name[MAX_LEN];
    struct u_node *next;
}User_list;

/*消息结构体*/
struct message
{
    char flag[15];
    char name[MAX_LEN];
    char msg[MAXLEN];
    char addressee[MAX_LEN];
    int size;
};

/*群成员结构体*/
typedef struct Group_Node
{
    char name[MAX_LEN];
    char admin[MAX_LEN];
    struct _LNode head;    
}group;

/*群链表结构体*/
typedef struct _GNode
{
    char name[MAX_LEN];
    struct _GNode *next;
}Group_list;
LinkList clientlink;
#endif
