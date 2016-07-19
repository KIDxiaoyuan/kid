/*************************************************************************
	> File Name: my_ls.c
	> Author: 
	> Mail: 
	> Created Time: Wed 20 Jul 2016 14:42:08 GMT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<pwd.h>
#include<errno.h>
#include<dirent.h>
int get_num(const char * path);//获取文件数量函数
int show_file(const char *path);//显示文件列表
int my_stat(const char * file_name);//带属性的显示文件信息
void get_st_mode(unsigned num,int *ans);//判断文件权限
int main(int argc,char **argv)
{
 int num;
 pus(argv[0]);
 num=get_num;
 printf("%d\n",num);
 show_file(argv[0]);
 my_stat(argv[0]);
 return 0;
}
void get_st_mode(unsigned num,int *ans)
{
    //r==2,w==3,x==5//
    ans[1]=ans[2]=ans[0]=1;//初始化,后判断
    //判断文件权限//
    if(num&S_IRUSR==S_IRUSR)
    ans[0]*=2；
    if(num&S_IWUSR==S_IWUSR)
    ans[0]*=3;
    if(num&S_IXUSR==S_IXUSR)
    ans[0]*=5;
    if(num&S_IRGRP==S_IRGRP)
    ans[1]*=2;
    if(num&S_IWGRP==S_IWGRP)
    ans[1]*=3;
    if(num&S_IXGRP==S_IXGRP)
    ans[1]*=5;
    if(num&S_IROTH==S_IROTH)
    ans[2]*=2;
    if(num&S_IWOTH==S_IWOTH)
    ans[2]*=3;
    if(num&S_IXOTH==S_IXOTH)
    ans[2]*=5;
    //判断文件类型//
        if(S_ISLNK(num)) ans[3]=1;
    else if(S_ISREG(num)) ans[3]=2;
    else if(S_ISDIR(num)) ans[3]=3;
    else if(S_ISCHR(num)) ans[3]=4;
    else if(S_ISBLK(num)) ans[3]=5;
    else if(S_ISFIFO(num))ans[3]=6;
    else if(S_ISSOCK(num))ans[3]=7;
}
int my_stat(const char *file_name)
{
    struct stat buf;
    int mode[4],i,j;
    struct passwd *psd;
    struct group  *grp;
    if(stat(file_name,buf)==-1)
    {
        perror("stat:");
        return -1;
    }
    get_st_mode(buf.st_mode,mode);
    switch(mode[3])
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
    if(mode[i]%2==0) printf("r");
    else  printf("-");
    if(mode[i]%3==0) printf("w");
    else printf("-");
    if(mode[i]%5==0) printf("x");
    else printf("-");

    }
    printf("\t");
  psd=getpwuid(buf.st_uid);
  grp=getgrgid(buf.st_gid);
  printf("%4d",buf.st_nlink);
  printf("%-8s",psd->pw_name);
  printf("%-8s %6d",grp->gr_name,buf.st_size);
  strcpy(buf_time,ctime(&buf.st_mtime));
  buf_time[strlen(buf_time)-1]='\0';
  puts(buf_time);
}

int show_file(const char *path)
{
    DIR *dir;
    int count=0;
    struct dirent *ptr;
    if((dir=opendir(path))==NULL)
    return -1;
    while((ptr=readdir(dir))!=NULL)
    {
    printf("%s\t",ptr->d_name);
    count++;
    if(count%5==0)
    printf("\n");
    }
    closedir(dir);
    return 0;
}

int get_num(const char * path)
{
    DIR*dir;
    int count=0;
    if((dir=opendir(path))==NULL)
    return -1;
    while((readdir(dir))!=NULL)
    count++；
    closedir(dir);
    return count;
}

