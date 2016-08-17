/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: Tue 16 Aug 2016 09:30:22 GMT
 ************************************************************************/
#include"client.h"

/////////////////////////函//数//声//明//区/////////////////////////////
int admin_chat(int sockfd,struct message *a);
int admin_kick(int sockfd,struct message *a);
int admin_screen(int sockfd,struct message *a);
int login_admin(struct message *a);
void handlerecvfile(struct message *msg);
void handlesendfile(void);
void handlerecvmsg(int *sockfd);
void cutStr(char str[],char left[], int n, char right[],int max, char c);
void common_use_words(char msg[]);
void expression(char name[],char msg[]);
int Interface(void);
int help(char str[]);
void log_user(struct message *a);
int login_success(struct message *a);
int Register(struct message *a);
/////////////////////////////////////////////////////////////////////

int main(int argc,char *argv[])
{
    int ret;
    int do_number;
    char str[MAXLEN];
    char buf[MAXLEN];
    char buf_new[100];
    struct message msg,a;
    struct sockaddr_in my_addr;
    time_t timep;
    enum action{log = 1,reg,ex};
    struct hostent *host;
    if(argc != 2)
    {
        fprintf(stderr,"Usage:%s hostname\a\n",argv[0]);
        exit(1);
    }
    //if((host = gethostbyname(argv[1]))==NULL)
    //{
    //    fprintf(stderr,"Gethostname error!\n");
    //    exit(1);
    //}
    while(1)
    {
        if((sockfd = socket(AF_INET,SOCK_STREAM,0))<0)
        {
            perror("socket ");
            exit(-1);
        }
        //设置套接字
        bzero(&my_addr,sizeof(struct sockaddr_in));
        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons(PORT);
        if (inet_aton(argv[1],&my_addr.sin_addr) == 0)
        {
        printf("error:inet_aton\n");
        exit(1);
        }
        //my_addr.sin_addr = *((struct in_addr*)host->h_addr);
        if(connect(sockfd,(struct sockaddr *)(&my_addr),sizeof(my_addr)) == -1)
        {
            puts("连接失败");
            exit(1);
        }
        do_number = Interface();
        switch(do_number)
        {
            case log:
            {
                int n =3;
                while(n)
                {
                    log_user(&a);
                    if(strcmp(a.msg,"hello,admin")==0)//管理员登陆成功
                    {
                        if(login_admin(&a) == 0)
                        {
                            return FAULT;
                        }
                    }
                    else if(strcmp(a.msg,"login,success!") == 0)
                    {
                        if(login_success(&a) == 0)
                        {
                            return FAULT;
                        }
                    }
                    else 
                    {
                        printf("你还有%d次机会!\n",--n);
                    }
                }
                close(sockfd);
                exit(3);
                break;
            }
            case reg:
            {
                Register(&a);
                break;
            }
            case ex:
            {
                close(fd);
                puts("退出");
                break;
            }
            default:
            {
                puts("无效的输入");
                break;
            }
        }
    }
close(sockfd);
return 0;
}
/***************************************************
函数名:admin_chat
功能:管理员聊天功能
传入参数:int sockfd,struct message *a
返回值:退出返回FAULT
***************************************************/
int admin_chat(int sockfd,struct message *a)  
{
    char str[MAXLEN];
    char buf[MAXLEN];
    time_t timep;
    sprintf(chat_log,"%s.txt",(*a).name);
    if((fd=open(chat_log,O_RDWR|O_CREAT|O_APPEND,0777)) < 0)
    {
        printf("打开聊天记录失败!");
	exit(1);
    }
    setbuf(stdin,NULL);
    strcpy((*a).flag,"all");
    printf("尊敬的%s你好，如需帮助请输入：.help\n",locname);
    while(1)
	{
		memset((*a).msg,0,strlen((*a).msg));
		memset(str,0,strlen(str));
		usleep(100000);
		printf("TO %s:\n",(*a).flag);
		setbuf(stdin,NULL);
		scanf("%s",str);
		if(OK == help(str))                      //提示信息  
		{
		    continue;
		}
		strcpy((*a).name,locname);
		strcpy(buf,(*a).flag);
		cutStr(str,(*a).flag,15,(*a).msg,MAXLEN,'$');    //调用字符切割函数  
		expression((*a).name,(*a).msg);               //表情替换函数 
		common_use_words((*a).msg);                    //常用语使用函数
		if(strcmp((*a).flag,"exit") == 0)
		{
		    return FAULT;
		}
		if(strcmp((*a).flag,"view") == 0)           
		{
		    send(sockfd,a,sizeof((*a)),0);       //请求查看在线用户
	     	strcpy((*a).flag,buf);
		    continue;
		}
		if(strcmp((*a).flag,"all") == 0)
		{
		    send(sockfd,a,sizeof(*a),0);        
		    continue;
		}
	        if ((strcmp((*a).flag,"trans") == 0) && (savefilefd <=0))
		{
    		    if ((strcmp((*a).msg,"agree") == 0) && (savefilefd == 0))
		    {
		        char savefilename[MAXLEN];
			strcpy((*a).addressee,filefromuser);
			printf("请输入你想保存的文件名：\n");
			do
			{
			    setbuf(stdin,NULL);
                scanf("%s",savefilename);
			    savefilefd = open(savefilename,O_RDWR|O_CREAT|O_EXCL,0777);
			    if(savefilefd == -1)
			    {
			        printf("文件已存在，你重新输入：\n");
			    }
			}while(savefilefd == -1);
			if(savefilefd < 0)
			{
			    printf("接收文件失败!\n");
			    savefilefd = -1;
			}
			else
			{
			    strcpy((*a).msg,"agree");
			    send(sockfd,a,sizeof(*a),0);
			    printf("文件接收中……\n");
			}
		}
			else
			{
				memset(str,0,strlen(str));
				cutStr((*a).msg,(*a).addressee,20,str,MAXLEN,'$');
				if (str[0] != '\0' && (*a).addressee[0] != '\0')
				{
					char transfileallname[22];
					sprintf(transfileallname,"./%s",str);
					savefilefd = open(str,O_RDWR,0666);
					if(savefilefd < 0)
					{
						printf("打开文件失败!\n");
						savefilefd = -1;
					}
        			else
					{
						memset((*a).msg,0,strlen((*a).msg));
						strcpy((*a).msg,str);
						send(sockfd,a,sizeof(*a),0);
					}
				}
				else
				{
					strcpy((*a).msg,"disagree");
					strcpy((*a).name,locname);
					strcpy((*a).addressee,filefromuser);
					send(sockfd,a,sizeof(*a),0);
				}
			}
			strcpy((*a).flag,buf);
			continue;
		}
		if (strcmp((*a).flag,"trans") == 0)
		{
			strcpy((*a).flag,buf);
		}
		else
		{
			strcpy(buf,(*a).flag);
			strcpy((*a).addressee,(*a).flag);
			strcpy((*a).flag,"personal");
			send(sockfd,a,sizeof(*a),0);             //发送私信
			strcpy((*a).flag,buf);
			time (&timep);
			memset(str,0,strlen(str));
			sprintf(str,"%s你对 %s 说: %s\n",ctime(&timep),(*a).flag,(*a).msg);
			printf("%s",str);
			write(fd,str,strlen(str));              //写入聊天记录文件中
		}
	}
}

