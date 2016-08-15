服务端代码
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<sys/socket.h>::q:
#include<sys/un.h>
#include<unistd.h>
#include<semaphore.h> //这里没有用二进制信号量可以删掉
 
 char buffer[1024]; //读写用的区域

void *pthread_function(void *arg); //线程入口函数声明
pthread_mutex_t work_mutex; //声明互斥锁

int main(){
    int result;  //整数变量用来储存调用函数的返回值
    struct sockaddr_un server_address, client_address;  //UNIX域的套接字，server_address用于服务端的监听，client_address用于客户端连接后的套接字
    int client_len; //连接后，accept函数会把客户端的地址的长度储存在这
    int server_socketfd, client_socketfd;//服务端和客户端的套接字文件描述符
    pthread_t a_thread; //线程ID标志
    pthread_attr_t thread_attr; //线程的属性，后面可以看的，被我注释掉了，没用到，可以删掉。
    
    result = sem_init(&bin_sem, 0, 1); //初始化二进制信号量，因为用了互斥锁，所以没用到，可以删掉
    if(result != 0){
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    result = pthread_mutex_init(&work_mutex, NULL);//初始化互斥锁
    if(result != 0){
        perror("pthread_mutex_init");
        exit(EXIT_FAILURE);
    }

    server_socketfd = socket(AF_UNIX, SOCK_STREAM, 0);//创建套接字，用TCP连接方式，出于演示目的只用UNIX域套接字。

    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, "server_socket");

    unlink("server_socket"); //在绑定之前，把以前存在当前目录下的套接字删除

    result = bind(server_socketfd, (struct sockaddr*)&server_address, sizeof(server_address)); //绑定
    if(result != 0){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    result = listen(server_socketfd, 5);//监听，最多允许5个连接请求
    if(result != 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    client_len = sizeof(client_address);
    while(1){    //开始进入无限循环
/*        printf("If you want to quit, please enter 'quit'\n");
        printf("Do you want to accept a connectiong\n");
        memset(buffer, '\0', sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);
        if((strncmp("quit", buffer, 4))==0) break; */

        client_socketfd = accept(server_socketfd, (struct sockaddr*)&client_address, &client_len); //接受一个连接请求

/*        result = pthread_attr_init(&thread_attr);
        if(result != 0){
            perror("pthread_attr_init");
            exit(EXIT_FAILURE);
        }
        result = pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
        if(result != 0){
            perror("pthread_attr_setdetachstate");
            exit(EXIT_FAILURE);
        } */
        result = pthread_create(&a_thread, NULL, pthread_function, (void *)client_socketfd); //成功接受一个请求后，就会创建一个线程，然后主线程又进入accept函数，如果此时没有连接请求，那么主线程会阻塞
        if(result != 0){
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
        
    }    
}

void *pthread_function(void *arg){  //线程入口函数，每调用一次pthread_create，都会创建一个新的线程
    int fd = (int) arg; //把函数参数，即连接成功后的套接字，赋给fd.
    int result;
    fd_set read_fds;  //文件描述符集合，用于select函数
    int max_fds;    //文件描述符集合的最大数

    printf("%d id has connected!!\n", fd);
    while (1){
        
        FD_ZERO(&read_fds);//清空集合
        FD_SET(0, &read_fds);//将标准输入放入监听的文件描述符集合， 这个用于读取标准输入，即键盘的输入
        FD_SET(fd, &read_fds);//将连接后的客户文件描述符放入监听的文件描述符集合， 这个用于向客户端读取数据
        max_fds = fd + 1;
        
//        sem_wait(&bin_sem);
        pthread_mutex_lock(&work_mutex);  //对关键区域上锁
        printf("%d has get the lock\n", fd);
        result = select(max_fds, &read_fds, (fd_set *)NULL, (fd_set *)NULL, (struct timeval*)NULL); //开始监听那些文件描述符出于可读状态
        if(result < 1){
            printf("select");
        }
        if(FD_ISSET(0, &read_fds)){ //如果标准输入处于可读状态，说明键盘有所输入，将输入的数据存放在buffer中，然后向客户端写回，如果输入“quit”将会退出一个聊天线程
            memset(buffer, '\0', sizeof(buffer)); //保险起见，清零
            fgets(buffer, sizeof(buffer), stdin);
            if((strncmp("quit", buffer, 4))==0){
                printf("You have terminaled the chat\n");
            //    sem_post(&bin_sem);
                pthread_mutex_unlock(&work_mutex);
                break;
            }
            else{
                result=write(fd, buffer, sizeof(buffer));
                if(result==-1){
                    perror("write");
                    exit(EXIT_FAILURE);
                }
            }
        }
        if(FD_ISSET(fd, &read_fds)){  //如果客户套接字符可读，那么读取存放在buffer中，然后显示出来，如果对方中断聊天，那么result==0
            memset(buffer, '\0', sizeof(buffer));
            result = read(fd, buffer, sizeof(buffer));
            if(result == -1){
                perror("read");
                exit(EXIT_FAILURE);
            }
            else if(result == 0){
                printf("The other side has terminal the chat\n");
            //    sem_post(&bin_sem);
                pthread_mutex_unlock(&work_mutex);
                break;
            }
            else{
                printf("receive message: %s", buffer);
            }
        }
        pthread_mutex_unlock(&work_mutex); //解锁
        sleep (1); //如果没有这一行，当前线程会一直占据buffer.让当前线程暂停一秒可以实现1对N的功能。
//        sem_post(&bin_sem);    
//        sleep (1);    
    }
//    printf("I am here\n");
    close(fd);
    pthread_exit(NULL);
    
}
