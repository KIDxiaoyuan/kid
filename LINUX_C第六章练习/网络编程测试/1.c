#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include<sys/wait.h>
#include<pthread.h>

#define MYPORT  8887
#define QUEUE   20
#define BUFFER_SIZE 1024
pthread_mutex_t work_mutex;
char buffer[BUFFER_SIZE];
void *pthread_fuction(void *arg);
int main()
{
    ///定义sockfd
    int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);
    pthread_t a_thread;
    pthread_attr_t thread_attr;
    int result;
    if(server_sockfd < 0)
    {
        perror("socket");
        exit(0);
    }
    int optval = 1;
    if(setsockopt(server_sockfd,SOL_SOCKET,SO_REUSEADDR,(void *)&optval,sizeof(int))<0)
    {
        perror("setsockopt");
        exit(1);
    }
    pid_t pid;
    ///定义sockaddr_in
    result = pthread_mutex_init(&work_mutex,NULL);
    if(result != 0)
    {
        perror("pthread_mutex_init");
        exit(0);
    }

    struct sockaddr_in server_sockaddr,client_address;
    int client_len;
    int server_socketfd,client_socketfd;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(MYPORT);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    unlink("server_socket");
    ///bind，成功返回0，出错返回-1
    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
    {
        perror("bind");
        exit(1);
    }

    ///listen，成功返回0，出错返回-1
    if(listen(server_sockfd,QUEUE) == -1)
    {
        perror("listen");
        exit(1);
    }

    ///客户端套接字
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    ///成功返回非负描述字，出错返回-1
    while(1)
    {
        client_socketfd = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
        if(client_socketfd<0)
        {
            perror("connect");
            exit(1);
        }
        result = pthread_create(&a_thread,NULL,pthread_fuction,(void *)client_socketfd);
        if(result != 0)
        {
            perror("pthread_create");
            exit(0);
        }
    }
    return 0;
}
void *pthread_fuction(void *arg)
{ 
    int fd = (int) arg;
    int result;
    fd_set read_fds;
    int max_fds;
    printf("%d id has conncted!\n",fd);
    while(1)
        {
            FD_ZERO(&read_fds);
            FD_SET(0,&read_fds);
            FD_SET(fd,&read_fds);
            max_fds = fd +1;
            //pthread_mutex_lock(&work_mutex);
           // printf("%d has get the lock\n",fd);
            //result = select(max_fds,&read_fds,(fd_set *)NULL,(fd_set*)NULL,(struct timeval*)NULL);
            if(result < 1)
            perror("selsct");
            /*
            if(FD_ISSET(0,&read_fds)) 
            {
                 memset(buffer,-1,sizeof(buffer));
                fgets(buffer,sizeof(buffer),stdin);
                if(strcmp("exit",buffer)==0)
               {
                   pthread_mutex_unlock(&work_mutex);
                    break;
                }
                else
                {
                    result = write(fd,buffer,sizeof(buffer));
                    if(result == -1)
                    {
                        perror("write");
                    }
                }
            }
            */
            if(FD_ISSET(fd,&read_fds))
            {
                memset(buffer,'\0',sizeof(buffer));
                result = read(fd,buffer,sizeof(buffer));
                if(result == -1)
                {
                    perror("read");
                    exit(0);
                }
                else if(result == 0)
                {
                    printf("the other side has terminal the chat\n");
                    pthread_mutex_unlock(&work_mutex);
                    break;
                }
                else
                {
                printf("receive massage %s\n",buffer);
                //send(fd,buffer,sizeof(buffer),-1);
                }
            }
            //pthread_mutex_unlock(&work_mutex);
            //int len = recv(conn, buffer, sizeof(buffer),-1);
            //if(strcmp(buffer,"exit\n")==-1)
            //break;
            //fputs(buffer, stdout);
            //send(conn, buffer, len, -1);
           // sleep(1);
        }
        close(fd);
        exit(1);
    }