/***************************************************
函数名:admin_kick
功能:管理员替人操作
传入参数:int sockfd,struct message *a
返回值:成功返回1，否则返回0
***************************************************/
int admin_kick(int sockfd,struct message *a)     
{
    char str[MAXLEN]; 
    do
    {
        printf("你想把谁踢出聊天室:\n");
	    setbuf(stdin,NULL);         //清空缓存
	    scanf("%s",str);                  
    }while(strcmp(str,"admin") == 0);                  
	if(strlen(str) == 0)           //str为空
	{
        return FAULT;
	}
	else
	{
        strcpy((*a).flag,"admin_kick");            //管理员踢人标志
        strcpy((*a).msg,str);
	    send(sockfd,a,sizeof(struct message),0);   //客户端把要求传给服务器       
	}
	return OK;
}
/***************************************************
函数名:admin_screen
功能:管理员禁言功能
传入参数:int sockfd,struct message *a
返回值:成功返回1，否则返回0
***************************************************/
int admin_screen(int sockfd,struct message *a)
{
    char str[MAXLEN];
    do
    {
        printf("你想禁言／解禁谁:\n");
      	setbuf(stdin,NULL);
	    scanf("%s",str);
    }while(strcmp(str,"admin") == 0);
	if(strlen(str) == 0) 
	{
		return FAULT;
	}
	else
	{
        strcpy((*a).flag,"admin_screen");        //禁言标志
        strcpy((*a).msg,str);
	    send(sockfd,a,sizeof(struct message),0);  
	}
	return OK;
}
/***************************************************
函数名:login_admin
功能:管理员登录
传入参数:struct message *a
返回值:成功返回1，否则返回0
***************************************************/
int login_admin(struct message *a)
{
	int do_number;						
	pthread_t pid;
	memset((*a).msg,0,strlen((*a).msg));                              //清空消息
	strcpy(chat_log,"admin");                                          //聊天记录名
	pthread_create(&pid,NULL,(void*)handlerecvmsg,(void *)&sockfd);     //创建线程    
	while(1)
	{
		usleep(5000);
		do
		{
			printf("1.踢人  2.禁言／解禁  3.查看在线用户  4.聊天  5.退出\n");
			printf("please input:\n");       
			setbuf(stdin,NULL);                                 //清空缓存
			scanf(" %d",&do_number);
			system("clear");
			setbuf(stdin,NULL);                                 //清空缓存
		}while((do_number != 1) && (do_number != 2) && (do_number != 3) && (do_number != 4)&& (do_number != 5));
	    switch(do_number)
		{
			case 1:
		    {
			    admin_kick(sockfd,a);               
			    break;
			}
			case 2:
			{
			    admin_screen(sockfd,a);       
		        break;
			}
			case 3:
		    {
			    strcpy((*a).flag,"view");
			    send(sockfd,a,sizeof(*a),0);
			    break;
		    }
			case 4:
			{
				admin_chat(sockfd,a);
		        break;
			}
    	    case 5:
		    {
		        close(sockfd);
			    return FAULT;
		    }
			default:
			{
	    		break;
			}
		}
	}
	return OK;
}
/***************************************************
函数名:handlerecvfile
功能:文件接收
传入参数:struct message *msg
返回值:无
***************************************************/
void handlerecvfile(struct message *msg)
{
    int n;
    struct message recvmsg = *msg;
    if(strcmp(recvmsg.msg,"end$") == 0)           //接收标志为end$
    {
        printf("文件传输结束!\n");
        close(savefilefd);
        savefilefd = -1;
        return ;
    }
    else
    {
        pthread_mutex_lock(&lock);           //上锁
        n=write(savefilefd,recvmsg.msg,recvmsg.size); //写入文件，保存聊天记录
        pthread_mutex_unlock(&lock);   //解琐
        if(n < recvmsg.size && n > 0)
        {
            printf("接收文件出错!\n");
	    return ;
        }
        }
    return ;
}

