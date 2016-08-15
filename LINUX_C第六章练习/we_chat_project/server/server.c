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
extern LinkList CreatLinkList(void);
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
////////////////////////////////////////////////////////////////////////////
int main (void)
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
    clientlink = CreatLinkList();
    if(fd = open("chat_log_server.txt",O_RDWR|O_CREAT|O_APPEND,0777)<0)
    {
        perror("fail to open chat_log_server.txt ");
        exit(1);
    }
    int lfd,cfd,sfd,rdy;
    struct sockaddr_in sin;
    struct sockaddr_in cin;
    int client[FD_SETSIZE];
    int maxi,maxfd;//最大连接数
    fd_set rset;
    fd_set allset;
    socklen_t addr_len;
    char buffer[MAX_LINE];
    int i,n,len,opt = 1;//套接字选项
    char addr_p[20];

    //设置server_addr_in
    bzero(&sin,sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(portnumber);

    //创建TCP协议套接字接口
    if((lfd = socket(AF_INET,SOCK_STREAM,0))== -1)
    {
        perror("socket error ");
        exit(1);
    }
    setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    //绑定套接字到sockfd
    if(bind(lfd,(struct sockaddr*)(&sin),sizeof(struct sockaddr)) == -1)
    {
        perror("bind error ");
        exit(1);
    }
    //监听端口，等待客户端请求
    if(listen(lfd,20) == -1)
    {
        perror("listen error ");
        exit(1);
    }

    puts("connecting……");
    //初始化最大文件描述符
    maxfd = lfd;
    maxi = -1;
    //初始化客户端连接描述符
    for(i = 0;i < FD_SETSIZE ; i++)
    {
        client[i]= -1;
    }
    FD_ZERO(&allset);
    FD_SET(lfd,&allset);
    //服务程序处理
    while(1)
    {
        rset = allset;
        rdy = select(maxfd+1,&rset,NULL,NULL,NULL);
        if(FD_ISSET(lfd,&rset))
        {
            addr_len = sizeof(sin);
            //接收客户端请求
            if((cfd = accept(lfd,((struct sockaddr*)(&sin)),&addr_len)) == -1)
            {
                perror("accept error ");
                exit(1);
            }
            //查找空闲
            for(i = 0;i<FD_SETSIZE;i++)
            {
                if(client[i] <= 0)
                {
                client[i]=cfd;//连接套接字
                break;
                }
            }
            //当连接客户端 超过最大值时 跳出循环
            if(i == FD_SETSIZE)
            {
                puts("too many client to connect !");
                break;
            }
            FD_SET(cfd,&allset);
            if(cfd > maxfd)
            {
                maxfd = cfd;
            }
             if(i>maxi)
            {
            maxi = i;
            }
            if(--rdy <= 0)
            continue;
        }
        //对每个连接描述符处理
        for(i = 0;i<FD_SETSIZE;i++)
        {
            if((sfd = client[i] ) < 0)
            {
                continue;
            }
            if(FD_ISSET(sfd,&rset))
            {
                if(--rdy < 0)
                break;//没有可读套接字时跳出循环
            }
            re = recv(sfd,&a,sizeof(a)+1,0);
            clientNode.decr = sfd;
            strcpy(clientNode.name,a.name);
            if(re == 0)
            {
                server_exit(&a,&clientNode);
                fflush(stdout);
                close(sfd);
                FD_CLR(sfd,&allset);
                client[i] = -1;
            }
            else
            {
                if(strcmp(a.flag,"login") == 0)//登录模式
                {
                    //不在线
                    if(overlap(a) == 0)
                    {
                        server_login(&a,&clientNode);
                    }
                    else//在线
                    {
                        strcpy(a.msg,"overlap");
                        send(clientNode.decr,&a,sizeof(struct message),0);
                    }
                    continue;
                }
                else if(strcmp(a.flag,"reg") == 0)//注册模式
                {
                    server_reg(&a,&clientNode);
                    continue;
                }
                else if(strcmp(a.flag,"all") == 0)//群聊模式
                {
                    if(server_all(&a,&clientNode) == 1)//将聊天记录保存到 服务器
                    {
                        memset(str,0,strlen(str));
                        time(&timep);
                        sprintf(str,"[%s] %s to %s : %s\n",ctime(&timep),a.name,a.flag,a.msg);
                        puts(str);
                        write(fd,str,strlen(str));
                    }
                    continue;
                }
                else if(strcmp(a.flag,"personal") == 0)//私聊模式
                {
                    if(server_personal(&a,&clientNode) == 1)
                    {
                        memset(str,0,strlen(str));
                        time(&timep);
                        sprintf(str,"[%s] %s to %s : %s\n",ctime(&timep),a.name,a.addressee,a.msg);
                        puts(str);
                        write(fd,str,strlen(str));
                    }

                }
                else if (strcmp(a.flag,"view") == 0)//查看在线用户人数
                {
                    server_view(&a,&clientNode);
                    continue;
                }
                else if (strcmp(a.flag,"admin_kick") == 0)//踢人操作
                {
                    int closefd;
                    closefd = server_admin_kick(&a,&clientNode);
                    FD_CLR(closefd,&allset);
                    for(int j =0;j<FD_SETSIZE;j++)
                    {
                        if(closefd == client[j])
                        {
                            client[j] == -1;
                            break;
                        }
                    }
                    continue;
                }
                else if (strcmp(a.flag,"trans") == 0)//文件传送操作
                {
                    server_transf(&a);
                    continue;
                }
                else//容错处理 
                {
                    printf("error input :%s\n",a.flag);
                    continue;
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
        fprintf(stderr,"SQLerror :%s",sqlite3_errmsg(db));
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
	rc = sqlite3_open("./src/db/chat.db",&db);     
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
			fprintf(stderr,"SQLerror:%s",sqlite3_errmsg(db));
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
    if (strcmp((*a).msg,"") != 0)
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
	int i;
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
			printf("%s 登录成功!\n",(*a).name);
			memset((*a).msg,0,strlen((*a).msg));
			strcpy((*a).msg,"login,success!");
			strcpy((*clientNode).name,(*a).name);
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
	sprintf((*a).msg,"%s 离开聊天室",(*clientNode).name);
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
	    printf("%s 登录失败!\n",(*a).name);
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
/****************************
 函数名：CreatLinkList
 参数：无
 功能：创建一个链表的头节点
 返回值：一个链表的头节点
 ***************************/
 LinkList CreatLinkList(void)
 {
     LinkList head;
     head = (LinkList)malloc(sizeof(LNode));
     if(head ==NULL)
     {
         printf("Created malloc error!\n");
         return NULL;
     }
     else
     return head;
 }
 /**************************
函数名： deletelist
函数功能：删除链表中的一个节点
传入参数:LinkList L datatype e
返回值：无
*****************************/
void deletelist(LinkList L, datatype e)
{
    for (LinkList head = L;strcmp(head->next->data.name,e.name)!=0;head = head->next)
    {
        LinkList temp;
        temp = head->next;
        head->next = temp->next;
        free(temp);
    }
}
/*****************************
函数名：insertend
函数功能： 在链表头插入一个节点
传入参数：LinkList L datatype e
返回值：无
*********************************/
void insertend(LinkList L,datatype e)
{
    if(L==NULL)
    return;
    LinkList before =L->next;
    if(before = NULL)
    return;
    LinkList temp = (LinkList)malloc(sizeof(LNode));
    if (temp == NULL)
    {
        printf("malloc error \n");
        return;
    }
    temp->data = e;
    temp->next = before;
    L->next = temp;
}
/****************************
函数名：DisplayList
函数功能：显示当前链表中所有信息
传入参数：LinkList L
返回值：无
******************************/
void DisplayList(LinkList L)
{
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

