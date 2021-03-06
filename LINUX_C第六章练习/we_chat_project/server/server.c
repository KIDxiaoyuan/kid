/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: Mon 15 Aug 2016 10:19:48 GMT
 ************************************************************************/
#include "server.h"
////////////////////////////函//数//声//明//区///////////////////////////////
int reg_check(struct message *recievemsg);
int login_check(struct message *recievemsg);
extern LinkList CreateLinkList(void);
extern void deletelist(LinkList L ,datatype e);
extern void insertend(LinkList l,datatype e);
extern void DisplayList(LinkList L);
extern LinkList findlist(LinkList l,datatype e);
int reg_check(struct message *recievemsg);
int login_check(struct message * recievemsg);
int server_admin_kick(struct message *a, clientinf *clientNode);
int server_admin_screen(struct message *a, clientinf *clientNode);
int server_all(struct message *a, clientinf *clientNode);
int server_personal(struct message *a, clientinf *clientNode);
int overlap(struct message a);
int server_login(struct message *a, clientinf *clientNode);
int server_exit(struct message *a, clientinf *clientNode);
int server_reg(struct message *a, clientinf *clientNode);
void server_trans(struct message *a,clientinf *clientNode);
void server_transf(struct message *a);
int server_view(struct message *a, clientinf *clientNode);
User_list * get_friend_list(char * name);
void friend_view(struct message *a ,clientinf *clientNode );
int add_friend( struct message *a,clientinf *clientNode);
void recv_friend(struct message *a,clientinf *clientNode);
////独是一个人的必修课，好好享受这份孤独，才发现音乐如此美好。//////
int main(void)
{
    int re;
	int fd;
    struct message a;
	datatype e;
    LinkList transfileNode;
    char buf[MAXLEN],str[MAXLEN];
    time_t timep;
    clientinf clientNode;
    clientNode.speak = 1;
	strcpy(clientNode.name,"***");
	clientlink=CreateLinkList();
	if((fd=open("chat_log_server.txt",O_RDWR|O_CREAT|O_APPEND,0777)) < 0)
	{
		printf("failure to open chat_log_server!");
		exit(1);
	}
	int lfd;
	int cfd;
	int sfd;
	int rdy;
	struct sockaddr_in sin;
	struct sockaddr_in cin;
	int client[FD_SETSIZE];  /* 客户端连接的套接字描述符数组 */
	int maxi;
	int maxfd;                        /* 最大连接数 */
	fd_set rset;
	fd_set allset;
	socklen_t addr_len;         /* 地址结构长度 */
	char buffer[MAX_LINE];
	int i;
	int n;
	int len;
	int opt = 1;   /* 套接字选项 */
	char addr_p[20];
	/* 对server_addr_in  结构进行赋值  */
	bzero(&sin,sizeof(struct sockaddr_in));   /* 先清零 */
	sin.sin_family=AF_INET;
	sin.sin_addr.s_addr=htonl(INADDR_ANY);  //表示接受任何ip地址   将ip地址转换成网络字节序
	sin.sin_port=htons(portnumber);         //将端口号转换成网络字节序
	/* 调用socket函数创建一个TCP协议套接口 */
	if((lfd=socket(AF_INET,SOCK_STREAM,0))==-1) // AF_INET:IPV4;SOCK_STREAM:TCP
	{
		fprintf(stderr,"Socket error:%s\n\a",strerror(errno));
		exit(1);
	}
	/*设置套接字选项 使用默认选项*/
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	/* 调用bind函数 将serer_addr结构绑定到sockfd上  */
	if(bind(lfd,(struct sockaddr *)(&sin),sizeof(struct sockaddr))==-1)
	{
		fprintf(stderr,"Bind error:%s\n\a",strerror(errno));
		exit(1);
	}
	/* 开始监听端口   等待客户的请求 */
	if(listen(lfd,20)==-1)
	{
		fprintf(stderr,"Listen error:%s\n\a",strerror(errno));
		exit(1);
	}
	printf("正在连接.......\n");

	maxfd = lfd;      /*对最大文件描述符进行初始化*/
	maxi = -1;
	/*初始化客户端连接描述符集合*/
	for(i = 0;i < FD_SETSIZE;i++)
	{
        client[i] = -1;
	}
	FD_ZERO(&allset);                     /* 清空文件描述符集合 */
	FD_SET(lfd,&allset);                 /* 将监听字设置在集合内 */
	/* 开始服务程序的死循环 */
	while(1)
	{
		rset = allset;
		/*得到当前可以读的文件描述符数*/
		rdy = select(maxfd + 1, &rset, NULL, NULL, NULL);
		if(FD_ISSET(lfd, &rset))
		{
			addr_len = sizeof(sin);

			/* 接受客户端的请求 */
			if((cfd=accept(lfd,(struct sockaddr *)(&cin),&addr_len))==-1)
			{
				fprintf(stderr,"Accept error:%s\n\a",strerror(errno));
				exit(1);
			}
            /*查找一个空闲位置*/
			for(i = 0; i<FD_SETSIZE; i++)
			{
				if(client[i] <= 0)
				{
					client[i] = cfd;   /* 将处理该客户端的连接套接字设置到该位置 */
					break;
				}
			}
		    /* 太多的客户端连接   服务器拒绝请求  跳出循环 */
			if(i == FD_SETSIZE)
			{
				printf("too many clients");
				exit(1);
			}
			FD_SET(cfd, &allset);     /* 设置连接集合 */
			if(cfd > maxfd)                  /* 新的连接描述符 */
			{
				maxfd = cfd;
			}
			if(i > maxi)
			{
				maxi = i;
			}
			if(--rdy <= 0)                /* 减少一个连接描述符 */
			{
				continue;
			}
		}
		/* 对每一个连接描述符做处理 */
		for(i = 0;i< FD_SETSIZE;i++)
		{
			if((sfd = client[i]) < 0)
			{
				continue;
			}

			if(FD_ISSET(sfd, &rset))
			{
				/*如果没有可以读的套接字   退出循环*/
				if(--rdy < 0)
				{
					break;
				}
				re = recv(sfd,&a,sizeof(a)+1,0);
				clientNode.decr = sfd;
				strcpy(clientNode.name,a.name);
				if(re == 0)                                /*客户端断开连接*/
				{
		            server_exit(&a, &clientNode) ;
					fflush(stdout);                                    /* 刷新 输出终端 */
					close(sfd);
					FD_CLR(sfd, &allset);                        /*清空连接描述符数组*/
					client[i] = -1;
				}
				else
				{
				    if(strcmp(a.flag,"login") == 0)      /*客户端请求登录*/
                    {
                        if(overlap(a) == 0)               /*若该用户名不在线*/
                        {
        			        server_login(&a,&clientNode);    /*处理登录请求*/
                        }
				        else                                 /*若该用户名已在线*/
					    {
				            strcpy(a.msg,"overlap");
					  	    send(clientNode.decr,&a,sizeof(struct message),0);/*返回重名提示*/
						}
					    continue;
                    }
                    else if(strcmp(a.flag,"reg") == 0)   /*客户端请求注册*/
                    {
		        	    server_reg(&a,&clientNode);       /*处理注册请求*/
                        continue;
                    }
                    else if (strcmp(a.flag,"all") == 0)    /*客户端请求广播*/
                    {
                        if(server_all(&a,&clientNode) == 1)   /*处理广播请求*/
                        {
							memset(str,0,strlen(str));
							time (&timep);
							sprintf(str,"%s%s TO %s: %s\n",ctime(&timep),a.name,a.flag,a.msg);
							printf("%s",str);
							write(fd,str,strlen(str));      /*将广播写入聊天记录*/
                        }
						continue;
                    }
                    else if(strcmp(a.flag,"personal") == 0)  /*客户端请求私聊*/
		    {
			if(server_personal(&a,&clientNode) == 1) /*处理私聊请求*/
			    {
				memset(str,0,strlen(str));
				time (&timep);
				sprintf(str,"%s%s TO %s: %s\n",ctime(&timep),a.name,a.addressee,a.msg);
				printf("%s",str);
				write(fd,str,strlen(str));       /*将私聊写入聊天记录*/
			    }
		    }
		    else if(strcmp(a.flag,"view") == 0)    /*客户端请求查看在线用户*/
                    {
                        server_view(&a,&clientNode);        /*处理查看在线用户请求*/
                        continue;
                    }
		            else if(strcmp(a.flag,"admin_kick") == 0) /*管理员替人请求*/
		            {
		                int closefd;
		                closefd = server_admin_kick(&a,&clientNode);  /*处理管理员替人请求*/
						FD_CLR(closefd, &allset);                      /*清空连接描述符数组*/
						int j;
						for(j = 0;j< FD_SETSIZE;j++)
						{
							if(closefd == client[j])
							{
							    client[j]=-1;
								break;
							}
						}
						continue;
		            }
		            else if(strcmp(a.flag,"admin_screen") == 0)/*管理员禁言请求*/
		            {
		                server_admin_screen(&a,&clientNode);/*处理管理员禁言请求*/
						continue;
		            }
 		            else if(strcmp(a.flag,"trans") == 0) /*客户端文件传输请求*/
                    {

				        server_trans(&a,&clientNode);    /*处理客户端文件传输请求*/
                        continue;
                    }
		            else if(strcmp(a.flag,"transf") == 0) /*客户端文件数据传输请求*/
		            {
					    server_transf(&a);        /*传送文件*/
			            continue;
		            }
                    else if (strcmp(a.flag,"friend_wanted") == 0)//接受好友邀请添
                    {
                        printf("收到好友请求确认\n");
                        recv_friend(&a,&clientNode);
                        continue;
                    }
                    else if (strcmp(a.flag,"view_friend") == 0)
                    {//查看好友在线情况
                        printf("view!\n");
                        friend_view(&a,&clientNode);
                        continue;
                    }
                    else if (strcmp(a.flag,"add_friend") == 0)
                    {//处理添加好友邀请
                        printf("收到添加好友邀请\n");
                        add_friend(&a,&clientNode);
                        continue;
                    }

                }
			}
		}
    }
	close(lfd);
	return 0;
}

