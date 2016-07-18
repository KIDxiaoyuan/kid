#include<string.h>
#include<stdio.h>
#include<stdlib.h> 
#include <unistd.h>
#define N 10                //课程和奖学金种类的总数量和
//科目信息 
typedef struct SCORE{
    char course_name[32];
    float score;
}Score;
//管理员信息 
typedef struct admin
{
    int auth;
    char usr_name[32];
    char passwd[18];
    struct admin *next;
}Admin;
//学生信息 
typedef struct node{
    int auth;
    char usr_name[32];
    char password[18];
    int id;
    char stu_name[32];
    char major[32];
    Score s[N];
    unsigned all_score;
    struct node* next;
}Node;
///////////////////全局变量/////////////////////// 
Node *head,*last;
Admin *ad_head,*ad_last;
int usr_type;
char usr_name[32],password[18];
const char* f_name="usr_info.conf";
const char* f_name2="admin_info.conf";
char str1[]="Student Information Management System";
//////////////////////函数声明///////////////////////
int login();
void load_data(),save_data(),export_data(),check_admin(),add_admin();
void add_student(),search_student(),del(),modify(),modify_password(),logout();
void show_score(),search_course();
void ad_login(),add_student_more(),  final_score(Node*p);
void sort_score (void),print(Node*p);
void title_win(),msg_win(const char*str);
void student_score(int stu_id);
/////////////////////////////////////////////////
int main()
{
    int ch,number;
    int ID;
    load_data();
    title_win();
    check_admin();
    
loop:
    printf("请选择用户类型:\n1:admin\n2:student\n");
    scanf("%d",&usr_type);
    if(usr_type!=1 && usr_type!=2)
    {
        msg_win("没有该用户类型，请重新输入，1表示admin，2表示student\n");
        goto loop;
    }
    
    if(usr_type==1)
    {
         title_win();
        ad_login();
        while(1)
        {
            msg_win("User Type:Admin");
            printf("1：添加学生信息\n2：搜索学生信息\n3：删除学生信息\n4：修改学生信息\n5：修改管理员密码\n6：登出管理员账户\n7：导出数据到文件\n8：保存当前数据\n9：添加管理员账户\n10：批量添加学生信息\n11：显示所有学生信息\n12：将学生信息按照总成绩排序\n");
            printf(">");
            scanf("%d",&ch);
            switch(ch)
            {
                case 1:add_student();break;
                case 2:search_student();break;
                case 3:del();break;
                case 4:modify();break;
                case 5:modify_password();break;
                case 6:logout();break;
                case 7:export_data();break;
                case 8:save_data();break;
                case 9:add_admin();break;
                case 10 :add_student_more();break;
                case 11:show_score();break;
                case 12: sort_score();break;
                default: break;
            }
            getchar();
            msg_win("按任意键返回");
            getchar();
            system("clear");
        }
    
    }
    else
    {
         title_win();
        ID=login();       
        while(1)
        {
            msg_win("User Type:Student");
            msg_win("1:显示成绩/n2:修改密码/n3：退出");
            printf(">");

            scanf("%d",&ch);
            
            switch(ch)
            {
                case 1:student_score(ID);break;
                case 2:modify_password();break;
                case 3:logout();break;
                default:break;
            }
            getchar();
            msg_win("按任意键返回");
            getchar();
            system("clear");
        } 
    
    }
    return 0;
}
/////////////////////信息读取函数////////////////////// 
void load_data()
{  
    FILE* fp=NULL;
    FILE* fq=NULL; 
    if(!(fp=fopen(f_name,"a")))
    {
        msg_win("Open file fail");
        return;
    }
    if(!(fq=fopen(f_name2,"a")))
    {
        msg_win("Open file fail");
        return;
    }
    if(!(fp=fopen(f_name,"r+")))
    {
        msg_win("Open file 1 fail");
        return;
    }
    if(!(fq=fopen(f_name2,"r+")))
    {
        msg_win("Open file 2 fail");
        return;
    }
    Node* p;
    Admin*q; 
   //文件读取学生信息 
    while(fp)
    {    
        p=(Node*)malloc(sizeof(Node));
        if(!p)
		{
		printf("!p\n");
		exit(1);
		}
        if(!fread(p,sizeof(Node),1,fp))
            break;     
        if(head==NULL)
        {
            head=last=p;
            head->next=NULL;
        }
        else
        {
            last->next=p;
            last=p;
            last->next=NULL;
        } 
    }
     //文件读取管理员信息 
     while(fq)
     {
     	q=(Admin*)malloc(sizeof(Admin));
     	if(!q) 
     	{
     		printf("!q\n");
     		exit(1);		
		 }
		 if(!fread(q,sizeof(Admin),1,fq))
		 break;
		 if(ad_head==NULL)
		 {
            ad_head=ad_last=q;
            ad_head->next=NULL;		 	
		 }
        else
        {
            ad_last->next=q;
            ad_last=q;
            ad_last->next=NULL;
        } 		 
	 }	  
     fclose(fp);
     fclose(fq);
     fq=fp=NULL;     
}
///////////////////////////信息保存函数//////////////////////// 
void save_data()
{
    FILE* fp=NULL;
    FILE* fq=NULL;
    if(!(fp=fopen(f_name,"w+")))
    {
        msg_win("Open file fail");
        return;
    }
    if(!(fq=fopen(f_name2,"w+")))
    {
        msg_win("Open file fail");
        return;   	
	}

    Node* p;
    Admin*q;
    p=head;
    if(p==NULL)
    {    
	printf("p==NULL\n");
	return ; 	
	}


    while(p)
    {
        fwrite(p,sizeof(Node),1,fp);
         p=p->next;
    }
    q=ad_head;
//    fprintf(fp,"#");
    while(q)
    {
    	fwrite(q,sizeof(Admin),1,fq);
    	q=q->next;
	}
    fclose(fp);
    fclose(fq);
    fp=fq=NULL;
}