/***************************************************
函数名:handlesendfile
功能:文件传输
传入参数:无
返回值:无
***************************************************/
void handlesendfile()
{

    struct message filedata;
    do
    {
        memset(filedata.msg,0,sizeof(filedata.msg));            //清空消息缓存    
        filedata.size =read(savefilefd,filedata.msg,1000);      
        strcpy(filedata.flag,"transf");
        strcpy(filedata.addressee,filefromname);
        if (filedata.size == 0)
        {
            printf("文件传输完毕!\n");
            strcpy(filedata.msg,"end$");
            send(sockfd,&filedata,sizeof(struct message),0);     //向服务器发送结束标志
        }
        else if (filedata.size > 0)
        {
            send(sockfd,&filedata,sizeof(struct message),0);     //发送文件数据    
	        usleep(100000);              
        }
        else
        {
            printf("读取文件失败,文件传输中止\n");
            break;
        }
    }while(filedata.size > 0);
    close(savefilefd);
    savefilefd = -1 ;
}
/***************************************************
函数名:handlerecvmsg
功能:消息接收
传入参数:int *sockfd
返回值:无
***************************************************/
void handlerecvmsg(int *sockfd)
{
    int connfd = *sockfd;
    int nread;
    char buf[1024];
    char str[1024];
    struct message recvmsg;
    time_t timep;                              //显示当前时间
    if((fd=open(chat_log,O_RDWR|O_CREAT|O_APPEND,0777)) < 0)          //创建聊天记录文件    
    {
        printf("打开聊天记录失败!");
        exit(1);
    }
    while(1)
    {
        nread = recv(connfd,&recvmsg,sizeof(struct message),0);    //接受文件     
        if(nread == 0)
        {
            printf("You have detached from the server!\n");     
            close(fd);
            close(connfd);
            exit(0);
        }
        else if (strcmp(recvmsg.flag,"all") == 0)             //接受群发信息
        {
        time (&timep);
	    memset(str,0,strlen(str));
        sprintf(str,"%s%s 对大家说: %s\n",ctime(&timep),recvmsg.name,recvmsg.msg);
	    puts(str);
	    write(fd,str,strlen(str));                          //聊天信息写入文件
        }
        else if (strcmp(recvmsg.flag,"personal") == 0)         //接受私信 
        {
            time (&timep);
	        memset(str,0,strlen(str));
            sprintf(str,"%s%s 对你说: %s\n",ctime(&timep),recvmsg.name,recvmsg.msg);
	        printf("%s\n",str);
	        write(fd,str,strlen(str));                         //保存聊天记录
        }
	else if (strcmp(recvmsg.flag,"sermsg") == 0)          //系统提示信息
        {
            time (&timep);
	        memset(str,0,strlen(str));
            sprintf(str,"%s系统信息: %s\n",ctime(&timep),recvmsg.msg);
	        printf("%s\n",str);
	        write(fd,str,strlen(str));
	    continue;
        }
        else if (strcmp(recvmsg.flag,"view") == 0)       //收到查看在线用户标志
        {
            time (&timep);
	        memset(str,0,strlen(str));
            sprintf(str,"%s在线用户:\n%s\n",ctime(&timep),recvmsg.msg);
	        printf("%s\n",str);
            continue;
        }
        else if (strcmp(recvmsg.flag,"trans") == 0)          //传输文件标志
        {
            pthread_t pid;
            if (strcmp(recvmsg.msg,"agree") == 0)          //同意接受文件标志
            {
                strcpy(filefromname,recvmsg.name);
		printf("文件传送中……\n");
                //创建线程发送文件
                pthread_create(&pid,NULL,(void *)handlesendfile,NULL);     //创建发送文件线程
            }
            else if(strcmp(recvmsg.msg,"disagree") == 0)
            {
                printf("对方拒绝接收文件\n");
                close(savefilefd);
                savefilefd = -1;
            }
            else if(strcmp(recvmsg.msg,"noexist") == 0)
            {
                printf("对不起，该客户端不存在\n");
                close(savefilefd);
                savefilefd = -1;
            }
            else
            {
                strcpy(filefromuser,recvmsg.name);
                strcpy(filefromname,recvmsg.msg);
                printf("%s向你传文件%s,是否同意接受?\n同意请输入：trans$agree\t不同意请输入：trans$disagree\n",recvmsg.name,recvmsg.msg);
                savefilefd = 0;
            }
            continue;
        }
        else if(strcmp(recvmsg.flag,"transf") == 0)               
        {
            pthread_t pid_transf;
	        pthread_create(&pid_transf,NULL,(void *)handlerecvfile,&recvmsg); //创建文件传输线程
	        pthread_join(pid_transf,NULL);			
            continue;
        }
        else
        {
            if(strcmp(recvmsg.flag,"") == 0)
            {
                continue;
            }
        }
    }
}
/***************************************************
函数名:cutStr
功能:将字符串str在字符c处分割，前后两段
分别赋给left和right，left最大长度为n，str最大
长度为max
传入参数:char str[],char left[], int n, char right[],int max, char c
返回值:无
***************************************************/
void cutStr(char str[],char left[], int n, char right[],int max, char c)
{
    int i,k,j;
    for(i = 0 ; i < n ;i++)
    {
        if(str[i] == c)                             //c为str的分割符
        break;
    }
    if(i == n)
    {
        i = -1;
    }
    else
    {
        memset(left,0,strlen(left));
        for(k = 0 ; k < i ; k++)
        {
            left[k] = str[k];         //c左边的字符串赋给left[] 
        }
    }
    for(j = i+1 ; j < max;j++)
    {
        if(str[j] == '\0')
        break;
        right[j-i-1] = str[j];     //c右边的字符串给right[]
    }
    left[i] = '\0';
    if(j < max)
    {
		right[j-i-1] = '\0';
    }	
    else
    {
		right[max] = '\0';
    }
}

