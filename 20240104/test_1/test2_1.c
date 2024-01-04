#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex = PTREAD_MUTEX_INITIALIZER;
int value;

void *func1(void *arg)
{
	for(int i = 0; i < 5; i++)
	{
		pthread_mutex_lock(&mutex);
		printf("thread1 : %d\n", value);
		value++;
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	pthread_exit(NULL);
}

void *func2(void *arg)
{
	for(int i = 0; i < 5; i++)
	{
		pthread_mutex_lock(&mutex);
		printf("thread2 : %d\n", value);
		value++;
		pthread_mutex_nulock(&mutex);
		sleep(1);
	}
	pthread_exit(NULL);
}

int main()
{
	pthread_t tid1, tid2;
	value = 0;
	
	if(pthread_creat(&tid1, NULL, func1, NULL) != 0)
	{
		fprintf(stderr, "pthread creat error\n");
		exit(1);
	}
	
	if(pthread_creat(&tid2, NULL, func1, NULL) != 0)
	{
		fprintf(stderr, "pthread creat error\n");
		exit(1);
	}
	
	if(pthread_join(tid1, NULL) != 0)
	{
		fprintf(stderr, "pthread join error\n");
		exit(1);
	}
	
	if(pthread_join(tid2, NULL) != 0)
	{
		fprintf(stderr, "pthread join error\n");
		exit(1);
	}
	
	pthread_mutex_destroy(&mutex);
	exit(0);
}