/////////////////导出信息文件////////////// 
void export_data()
{
    FILE* fp;
    if(!(fp=fopen("export.log","w+")))
    {
        msg_win("Open file fail");
        return;
    }
    
    Node* p;
    Admin *q;
    int i;
    p=head;
    q=ad_head;
    while(p)
    {
        fprintf(fp,"%-2d|%-15s|%-18s|%-15s|%-10d|%-18s|\n",p->auth,p->usr_name,p->password,p->stu_name,p->id,p->major);
        for(i=0;i<N;i++)
                fprintf(fp,"%s:%-5.1f|",p->s[i].course_name,p->s[i].score);
        fprintf(fp,"\n");
        fprintf(fp,"SCORE:%d",p->all_score); 
         p=p->next;
    }
    while(q)
    {
    	fprintf(fp,"%d|%s\n",q->auth,q->usr_name);
    	fprintf(fp,"%s\n",q->passwd);
    	q=q->next;
	}
    fclose(fp);
}
/////////////管理员账户设置/////////////////////// 
void check_admin()
{
    Admin* p;
    p=ad_head; 
    if(p)
    return ;
    p=(Admin*)malloc(sizeof(Admin));
    p->auth=1;
    msg_win("请设置管理员账户");
    printf("用户名:");
    scanf("%s",p->usr_name);
    printf("密码:");
    scanf("%s",p->passwd);
    
    if(ad_head==NULL)
    {
        ad_head=ad_last=p;
        ad_head->next=NULL;
    }
    else
    {
        ad_last->next=p;
        ad_last=p;
        ad_last->next=NULL;
    }       
    msg_win("管理员账户设置成功");
    sleep(1);
    system("clear");
}
////////////添加管理员账户/////////////// 
void add_admin()
{
    Admin* p;
    p=(Admin*)malloc(sizeof(Admin));
    p->auth=1;
    msg_win("添加管理员账户");
    printf("用户名:");
    scanf("%s",p->usr_name);
    printf("密码:");
    scanf("%s",p->passwd);
    
    if(ad_head==NULL)
    {
        ad_head=ad_last=p;
        ad_head->next=NULL;
    }
    else
    {
        ad_last->next=p;
        ad_last=p;
        ad_last->next=NULL;
    }       
    msg_win("新管理员账户设置成功");

}
/////////////学生账户登录///////////////// 
int login()
{
login: system("clear");
    msg_win("登录");
    printf("请输入学生姓名（ID）："); 
    scanf("%s",usr_name);
    printf("密码:");
    scanf("%s",password);
    Node* p;
    p=head;
    while(p)

    {
        if(!strcmp(p->usr_name,usr_name) && !strcmp(p->password,password) && p->auth==usr_type)
        {
            msg_win("登录成功");
            sleep(1);
            system("clear");
            return p->id;
        }
        p=p->next;
    }
    msg_win("用户名或密码不正确,按任意键重新登录");
    getchar();getchar();
        goto login;
}
////////////////管理员账户登录////////////// 
void ad_login()
{

    login:system("clear");
    msg_win("管理员登录");
    printf("用户名：");
    scanf("%s",usr_name);
    printf("密码：");
    scanf("%s",password);
    Admin *p;
    p=ad_head;
    while(p)
    {
        if(!strcmp(p->usr_name,usr_name)&&!strcmp(p->passwd,password))
        {
            msg_win("登录成功");
            sleep(1);
            system("clear");
            return;

        }
        p=p->next;
    }
    msg_win("用户名或密码不正确，按任意键重新登录");
    goto login;
}
////////////////单个添加学生信息////////////// 
void add_student()
{
	int subject_num;
    Node* p=(Node*)malloc(sizeof(Node));
    if(!p)  return ;
    p->auth=2;
    msg_win("添加学生账户");
    printf("用户名（姓名）:");
    scanf("%s",p->usr_name);
    printf("密码:");
    scanf("%s",p->password);
    printf("姓名:");
    scanf("%s",p->stu_name);
    printf("学号:");
    scanf("%d",&(p->id));
    printf("专业:");
    scanf("%s",p->major);
    puts("科目数量：");
	scanf("%d",&subject_num);
	for(int i=0;i<subject_num;i++) 
	{
	printf("科目：");
    scanf("%s",p->s[i].course_name);
    printf("成绩：");
    scanf("%f",&p->s[i].score);
    } 
    final_score(p);
    if(head==NULL)
    {
        head=last=p;
        head->next=NULL;
    }
    else
    {
        last->next=p;
        last=p;
        last->next=NULL;
    }       
    msg_win("学生账户设置成功");
}