/***************************************************
函数名:common_use_words
功能:常用语替换
传入参数:char msg[]
返回值:无
***************************************************/
void common_use_words(char msg[])
{
    int i=0,j=0;
	char common[MAXLEN];
    char tmp[MAXLEN];
	if(msg[0] == '/')
	{
		strcpy(tmp,msg);
		memset(msg,0,strlen(msg));
		for(i=1;i<MAXLEN;i++)
		{
			if(tmp[i] != '/')
			{
			    msg[i-1] = tmp[i];
			}
			else
			{
			    break;
			}
		}
		msg[i-1] = ',';
		msg[i] = '\0';
		i++;
		for(j=0;i<MAXLEN;i++,j++)
		{
		    if(tmp[i] != '\0')
		    {
		        common[j] = tmp[i];
		    }
		    else
		    {
			break;
		    }
		}
		common[j] = '\0';
		if(strcmp(common,"welcome") == 0)
		{
		    strcat(msg,"欢迎来到我们的聊天室^_^");         //连接字符串
			return ;
		}
		strcat(msg,common);
		
	}
}
/***************************************************
函数名:expression
功能:常用表情替换
传入参数:char name[],char msg[],int m
返回值:无
***************************************************/
void expression(char name[],char msg[])
{
	if(strcmp(msg,":)") == 0)
	{
        sprintf(msg,"%s :-D",name);
	}
    if(strcmp(msg,":(") == 0)
	{
	    sprintf(msg,"%s >_<",name);
	}
	if(strcmp(msg,"囧") == 0)
	{
	    sprintf(msg,"%s o(╯□╰)o",name);
	}
	if(strcmp(msg,"哈哈") == 0)
	{
	    sprintf(msg,"O(∩_∩)O哈哈~");
	}
	if(strcmp(msg,"汗") == 0)
	{
	    sprintf(msg,"~_~|||");
	}
	if(strcmp(msg,"晕") == 0)
	{
	    sprintf(msg,"＠_＠");
	}
}

