/*************************************************************************
	> File Name: tongji.c
	> Author: 
	> Mail: 
	> Created Time: Wed 15 Jun 2016 22:26:51 GMT
 ************************************************************************/

#include<stdio.h>
#include<string.h>
int main()
{
    int n;
    scanf("%d",&n);
    char st[10][100],c;
    int d,x,w,s,q,i,l,k,j,word=0;
    for(j=0;j<n;j++)
    gets(st[j]);
    w=k=d=x=s=q=0;
    for(j=0;j<n;j++)
    {
    l=strlen(st[j]);
    for(i = 0; st[j][i]; i ++)
    {
        if(st[j][i]>='A' && st[j][i]<='Z')d++;
        else if(st[j][i]>='a' && st[j][i]<='z')x++;
        else if(st[j][i]>='0' && st[j][i]<='9')s++;
        else if(st[j][i]==' ') k++;
        else q++;
    }
    for(i=0;(c=st[j][i])!='\0';i++)
    {
    if
    while()
    if(c==' ')
    word=0;
    else if(word==0)
    {
    word=1;
    w++;
    }
    }

        printf("%d %d %d %d %d %d %d\n",l,w,d,x,s,k,q);

    }
return 0;
}
/*
 for(i=1;i<=n;i++)
    {
        gets(a);
        getchar();
        while(a[j]!='\0')
        {
            if(a[j]<='z'&&a[j]>='a'||a[j]<='Z'&&a[j]>='A')
                if(a[j+1]==' ' || a[j+1]=='\0')            //这里
                    k++;
            j++;
        }
        //k++;         //这里
        printf("%d\n",k);
        k=0;
        j=0;
    }
    */
