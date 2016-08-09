/*************************************************************************
	> File Name: tsd.c
	> Author: 
	> Mail: 
	> Created Time: Mon 08 Aug 2016 10:00:30 GMT
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<pthread.h>

pthread_key_t key;

int tsd = 1;

void *thread2(void *arg)
{
    tsd = 4;
    printf("thread2 %ld is running \n",(long int)pthread_self());
    pthread_setspecific(key, (void *)(tsd));
    printf("thread2 %ld returns %ld \n",(long int)pthread_self(),(long int)pthread_getspecific(key));
    tsd =2;
    pthread_setspecific(key,(void*)tsd);
    printf("new tsd is %d\n",pthread_getspecific(key));
}
void *thread1(void *arg)
{
    //int tsd = 10;
    pthread_t thid2;
    printf("thread1 %ld is running\n",(long int)pthread_self());
    pthread_setspecific(key, (void *)tsd);
    printf("thread1 %d\n",pthread_getspecific(key));
    tsd =3;
    pthread_create(&thid2,NULL,thread2,NULL);
    sleep(2);
    pthread_setspecific(key,(void *)tsd);
    printf("thread1 %ld,returns %ld\n",(long int)pthread_self(),(long int)pthread_getspecific(key));
}
int main (void)
{
    pthread_t thid1;
    printf("main thread begins\n");
    pthread_key_create(&key,NULL);
    pthread_create(&thid1,NULL,thread1,NULL);
    sleep(9);
    pthread_key_delete(key);
    printf("main thread exit\n");
    return 0;
}