/***************************************************
函数名:Interface
功能:登录界面
传入参数:无
返回值:无
***************************************************/
int Interface()
{
    int do_number;
	do
	{
		system("clear");
		printf("----------------------------------\n");
		printf("        欢迎进入聊天室	  \n");
		printf("	   1.登录                 \n");
		printf("	   2.注册			      \n");
		printf("	   3.退出			      \n");
		printf("----------------------------------\n");
		printf("请选择:\n");
		setbuf(stdin,NULL);
		scanf(" %d",&do_number);
		setbuf(stdin,NULL);
	}while((do_number != 1) && (do_number != 2) && (do_number != 3));
	return do_number;
}
/***************************************************
函数名:help
功能:帮助
传入参数:char str[]
返回值:成功返回1，否则返回0
***************************************************/
int help(char str[])
{
    if(strcmp(str,".help") == 0)
    {
        printf("尊敬的%s用户，你好：\n",locname);
        printf("all$hello!-------------对所有人说hello!\n");
        printf("bill$hello!------------对bill说hello!\n");
        printf("trans$bill$hello.txt---传文件hello.txt给bill\n");
        printf("trans$agree------------同意接收文件\n");
        printf("trans$disagree---------不同意接收文件\n");
        printf("view$------------------查看在线用户\n");
        printf("exit$------------------退出\n");                 
		return OK;
    }
	else
	{
	    return FAULT;
	}
}
/***************************************************
函数名:log_user
功能:用户登录
传入参数:struct message *a
返回值:无
***************************************************/
void log_user(struct message *a)
{
    do
    {
		printf("请输入用户名（10字以内）:\n");
		memset((*a).name,0,strlen((*a).name));
		scanf("%s",(*a).name);
		strcpy(locname,(*a).name);
		printf("请输入密码（20位以内）:\n");
		memset((*a).msg,0,strlen((*a).msg));
		scanf("%s",(*a).msg);
    }while(strlen((*a).name)>20 || strlen((*a).msg)>20);
    strcpy((*a).flag,"login");
    send(sockfd,a,sizeof(*a),0);                 //向服务器发送登录信息
    printf("正在登录，请稍等……\n");
    recv(sockfd,a,sizeof(*a),0);
    printf("recv the message from server:%s\n",(*a).msg);
}
/***************************************************
函数名:login_success
功能:登录成功进入聊天模式
传入参数:struct message *a
返回值:退出返回0，否则返回1
***************************************************/
int login_success(struct message *a)
{
    char str[MAXLEN];
    char buf[MAXLEN];
    time_t timep;
    pthread_t pid;
    sprintf(chat_log,"%s.txt",(*a).name);
    if((fd=open(chat_log,O_RDWR|O_CREAT|O_APPEND,0777)) < 0)
    {
        printf("打开聊天记录失败!");
	exit(1);
    }
    pthread_create(&pid,NULL,(void*)handlerecvmsg,(void *)&sockfd);         //创建接受消息线程
    setbuf(stdin,NULL);
    strcpy((*a).flag,"all");
    printf("尊敬的%s你好，如需帮助请输入：.help\n",locname);
	while(1)
	{
		memset((*a).msg,0,strlen((*a).msg));
		memset(str,0,strlen(str));
		usleep(100000);
		printf("TO %s:\n",(*a).flag);
		setbuf(stdin,NULL);
		scanf("%s",str);
		if(OK == help(str))                      //提示信息  
		{
		    continue;
		}
		strcpy((*a).name,locname);
		strcpy(buf,(*a).flag);
		cutStr(str,(*a).flag,15,(*a).msg,MAXLEN,'$');    //调用字符切割函数  
		expression((*a).name,(*a).msg);               //表情替换函数 
		common_use_words((*a).msg);                    //常用语使用函数
		if(strcmp((*a).flag,"exit") == 0)
		{
		    return FAULT;
		}
		if(strcmp((*a).flag,"view") == 0)           
		{
		    send(sockfd,a,sizeof((*a)),0);       //请求查看在线用户
	     	strcpy((*a).flag,buf);
		    continue;
		}
		if(strcmp((*a).flag,"all") == 0)
		{
		    send(sockfd,a,sizeof(*a),0);        
		    continue;
		}
	        if ((strcmp((*a).flag,"trans") == 0) && (savefilefd <=0))
		{
    		    if ((strcmp((*a).msg,"agree") == 0) && (savefilefd == 0))
		    {
		        char savefilename[MAXLEN];
			    strcpy((*a).addressee,filefromuser);
			    printf("请输入你想保存的文件名：\n");
			do
			{
			    setbuf(stdin,NULL);
                scanf("%s",str);
			    savefilefd = open(savefilename,O_RDWR|O_CREAT|O_EXCL,0777);
			    if(savefilefd == -1)
			    {
			        printf("文件已存在，你重新输入：\n");
			    }
			}while(savefilefd == -1);
			if(savefilefd < 0)
			{
			    printf("接收文件失败!\n");
			    savefilefd = -1;
			}
			else
			{
			    strcpy((*a).msg,"agree");
			    send(sockfd,a,sizeof(*a),0);
			    printf("文件接收中……\n");
			}
		}
			else
			{
				memset(str,0,strlen(str));
				cutStr((*a).msg,(*a).addressee,20,str,MAXLEN,'$');
				if (str[0] != '\0' && (*a).addressee[0] != '\0')
				{
					char transfileallname[22];
					sprintf(transfileallname,"./%s",str);
					savefilefd = open(str,O_RDWR,0666);
					if(savefilefd < 0)
					{
						printf("打开文件失败!\n");
						savefilefd = -1;
					}
        			else
					{
						memset((*a).msg,0,strlen((*a).msg));
						strcpy((*a).msg,str);
						send(sockfd,a,sizeof(*a),0);
					}
				}
				else
				{
					strcpy((*a).msg,"disagree");
					strcpy((*a).name,locname);
					strcpy((*a).addressee,filefromuser);
					send(sockfd,a,sizeof(*a),0);
				}
			}
			strcpy((*a).flag,buf);
			continue;
		}
		if (strcmp((*a).flag,"trans") == 0)
		{
			strcpy((*a).flag,buf);
		}
		else
		{
			strcpy(buf,(*a).flag);
			strcpy((*a).addressee,(*a).flag);
			strcpy((*a).flag,"personal");
			send(sockfd,a,sizeof(*a),0);             //发送私信
			strcpy((*a).flag,buf);
			time (&timep);
			memset(str,0,strlen(str));
			sprintf(str,"%s你对 %s 说: %s\n",ctime(&timep),(*a).flag,(*a).msg);
			printf("%s",str);
			write(fd,str,strlen(str));              //写入聊天记录文件中
		}
	}
	return OK;
}
/***************************************************
函数名:Register
功能:注册
传入参数:struct message *a
返回值:返回0
***************************************************/
int Register(struct message *a)
{
    char password_t[MAXLEN];
	do
	{
		printf("请输入用户名（10字以内）:\n");
		memset((*a).name,0,strlen((*a).name));
		setbuf(stdin,NULL);
		scanf("%s",(*a).name);
	}while(strlen((*a).name)>20);
    while(1)
    {
        printf("请输入密码（20位以内）:\n");
	    memset((*a).msg,0,strlen((*a).msg));
		setbuf(stdin,NULL);
        scanf("%s",(*a).msg);
        printf("请再次输入密码（20位以内）:\n");
		memset(password_t,0,strlen(password_t));
		setbuf(stdin,NULL);
        scanf("%s",password_t);
        if(strcmp((*a).msg,password_t) != 0 || strlen((*a).msg)>20 || strlen(password_t)>20)
        {
            printf("密码出错！\n");
        }
        else
        {
            break;
        }
    }
    strcpy((*a).flag,"reg");
    send(sockfd,a,sizeof(*a),0);
    printf("正在注册，请稍等……\n");
    recv(sockfd,a,sizeof(*a),0);
    printf("recv the message from server:%s\n",(*a).msg);
    sleep(3);
}
