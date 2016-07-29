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

#define A 2
#define R 3
#define U 5
#define L 7
#define T 11
#define B 13

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
void show (DIR* dir_ptr,char *dirname,int flag);
Node * Creat(DIR *dir_ptr,char *dirname,int flag);
Node *InsertSort(Node *head);
Node *Reverse(Node *head);
int main(int argc,char *argv[])
{
    char dir_now[4096];
    getcwd(dir_now,sizeof(dir_now));
    int flag=1,oc;
    /*while (argv[t][0]=='-')
    {
        m=1;
        while(argv[t][m]!='\0')
        {
            switch(argv[t][m])
            {
            case 'a':
            {
             flag*=A;
             break;
            }
            case 'u':
            {
             flag*=U;
             break;
            }
            case 'l':
            {
             flag*=L;
            }
            case 't':
            {
             flag *= T;
             break;
            }
            case 'r':
            {
             flag *=B;
             break;
            }
            case 'R':
            {
             flag*=R;
            }
            default:
            {
            printf("%c not fond \n",argv[t][m]);
            break;
            }
            }
            m++;
        }
        t++;
    }
    */
    while((oc=getopt(argc,argv,"aultRr"))!=-1)
    {
        switch(oc)
        {
            case 'a':
            {
            flag*=A;
            break;
            }
            case 'u':
            {
                flag*=U;
                break;
            }
            case 'l':
            {
                flag*=L;
                break;
            }
            case 't':
            {
                flag*=T;
                break;
            }
            case 'R':
            {
                flag*=R;
                break;
            }
            case 'r':
            {
                flag*=B;
                break;
            }
            default:
                puts("无效的输入\n"); 
            break;
        }
    }
    if(argc==1||(argc==2&&((flag%L==0)||(flag%A==0)||(flag%R==0)||(flag%U==0)||(flag%T==0)||(flag%B==0))))
    {
        drive(dir_now,flag);
    }
    else 
    {

        int i=1,j=argc;
        while(i < j && argv[i][0] != '/')
        {
          i++;
        }

        for (i ; i < j; i++)
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
        printf(" ");
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
        if(flag%R==0)
        {
            flag=(flag=flag/R);
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
        perror("opendir error ");
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
            if((flag%A)!=0&&(direntp->d_name[0]=='.'))
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
    if(flag%U==0)
    u=true;
    DIR* dir_ptr2;
    dir_ptr2=opendir(dirname);
    if(flag%T==0 || flag % B ==0)
    {
        Node *head;
        head=Creat(dir_ptr2,dirname,u);
        if (flag % T ==0)
        {
            head=InsertSort(head);
        }
        if(flag % B == 0)
        {
            head=Reverse(head);
        }
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
        if(((flag%A)!=0)&&(direntp->d_name[0]=='.'))
        continue;
        if(flag%U==0)
        Flag=true;
        if(flag%L==0)
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
    printf("\n");
}
void show_sort(Node*head,int flag)
{
    int cont=0;
    Node*p=head,*q;//->next,*q;
    if (head == NULL || head->next == NULL)
    return;
    for(p;p!=NULL;q=p,p=p->next,free(q))
    {
        if((flag&A==0)&&(p->filename[0]=='.'))
        continue;
        if(flag%L==0)
        {
        display_l(p->fullname,flag);
        printf("\t%s\n",p->filename);
        }
        else
        {
            display(p->filename);
            cont++;
            if(cont%5==0)
            printf("\n");
        }
    }
    printf("\n");
}

Node *Creat(DIR *dir_ptr,char *dirname,int flag)
{
    DIR *dir_ptr2;
    char fullname[4096];
    Node *H,*s;
    Node* head=(Node *)malloc(sizeof(Node));
    head->next=H;
    struct dirent *direntp;
    struct stat buf;
    dir_ptr2=opendir(dirname);
    if(dir_ptr2==NULL)
    {
        perror("error");
        return NULL;
    }
    while((direntp=readdir(dir_ptr2))!=NULL)
    {
        s=(Node*)malloc(sizeof(Node));
        if(s==NULL)
        {
            puts("malloc error");
            exit(0);
        }

        strcpy(s->filename,direntp->d_name);
        char Dirname[1024];
        strcpy(Dirname,dirname);
        int l=strlen(dirname);
        Dirname[l]='/';
        Dirname[l+1]='\0';
        strcopy(s->fullname,Dirname,direntp->d_name);
        stat(s->fullname,&buf);
        if(flag==true)
        s->st_time=buf.st_atime;
        else 
        s->st_time=buf.st_ctime;
        if((flag%A!=0)&&(direntp->d_name[0]=='.'))
        {
            free(s);
            continue;
        }
        s->next=H;
        H=s;
    }
    closedir(dir_ptr2);
    return H;
}

Node *InsertSort(Node *head)
{
    if(head == NULL)
    {
        return NULL;
    }
    else if (head  ->next  == NULL)
    {
        return NULL;
    }
    Node *p = head->next, *r, *pre;

    head->next = NULL;
    
    while(p != NULL)
    {
        r=p->next;
        pre=head;
        while (pre->next != NULL && pre->next->st_time > p->st_time)
        {
            pre=pre->next;
        }
        p->next=pre->next;
        pre->next=p;
        p=r;
    }

    return head;
}

Node *Reverse(Node *head)
{
    if(head == NULL || head->next == NULL)
    return head;
    Node *p, *q, *ptmp=NULL;
    p = head;
    q = head->next;
    while (q != NULL) 
    {
        ptmp = q->next;
        q->next = p;
        p = q;
        q = ptmp;
    }
    head->next = NULL;
    head = p;
    return head;
}
