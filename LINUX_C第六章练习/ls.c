/*************************************************************************
	> File Name: ls.c
	> Author: 
	> Mail: 
	> Created Time: Thu 21 Jul 2016 09:02:01 GMT
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>
#include<pwd.h>
#include<grp.h>
#include <time.h>
#include<unistd.h>

#define A 10000
#define R 01000
#define U 00100
#define L 00010
#define T 00001
#define true 0
#define false 1

typedef struct node
{
    char fullname[255];
    char filename[255];
    time_t st_time;
    struct node *next;
}Node;

void get_st_mode(unsigned num,int *ans);
void drive(char *dirname,int flag);
void display_l(const char *filename,int flag);
void show_size(long int num);
int display(const char *dirname);
void strcopy(char*str,char*str1, char*str2);
void f_catalog(DIR*dir_ptr,char*dirname,int flag);
void show_dispose(DIR*dir_ptr,char*dirname,int flag);
void show_sort(Node*head,int flag);
void sort_dir(Node *head);
void show (DIR* dir_ptr,char *dirname,int flag);
Node *creat(DIR *dir_ptr,char *dirname,int flag);

int main(int argc,char *argv[])
{
    char dir_now[4096];
    getcwd(dir_now,sizeof(dir_now));
    int flag=00000,oc;
    while((oc=getopt(argc,argv,"aultR"))!=-1)
    {
        switch(oc)
        {
            case 'a':
            {
            flag=flag|A;
            break;
            }
            case 'u':
            {
                flag=flag|U;
                break;
            }
            case 'l':
            {
                flag=flag|L;
                break;
            }
            case 't':
            {
                flag=flag|T;
                break;
            }
            case 'R':
            {
                flag=flag|R;
                break;
            }
            default: break;
        }
    }
    if(argc==1||(argc==2&&((flag&L)||(flag&A)||(flag&R)||(flag&U)||(flag&T))))
    {
        drive(dir_now,flag);
    }
    else 
    {

        int i=1,j=argc;
        while(argv[i][0]!='/')
        {
          i++;
        }
        
        for (i; i < j; i++)
        {
            printf("%s:\n",argv[i]);
            char path[4096];
            strcpy(path,argv[i]);
            drive(path,flag);
            printf("\n");
        }
    }

    return 0;
}
void strcopy(char*str,char*str1,char*str2)
{
  char *p=str;
  strcpy(str,str1);
  p+=strlen(str1);
  strcpy(p,str2);
}
int display(const char *filename)
{
    printf("%s\t",filename);
    return 1;
}
void show_size(long int num)
{
    float big;
    if(num<1024)
     printf(" %5ldB",num);
    else if (num<1024*1024)
    printf("%5.1fKB",((float)num)/1024.0);
    else if (num<1024*1024*1024)
    printf("%5.1fMB",((float)num)/(1024.0*1024.0));
    else printf("%5.1lfGB",((float)num)/(1024.0*1024.0*1024.0));
    printf(" ");
}
void display_l(const char * filename,int flag)
{
    int ans[4],i;
    struct stat buf;
    struct passwd* pwd;
    struct group*  grp;
    if(stat(filename,&buf) == -1)
    {
        perror("stat");
        return ;
    }
    get_st_mode(buf.st_mode,ans);
    switch(ans[3])
    {
                case(1): printf("l"); break;
                case(2): printf("-"); break;
                case(3): printf("d"); break;
                case(4): printf("c"); break;
                case(5): printf("b"); break;
                case(6): printf("f"); break;
                case(7): printf("s"); break;
                default: break;           
    }
        for(i=0;i<3;i++)
    {
            if(ans[i]%2==0) printf("r");
            else  printf("-");
            if(ans[i]%3==0) printf("w");
            else printf("-");
            if(ans[i]%5==0) printf("x");
            else printf("-");
    }
        printf("\t");
    printf(" %3ld ",buf.st_nlink);
    if((pwd=getpwuid(buf.st_uid))==NULL)
       printf(" %-3d ",buf.st_uid);
       else 
       printf(" %5s ",pwd->pw_name);
    if((grp=getgrgid(buf.st_gid))==NULL)
    printf(" %5d ",buf.st_gid);
    else 
       printf(" %5s ",grp->gr_name);
    show_size(buf.st_size);
    char time[100];
    if(flag==true)
    strcpy(time,ctime(&buf.st_mtime));
    else 
    strcpy(time,ctime(&buf.st_atime));
    for(i=0;i<10;i++)
    printf("%c",time[i]);
}

void get_st_mode(unsigned num,int *ans)
{
        //r==2,w==3,x==5//
    ans[1]=ans[2]=ans[0]=1;
    if(num&S_IRUSR)
    ans[0]*=2;
    if(num&S_IWUSR)
    ans[0]*=3;
    if(num&S_IXUSR)
    ans[0]*=5;

    if(num&S_IRGRP)
    ans[1]*=2;
    if(num&S_IWGRP)        
    ans[1]*=3;  
    if(num&S_IXGRP)
    ans[1]*=5;

    if(num&S_IROTH)
    ans[2]*=2;
    if(num&S_IWOTH)
    ans[2]*=3;
    if(num&S_IXOTH)
    ans[2]*=5;

    //判断文件类型//
    ans[3]=0;
    if(S_ISLNK(num)) ans[3]=1;
    else if(S_ISREG(num)) ans[3]=2;
    else if(S_ISDIR(num)) ans[3]=3;
    else if(S_ISCHR(num)) ans[3]=4;
    else if(S_ISBLK(num)) ans[3]=5;
    else if(S_ISFIFO(num))ans[3]=6;
    else if(S_ISSOCK(num))ans[3]=7;
}
void drive(char *dirname,int flag)
{
    int cont=0;
    DIR *dir_ptr;
    if((dir_ptr=opendir(dirname))==NULL)
    {
        perror("opendir:");
        return ;
    }
    else
    {
        if(flag&R)
        {
            flag=(flag&(~R));
            f_catalog(dir_ptr,dirname,flag);
        }
        else
        {
            show_dispose(dir_ptr,dirname,flag);
        }
    }

    closedir(dir_ptr);
}

void f_catalog(DIR*dir_ptr,char*dirname,int flag)
{
    DIR *dir_ptr2;
    struct dirent *direntp;
    struct stat buf;
    dir_ptr2=opendir(dirname);

    if(dir_ptr2 == NULL) {
        perror("");
        return ;
    }

    show_dispose(dir_ptr,dirname,flag);
    while((direntp=readdir(dir_ptr2))!=NULL)
    {
        int l;
        if((strcmp(direntp->d_name,".")==0)||(strcmp(direntp->d_name,"..")==0)||(strcmp(direntp->d_name,"./.")==0))
        continue;
        char dname[4096];
        char daname[4096];
        strcpy(dname,dirname);
        l=strlen(dname);
        
        if (dname[l - 1] != '/') {
            dname[l]='/';
            dname[l+1]='\0';
        }
        
        strcopy(daname,dname,direntp->d_name);
        
        if((stat (daname,&buf))==-1)
        {
            puts(daname);
            perror("stat_f_catalog:");
            continue;
        }

        if(S_ISDIR(buf.st_mode))
        {
            if((flag&A)==0&&(direntp->d_name[0]=='.'))
            continue;
            printf("\n%s:\n",daname);
            f_catalog(dir_ptr2,daname,flag);
        }
    }
    closedir(dir_ptr2);
}

void show_dispose(DIR*dir_ptr,char*dirname,int flag)
{
    int u=false;
    if(flag&U)
    u=true;
    DIR* dir_ptr2;
    dir_ptr2=opendir(dirname);
    if(flag&T)
    {
        Node *head;
        head=creat(dir_ptr2,dirname,u);
        sort_dir(head);
        show_sort(head,flag);
    }
    else
    show(dir_ptr2,dirname,flag);
    closedir(dir_ptr2);

}
void show (DIR* dir_ptr,char *dirname,int flag)
{
    int Flag,cout=0,l;
    struct dirent *direntp;
    struct stat buf;
    l=strlen(dirname);
    dirname[l]='/';
    dirname[l+1]='\0';
    while((direntp=readdir(dir_ptr))!=NULL)
    {
        char fullname[4096];
        strcopy(fullname,dirname,direntp->d_name);
        if(((flag&A)==0)&&(direntp->d_name[0]=='.'))
        continue;
        if(flag&U)
        Flag=true;
        if(flag&L)
        {
        display_l(fullname,Flag);
        printf("\t%s\n",direntp->d_name);
        }
        else 
        {
            display(direntp->d_name);
            cout++;
            if(cout%5==0)
            printf("\n");
        } 
    }
}
void show_sort(Node*head,int flag)
{
    int cont=0;
    Node*p=head->next;
    for(p;p!=NULL;p=p->next)
    {
        if((flag&A)&&(p->fullname[0]=='.'))
        continue;
        if(flag&&L)
        {
        display_l(p->fullname,flag);
        printf("\t%s\n",p->filename);
        }
        else
        {
            display(p->fullname);
            cont++;
            if(cont%5==0)
            printf("\n");
        }
    }
}

void sort_dir(Node *head)
{
  Node* p3=head;
  Node* p2,*p1,*p4,*p5;
  p2=p3->next;
  p5=p2;
  p1=p2;
  int flag=false;
  while(p1->next!=NULL)//内存循环终止的条件
  {
   if(p5->st_time<p1->next->st_time)//找出原链表中剩余节点中值最小的那个节点
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
}

Node *creat(DIR *dir_ptr,char *dirname,int flag)
{
    char fullname[4096];
    Node *head,*p,*q;
    struct dirent *direntp;
    struct stat buf;
    head =(Node*)malloc(sizeof(Node));
    q=head->next=NULL;
    while((direntp=readdir(dir_ptr))!=NULL)
    {
        p=(Node*)malloc(sizeof(Node));
        if(p==NULL)
        {
            puts("malloc error");
            exit(0);
        }
        strcpy(p->filename,direntp->d_name);
        strcopy(fullname,dirname,direntp->d_name);
        stat(fullname,&buf);
        if(flag=true)
        p->st_time=buf.st_atime;
        else 
        p->st_time=buf.st_ctime;
        strcopy(p->fullname,dirname,direntp->d_name);
        p->next=q;
        q=p;
    }
    return head;
}