/***************************************
 * 说明：reg_check
 * 功能：注册检查
 * 传入参数：struct message *recievemsg
 * 返回值：成功:REG_OK,失败:REG_FAULT
 ***************************************/
int reg_check(struct message *recievemsg)
{
    int fd;
    int read_size,write_size;
    struct message cmpmsg;
    char *p_name;
    char *p_passwore;
    int rc,i,ncols;
    sqlite3 *db;
	    sqlite3_stmt *stmt;
    char sql[128];
    const char *tail;
    if (strlen(recievemsg->name)>20 || strlen(recievemsg->msg) > 20)
    {
        puts("message is too long");
        return REG_FAULT;
    }
    //容错处理，字符串检查
    if(strcmp(recievemsg->name,"admin") == 0)
    {
        return REG_FAULT;
    }
    if(strcmp(recievemsg->name,"all") == 0)
    {
        return REG_FAULT;
    }
    if(strcmp(recievemsg->name,"reg") == 0)
    {
        return REG_FAULT;
    }
    if(strcmp(recievemsg->name,"login") == 0)
    {
        return REG_FAULT;
    }
    if(strcmp(recievemsg->name,"trans") == 0)
    {
        return REG_FAULT;
    }
    //打开数据库
    rc = sqlite3_open("chat.db",&db);
    if(rc)
    {
        fprintf(stderr,"can't open database:%s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return REG_FAULT;
    }
    //重名检查
    memset(sql,0,sizeof(sql));
    sprintf(sql,"select * from member where name = '%s';",(recievemsg->name));
    rc = sqlite3_prepare(db,sql,strlen(sql),&stmt,&tail);
    if(rc != SQLITE_OK)
    {
        fprintf(stderr,"reg_check:SQLerror :%s\n",sqlite3_errmsg(db));
        return 0;
    }
    //执行
    rc = sqlite3_step(stmt);
    ncols = sqlite3_column_count(stmt);
    while(rc == SQLITE_ROW)
    {
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return REG_FAULT;
    }
    //销毁stmt
    sqlite3_finalize(stmt);
    //注册
    memset(sql,0,sizeof(sql));
    sprintf(sql,"insert into member (name,password) values(?,?);");
    rc = sqlite3_prepare(db,sql,strlen(sql),&stmt,&tail);
    p_name = recievemsg->name;
    sqlite3_bind_text(stmt,1,p_name,strlen(p_name),NULL);
    p_passwore = recievemsg->msg;
    sqlite3_bind_text(stmt,2,p_passwore,strlen(p_passwore),NULL);
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return REG_OK;
}

/************************************************************************
 * 函数名：login_check
 * 功能：注册检查
 * 传入参数：struct message *recievemsg
 * 返回值：管理员成功返回LOG_ADMIN 用户成功返回LOG_USER 失败返回LOG_FAULT
 ***********************************************************************/
 int login_check(struct message *recievemsg)
{
    int fd;
    int read_size,write_size;
    struct message cmpmsg;
	char *p_name;
	char *p_passwore;
	int rc,i,ncols;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char sql[128] ;
	const char *tail;
	
	/*打开数据库*/
	rc = sqlite3_open("chat.db",&db);     
    if(rc)
    { 
         fprintf(stderr,"can't open databse:%s",sqlite3_errmsg(db));
         sqlite3_close(db);
         return LOG_FAULT;
    }      
	/*检查用户名密码*/
	memset(sql,0,sizeof(sql));
	sprintf(sql,"select * from member where name='%s' and password='%s';",(recievemsg->name),(recievemsg->msg));
	rc = sqlite3_prepare(db,sql,strlen(sql),&stmt,&tail);
	if(rc != SQLITE_OK)
	{
            fprintf(stderr,"364:SQLerror:%s",sqlite3_errmsg(db));
	}
	  /*执行*/
	 rc = sqlite3_step(stmt);
	 ncols = sqlite3_column_count(stmt);
	 while (rc == SQLITE_ROW)
	 {
			 rc = sqlite3_step(stmt);
			 sqlite3_finalize(stmt);
			 sqlite3_close(db);
			 if(strcmp(recievemsg->name,"admin") == 0)
			 {
			     return LOG_ADMIN;
			 }
			 else
			 {
				 return LOG_USER;
			 }

	 }
	  /*销毁stmt*/
	sqlite3_finalize(stmt);
	  /*关闭数据库*/
	sqlite3_close(db);
	return LOG_FAULT;
}

/***************************************************
函数名:server_admin_kick
功能:处理管理员踢人功能
传入参数:struct message *a, clientinf *clientNode
返回值:成功返回踢出客户端的fd，否则返回0
***************************************************/
int server_admin_kick(struct message *a, clientinf *clientNode)
{
    int closefd;
	datatype e;
	strcpy(e.name,(*a).msg);
	LinkList L;
	L = (LinkList)findlist(clientlink,e);
	if(L == NULL)
	{
		strcpy((*a).flag,"sermsg");
		sprintf((*a).msg,"该用户不在线\n");
		send((*clientNode).decr,a,sizeof(struct message),0);
		return 0;
	}
	else
	{
	    closefd=L->data.decr;
		close(closefd);
		deletelist(clientlink,e);
		L = clientlink;
		L = L->next;
		strcpy((*a).flag,"sermsg");
		sprintf((*a).msg,"%s 被管理员踢出了聊天室!",(*a).msg);
		while(L != NULL)
		{
			send(L->data.decr,a,sizeof(struct message),0);
			L = L->next;
		}
	}   
	return closefd;
}

/***************************************************
函数名:server_admin_screen
功能:处理管理员禁言功能
传入参数:struct message *a, clientinf *clientNode
返回值:成功返回1，否则返回0
***************************************************/
int server_admin_screen(struct message *a, clientinf *clientNode)
{
	datatype e;
	strcpy(e.name,(*a).msg);
	LinkList L;
	L = (LinkList)findlist(clientlink,e);
	if(L == NULL)
	{
		 strcpy((*a).flag,"sermsg");
         sprintf((*a).msg,"该用户不在线\n");
		 send((*clientNode).decr,a,sizeof(struct message),0);
		 return 0;
	}
	else
	{
        L->data.speak = (L->data.speak+1)%2;
	    strcpy((*a).flag,"sermsg");
		if(L->data.speak == 0)
		{
		    sprintf((*a).msg,"%s 被管理员禁言了!",(*a).msg);
		}
	    else
		{
			sprintf((*a).msg,"%s 被管理员解禁!",(*a).msg);
		}
		L = clientlink;
	    L = L->next;
		while(L != NULL)
	    {
	        send(L->data.decr,a,sizeof(struct message),0);
			L = L->next;
		}
	}
	return 1;
}
/***************************************************
函数名:server_all
功能:处理客户端群发消息
传入参数:struct message *a, clientinf *clientNode
返回值:禁言返回0，否则返回1
***************************************************/
int server_all(struct message *a, clientinf *clientNode)
{

	datatype e;
    strcpy(e.name,(*a).name);
	LinkList L;
	L = (LinkList)findlist(clientlink,e);
	if(L != NULL)
	{
	    if(L->data.speak == 0)
	    {
			strcpy((*a).flag,"sermsg");
			sprintf((*a).msg,"对不起，你已经被管理员禁言了!");
			send(L->data.decr,a,sizeof(struct message),0);
			return 0;
		}
	}
    if (strlen((*a).msg) > 0)
    {
        L = clientlink;
        L = L->next;
        strcpy((*a).name,(*clientNode).name);
        while(L != NULL)
        {
            send(L->data.decr,a,sizeof(struct message),0);
            L = L->next;
        }
    }
	return 1;
}
/***************************************************
函数名:server_personal
功能:处理客户端私聊
传入参数:struct message *a, clientinf *clientNode
返回值:成功返回1，否则返回0
***************************************************/
int server_personal(struct message *a, clientinf *clientNode)
{
	datatype e;
	strcpy(e.name,(*a).name);
	LinkList L;
	L = clientlink;
	L=L->next;
	strcpy(e.name,(*a).addressee);
	L = (LinkList)findlist(clientlink,e);
	if(L == NULL)
	{
		strcpy((*a).flag,"sermsg");
		sprintf((*a).msg,"该用户不在线\n");
		send((*clientNode).decr,a,sizeof(struct message),0);
		return 0;
	}
	else
	{
		if (strcmp((*a).msg,"") != 0)
		{
			send(L->data.decr,a,sizeof(struct message),0);
		}
	}
	return 1;
}
/***************************************************
函数名:overlap
功能:重名用户检查
传入参数:struct message a
返回值:重名返回1，否则返回0
***************************************************/
int overlap(struct message a)
{
    LinkList L;
    //puts("display!");
    //DisplayList(L);
	L = clientlink->next;
	while(L != NULL)
	{
		if(strcmp(L->data.name,a.name) == 0)
		{
		    return 1;
		}
		else
		{
			L = L->next;
		}
	}
	return 0;
}

/***************************************************
函数名:server_login
功能:处理客户端登录
传入参数:struct message *a, clientinf *clientNode
返回值:无
***************************************************/
int server_login(struct message *a, clientinf *clientNode)
{
	int i,server_flag;
	char buf[MAXLEN];
	i = login_check(a);

	if(i == 1)
	{
		printf("管理员admin登录成功!\n");
		strcpy((*a).msg,"hello,admin!");
		strcpy((*clientNode).name,(*a).name);
		insertend(clientlink,*clientNode);
		send((*clientNode).decr,a,sizeof(struct message),0);
	}
	else
	{
		if(i == 0)
		{
            User_list* Friend_list;
			printf("%s 登录成功!\n",(*a).name);
			memset((*a).msg,0,strlen((*a).msg));
			strcpy((*a).msg,"login,success!");
			strcpy((*clientNode).name,(*a).name);
            Friend_list = get_friend_list((*a).name);
            clientNode->friend_list = Friend_list;
			insertend(clientlink,*clientNode);
			send((*clientNode).decr,a,sizeof(struct message),0);
			
			LinkList L;
			L = clientlink;
			L=L->next;
			strcpy((*a).flag,"sermsg");
			sprintf((*a).msg,"欢迎 %s 进入聊天室!",(*a).name);
            

			while(L != NULL)
			{
				send(L->data.decr,a,sizeof(struct message),0);
				L = L->next;
			}
	    }
		else
		{
			printf("%s 登录失败!\n",(*a).name);
			strcpy((*a).msg,"login,failure!");
			send((*clientNode).decr,a,sizeof(struct message),0);
		}
	}

}

/***************************************************
函数名:server_exit
功能:处理客户端与服务器断开连接
传入参数:struct message *a, clientinf *clientNode
返回值:无
***************************************************/
int server_exit(struct message *a, clientinf *clientNode)   
{
	LinkList L;
	L = clientlink->next;
	while(L != NULL)
	{
		if(L->data.decr == (*clientNode).decr)
		{						
			strcpy((*clientNode).name,L->data.name);
			break;
		}
		else
		{
			L = L->next;
		}
	}
	strcpy((*a).flag,"sermsg");
	printf("%s 离开聊天室\n",(*clientNode).name);
	deletelist(clientlink ,(*clientNode));
	L = clientlink;
	L=L->next;
	while(L != NULL)
	{
		send(L->data.decr,a,sizeof(struct message),0);
		L = L->next;
	}
}
/***************************************************
函数名:server_reg
功能:客户端注册处理
传入参数:struct message *a, clientinf *clientNode
返回值:无
***************************************************/
int server_reg(struct message *a, clientinf *clientNode)
{
    int i;
	char buf[MAXLEN];
    i = reg_check(a);
    if(i == 1)
    {
	    printf("%s 注册成功!\n",(*a).name);
   	    strcpy((*a).msg,"register,success!");
	    strcpy((*clientNode).name,(*a).name);
	    send((*clientNode).decr,a,sizeof(struct message),0);
    }
    else
    {
	    printf("%s 注册失败!\n",(*a).name);
	    strcpy((*a).msg,"register,failure!");
	    send((*clientNode).decr,a,sizeof(struct message),0);
    }
}
/***************************************************
函数名:server_trans
功能:处理传输文件请求
传入参数:struct message *a,clientinf *clientNode
返回值:无
***************************************************/
void server_trans(struct message *a,clientinf *clientNode)
{
    LinkList L;
    L = clientlink;
    L=L->next;
    while(L != NULL)
	{
		if (strcmp(L->data.name,(*a).addressee)== 0)
		{
			break;
		}
		L = L->next;
			}
	if(L == NULL)
	{
		strcpy((*a).msg,"noexist");
		send((*clientNode).decr,a,sizeof(struct message),0);
		return ;
	}

    L = clientlink;
    L=L->next;
    while(L != NULL)
	{
		if (strcmp(L->data.name,(*a).name)== 0)
		{
			break;
		}
		L = L->next;
	}
	if(L == NULL)
	{
		strcpy((*a).msg,"noexist");
		send((*clientNode).decr,a,sizeof(struct message),0);
	}
	else
	{
		L = clientlink->next;
		while(L != NULL)
		{
			if(strcmp(L->data.name,(*a).addressee) == 0)
			{					 
				break;
			}
			else
			{
				L = L->next;
			}
		}
		send(L->data.decr,a,sizeof(struct message),0);
	}
}

/***************************************************
函数名:server_transf
功能:传输文件数据
传入参数:struct message *a
返回值:无
***************************************************/
void server_transf(struct message *a)
{
	LinkList L;
    L = clientlink->next;
    while(L != NULL)
    {
		if(strcmp(L->data.name,(*a).addressee) == 0)
		{						
		    break;
        }
 	    else
		{
			L = L->next;
		}
	}
	send(L->data.decr,a,sizeof(struct message),0);
}
/***************************************************
函数名:server_view
功能:处理客户端查看当前在线用户
传入参数:struct message *a, clientinf *clientNode
返回值:无
***************************************************/
int server_view(struct message *a, clientinf *clientNode)
{
	char str[MAXLEN],buf[MAXLEN];
    LinkList L;
    int i = 1;
    L = clientlink;
    L=L->next;
    memset(buf,0,strlen(buf));
    while(L != NULL)
    {
        memset(str,0,strlen(str));
		if(L->data.speak==1)
		{
			sprintf(str,"%d. %s （在线）\n",i,L->data.name);
			strcat(buf,str);
		}
		else
		{
			sprintf(str,"%d. %s （禁言）\n",i,L->data.name);
			strcat(buf,str);
		}
        L = L->next;
        i++;
    }
    strcpy((*a).name,(*clientNode).name);
    strcpy((*a).msg,buf);
    send((*clientNode).decr,a,sizeof(struct message),0);
}
/***************************************
函数名：friend_view
参数 :messag *a ,clientNode clientlink
功能：查询好友在线情况
***************************************/
void friend_view(struct message *a ,clientinf *clientNode )
{
    User_list* friends ;
    LinkList L=clientlink,now;
    datatype e;
    strcpy(e.name,a->name);
    now = (LinkList)findlist(clientlink,e);
    if(now == NULL)
    {
        printf("friend_view:Not find %s\n",a->name);
    }
    friends = now->data.friend_list->next;
    char str[MAXLEN],buf[MAXLEN];
    memset(buf,0,strlen(buf));
    int i=1;
    while (friends!= NULL)
    {
        int f =0;

        for(L=clientlink;L!=NULL;L=L->next)
        {
            if(strcmp(L->data.name,friends->name) == 0)
            {
                if(L->data.speak == 0)
                {
                    sprintf(str,"%d .%s(禁言)\n",i,L->data.name);
                    strcat(buf,str);
                }
                else
                {
                    sprintf(str,"%d .%s(在线)\n",i,L->data.name);
                    strcat(buf,str);
                }
                f=1;
                break;
            }
        }
        if(f ==0)
        {
            sprintf(str,"%d .%s(离线)\n",i,friends->name);
            strcat(buf,str);
        }
        friends=friends->next;
        i++;
    }
    strcpy(a->name,clientNode->name);
    strcpy(a->msg,buf);
    send(clientNode->decr,a,sizeof(struct message),0);
    printf("test2:消息包内容：%s\n",a->msg);
    printf("已向%s发送好友查看回执\n",a->name);
}
/****************************
函数名：get_friend_list
参数：char* name
功能：载入好友列表
返回值：该好友类型的结构体指针
*****************************/
User_list * get_friend_list(char * name)
{
    int file_fd;
    char usr_name[MAX_LEN];
    if((file_fd = open(name,O_RDWR|O_CREAT,0777))== -1)
    {
        perror("friend_list open error ");
        return NULL;
    }

    User_list * head,*p;

    head =(User_list *) malloc(sizeof(User_list));
    if(head == NULL)
    {
        printf("malloc error!\n");
        return NULL;
    }

    head->next = NULL;

    while(read(file_fd,usr_name,(size_t) MAX_LEN+1) > 0)
    {
        p = (User_list *) malloc(sizeof(User_list));
        printf("read %s\n",usr_name);
        strcpy(p->name,usr_name);
        p->next = head ->next;
        head->next = p;
    }

    close(file_fd);
    //printf("test:show findlist:\n");
    //for(p=head->next;p != NULL;p=p->next)
    //{
    //    printf("%s\n",p->name);
    //}

    return head;
}
/****************************
函数名：add_friend
参数：messag a,clientinf clientNode
功能 从want向wanted发送好友邀请
返回值：0表示发送失败 1表示发送成功
****************************/
int add_friend( struct message *a,clientinf *clientNode)
{
    datatype e;
    LinkList L=NULL;
    if(strcmp(a->name,a->addressee)==0)
    {
        strcpy(a->flag,"sermsg");
        strcpy(a->msg,"不能添加自己为好友");
        send(clientNode->decr,a,sizeof(struct message),0);
    }
    //L=L->next;
    strcpy(e.name,a->addressee);
    L = (LinkList)findlist(clientlink,e);
    if(L == NULL)
    {
        strcpy(a->flag,"sermsg");
        sprintf(a->msg,"该用户不在线\n");
        send(clientNode->decr,a,sizeof(struct message),0);
        return 0;
    }
    else
    {
        strcpy(a->flag,"friend_wanted");
        send(L->data.decr,a,sizeof(struct message),0);
        printf("好友请求已经发送\n");
    }
    return 1;
}

/*********************************
函数名：recv_friend
参数： message* a, clientNode clientlink
功能：处理一个好友接受请求
返回值：无
***********************************/
void recv_friend(struct message *a,clientinf *clientNode)
{
    struct message send_msg;
    datatype e;
    strcpy(e.name,a->name);
    LinkList L = NULL;
    L =(LinkList)findlist(clientlink,e);
    if(L == NULL)
    {
        printf("Not Find %s \n",a->name);
        return;
    }

    if(strcmp(a->msg,"no") == 0)
    {
        printf("no\n");
        strcpy(send_msg.msg,"receive!");
        strcpy(send_msg.flag,"sermsg");

    }
    else
    {
        if(strcmp(a->msg,"yes") == 0)
        {
            printf("yes\n");
            strcpy(send_msg.msg,"accept!");
            strcpy(send_msg.flag,"sermsg");
            int fd1 = open(a->name,(O_RDWR|O_APPEND|O_CREAT),0777);
            int fd2 = open(a->addressee,(O_RDWR|O_CREAT|O_APPEND),0777);
            if(fd1 == 0 ||fd2 == 0)
            {
                perror("open error ");
                return;
            }
            write(fd1,a->addressee,MAX_LEN+1);
            write(fd2,a->name,MAX_LEN+1);
            close(fd1);
            close(fd2);
            datatype temp1,temp2;
            //temp1 = (datatype *)malloc(sizeof(datatype));
            //temp2 = (datatype *)malloc(sizeof(datatype));
            strcpy(temp1.name,a->name);
            strcpy(temp2.name,a->addressee);
            L=NULL;
            L = (LinkList)findlist(clientlink,temp1);
            if(L == NULL)
            {
                printf("Not fond!\n");
                return;
            }

            User_list *t;
            t=(User_list*)malloc(sizeof(User_list));
            strcpy(t->name,a->addressee);
            t->next = (User_list*)L->data.friend_list->next;
            L->data.friend_list ->next = t;
            L=NULL;
            L = (LinkList)findlist(clientlink,temp2);
            {
                if(L == NULL)
                {
                    printf("NOT FOND!\n");
                    return;
                }
            }

            t= (User_list *)malloc(sizeof(User_list));
            strcpy(t->name,a->name);
            t->next =(User_list*) L->data.friend_list->next;
            L->data.friend_list->next = t;
            printf("write down!\n");
        }
        
    }
    send(L->data.decr,&send_msg,sizeof(struct message),0);
    printf("回执成功！\n");
}

/****************************
 函数名：CreateLinkList
 参数：无
 功能：创建一个链表的头节点
 返回值：一个链表的头节点
 ***************************/
 LinkList CreateLinkList(void)
 {
     LinkList head;
     head = (LinkList)malloc(sizeof(LNode));
     if(head ==NULL)
     {
         printf("Created malloc error!\n");
         return NULL;
     }
     else
    {
        head ->next = NULL;
        return head;
    }
 }
 /**************************
函数名： deletelist
函数功能：删除链表中的一个节点
传入参数:LinkList L datatype e
返回值：无
*****************************/
void deletelist(LinkList L, datatype e)
{
    if(L==NULL||L->next ==NULL)
    return;
    LinkList p   = L->next;
    LinkList pre = L;
    while(p)
    {
        if(strcmp(p->data.name,e.name) == 0)
        {
            pre->next = p->next;
            free(p);
            break;
        }
        else
        {
            pre = p;
            p   = p->next;
        }
    }
}
/*****************************
函数名：insertend
函数功能： 在链表尾插入一个节点
传入参数：LinkList L datatype e
返回值无
*********************************/
void insertend(LinkList L,datatype e)
{
    if(L==NULL)
    {
        printf("L is empty !\n");
        return;
    }
    LinkList before =L->next;
    if(before = NULL)
    {
        printf("L->next is empty!\n");
        return ;
    }
    
    LinkList temp = (LinkList)malloc(sizeof(LNode));
    if (temp == NULL)
    {
        printf("malloc error \n");
        return;
    }
    //puts(e.name);
    temp->data = (e);
    LinkList t;
    for( t = L;t->next;t=t->next);
    temp->next =NULL;
    t->next =temp;
    //puts("show!");
    //DisplayList(L);
}
/****************************
函数名:friend_instert
函数功能：在链表尾部插入一个节点
传入参数：User_list * l, char *name
返回值：无
*****************************
void friend_instert(User_list *L,char*name)
{
    User_list *p=L,*tail;
    
    while((tail != NULL) && (tail->next != NULL))
    {
        tail=tail->next;
    }


}
*/

/****************************
函数名：DisplayList
函数功能：显示当前链表中所有信息
传入参数：LinkList L
返回值：无
******************************/
void DisplayList(LinkList L)
{
    if(L == NULL || L->next == NULL)
    return;
    for(LinkList p = L;p!=NULL;p=p->next)
    {
        printf("name:%s\tpasswd:%s\n",p->data.name,p->data.passwd);
    }
}
/***************************
函数名：findlist
函数功能：找到参数中对应结构体链表并返回
传入参数：LinkList L datatype E
返回值：对应节点
****************************/
LinkList findlist(LinkList L,datatype e)
{
    for(LinkList now=L;now != NULL;now=now->next)
    {
        if(strcmp(now->data.name,e.name)==0)
        {
            return now;
        }
    }
    return NULL;
}