void search_student()
{
    msg_win("搜索账户信息");
    int stu_id,i;
    printf("请输入学号:");
    scanf("%d",&stu_id);
    
    Node* p;
    p=head;
    while(p)
    {
        if(p->id==stu_id)
        {
            printf("%d %s %s %s %d %s\n",p->auth,p->usr_name,p->password,p->stu_name,p->id,p->major);
            for(i=0;i<N;i++)
                printf("%s:%-5.1f",p->s[i].course_name,p->s[i].score);
            printf("\n");
            return;
        }
        p=p->next;
    }
    msg_win("系统中没有该学号");
}

void del()
{
    Node *p,*q;
    int stu_id;
    
    msg_win("删除账户信息");
    printf("请输入学号:");
    scanf("%d",&stu_id);
    
    p=head;
    while(p)
    {
        if(p->id==stu_id)
        {
            if(head==p)
                head=head->next;
            else
                q->next=p->next;            
            
            free(p);
            msg_win("删除成功！");
            return;
        }
        q=p;
        p=p->next;
    }
    msg_win("删除失败！系统中没有该学号");    
}

void modify()
{
    int stu_id,i;
    
    msg_win("修改课程成绩和奖学金");
    printf("请输入学号:");
    scanf("%d",&stu_id);
    
    Node* p;
    p=head;
    while(p)
    {
        if(p->id==stu_id)
        {
            for(i=0;i<N;i++)
                scanf("%s %f",p->s[i].course_name,&(p->s[i].score));
            return;
        }
        p=p->next;
    }
    msg_win("系统中没有该学号");    
}
//////////////////////////////////////////////////////密码修改函数///////////////////////////////////////////////////
void modify_password()
{
    char temp[18],new_password[18];
    
    msg_win("修改密码");
    
    printf("请输入旧密码:");
    scanf("%s",temp);
    printf("请输入新密码:");
    scanf("%s",new_password);
    printf("请确认新密码:");
    scanf("%s",password);
    
    Node* p;
    p=head;
    while(p)
    {
        if(!strcmp(usr_name,p->usr_name))
        {
            if(!strcmp(p->password,temp)&& !strcmp(new_password,password))
            {
                strcpy(password,new_password);
                strcpy(p->password,password);
                msg_win("密码修改成功");
                return;
            }
            else
            {
                msg_win("输入有误，自动退出");
                return;
            }
        }
        p=p->next;
    }
}
/////////////////////////////////登出函数///////////////////////
void logout()
{
    save_data();
    msg_win("成功退出");
    exit(1);
}
///////////////////////////////////////全体成员成绩显示函数/////////////////////////////////////////////////////////////////////////
void show_score()
{
    int i;
    Node* p;
    p=head;
    msg_win("所有用户信息");
    while(p)
    {
     printf("用户名：%s\n 密码：%s\n 学生姓名： %s\n 学号： %d\n 专业： %s\n",p->usr_name,p->password,p->stu_name,p->id,p->major);
     for(i=0;i<N;i++)
     {
    	if(p->s[i].score<1.0)
    	break;
       printf("%s:%-5.1f",p->s[i].course_name,p->s[i].score);
	 }
	 printf("总分：%d",p->all_score);
     printf("\n");
     p=p->next;
    }
}
void print(Node*p)
{
	printf("----------\n");
	 printf("用户名：%s\n 密码：%s\n 学生姓名： %s\n 学号： %d\n 专业： %s\n",p->usr_name,p->password,p->stu_name,p->id,p->major);
     for(int i=0;i<N;i++)
     {
    	if(p->s[i].score<1.0)
    	break;
       printf("%s:%-5.1f",p->s[i].course_name,p->s[i].score);
	 }	
}
////////////////////搜索函数///////////////////// 
void search_course()
{
    char course_name[32];
    int i;
    msg_win("输入要查询的课程");
    printf(">");
    scanf("%s",course_name);
    
     Node* p;
    p=head;
    while(p)
    {
     if(!strcmp(usr_name,p->usr_name))
     {  
        for(i=0;i<N;i++)
         {      
            if(!strcmp(course_name,p->s[i].course_name))
             {
               printf("%s:%-5.1f\n",p->s[i].course_name,p->s[i].score);
               return;
              }
        }
      }
        p=p->next;
    }
}
/////////////////////标题显示函数/////////////////////////////////////////////////////
void title_win()
{
     int i,M;
    
     M=strlen(str1);
    for(i=0;i<M;i++)   printf("-");
    printf("\n");
    printf("%s\n",str1);
    for(i=0;i<M;i++)   printf("-");
    printf("\n");
}
//////////////////消息处理函数///////////////////////////////// 
void msg_win(const char* str)
{
    int i,M;
    
    M=strlen(str1);
    printf("\n");
    printf("%s\n",str);
    for(i=0;i<M;i++)   printf("-");
    printf("\n");   
}
///////////////////批量添加学生信息函数///////////////////////// 
void add_student_more()
{
	Node* p,*q=last;
	char passwd[18],subject_name[N][32],smajor[32];
	int start_num,i,subject_num,number,j;
	printf("请输入学生数：");
	scanf("%d",&number);
	printf("请设置通用密码：\n");
	scanf("%s",passwd);
	 printf("专业:");
    scanf("%s",smajor);
	printf("请输入起始学号:");
	scanf("%d",&start_num);
	printf("您要添加多少科目？\n");
	scanf("%d",&subject_num);
	for(i=0;i<subject_num;i++)
	{
		printf("请输入第 %d 个科目名称\n",i+1);
		scanf("%s",subject_name[i]);
	}
   p=(Node*)malloc(sizeof(Node));
    if(!p)  return ;
    p->auth=2;
    msg_win("添加学生账号信息");
    for(i=0;i<number;i++,start_num++)
    {
  	strcpy(p->password,passwd);
    printf("姓名：");
    scanf("%s",p->stu_name);
    strcpy(p->usr_name,p->stu_name);
 	p->id=start_num;
	strcpy(p->major,smajor);
	for(j=0;j<subject_num;j++)
	{
	
	printf("请输入 %s 成绩",subject_name[j]);
	strcpy(p->s[j].course_name,subject_name[j]);
	scanf("%f",&p->s[j].score);
	}
	final_score(p);
    if(head==NULL)
        {
            head=last=p;
            head->next=NULL;
        }
    else
        {
            q->next=p;
            q=q->next;
        }
	 p=(Node*)malloc(sizeof(Node));
	}
	last=q;
	q->next=NULL;
    
    msg_win("学生账户设置成功");
}
void sort_score (void)
{
 Node* p3=head;
 Node* p2,*p1,*p4,*p5;
  p2=p3->next;
  p5=p2;
  p1=p2;
  bool flag=false;
  while(p1->next!=NULL)//内存循环终止的条件
  {
   if(p5->all_score<p1->next->all_score)//找出原链表中剩余节点中值最小的那个节点
   {
    p5=p1->next;
    p4=p1;
    flag=true;
   }
   p1=p1->next;
  }
  if(flag)//交换两个节点的顺序，值小的节点往前调
  {
   if(p2==p4)//此种情况为要交换顺序的两个节点相邻
   {
    p2->next=p5->next;
    p5->next=p2;
    p3->next=p5;
   }
   else//这种情况为要交换的两个节点不相邻
   {
    Node*temp=p5->next;
    p5->next=p2->next;
    p3->next=p5;
    p4->next=p2;
    p2->next=temp;
   }
   
  }
  p3=p3->next;
 Node*temp=head;
 temp=temp->next;


 while(temp!=NULL)
 {
  print(temp);
  temp=temp->next;
 }
}
 void final_score(Node*p)
 {
 	int i,score=0;
 	for(i=0;p->s[i].score>1;i++)
 	score+=p->s[i].score;
 	p->all_score=score;
}
void student_score(int stu_id)
{
	Node*p;
	for(p=head;p!=NULL;p=p->next) 
	{
		 if(p->id==stu_id)
        {
            printf("账户名：%s  姓名：%s 学号：%d 专业：%s\n",p->usr_name,p->stu_name,p->id,p->major);
            for(int i=0;i<N;i++)
            {
			    	if(p->s[i].score<1.0)
    	 			break; 
                printf("%s:%-5.1f",p->s[i].course_name,p->s[i].score);
            } 
            printf("总分：%d\n",p->all_score);
            printf("\n");
            return;
	     }
	 }
} 
