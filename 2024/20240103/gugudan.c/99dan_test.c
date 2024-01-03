#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
 
// 쓰레드 동작시 실행될 함수
void *firstRun(void* arg)
{
    printf("\n");
    sleep(1);
    for(int i = 1; i <= 9; i++)
    {
		printf("%d * %d = %d\n", 1, i, 1 * i);
		sleep(1);
	}    
	pthread_exit(NULL);
}

void *secondRun(void* arg)
{
    printf("\n");
    sleep(10);
    for(int i = 1; i <= 9; i++)
    {
		printf("%d * %d = %d\n", 2, i, 2 * i);
		sleep(1);
	}    
	pthread_exit(NULL);
}

void *thirdRun(void* arg)
{
    printf("\n");
    sleep(20);
    for(int i = 1; i <= 9; i++)
    {
		printf("%d * %d = %d\n", 3, i, 3 * i);
		sleep(1);
	}    
	pthread_exit(NULL);
}
 
int main()
{
    pthread_t first, second, third;
    int threadErr;
    
    
    // 쓰레드를 만들고 쓰레드 함수 실행
    if(threadErr = pthread_create(&first,NULL,firstRun,NULL))
    {
        // 에러시 에러 출력
        printf("Thread Err = %d",threadErr);
    }
    
    if(threadErr = pthread_create(&second,NULL,secondRun,NULL))
    {
        // 에러시 에러 출력
        printf("Thread Err = %d",threadErr);
    }
    
    if(threadErr = pthread_create(&third,NULL,thirdRun,NULL))
    {
        // 에러시 에러 출력
        printf("Thread Err = %d",threadErr);
    }
    
    //while(1);
    pthread_join(first, NULL);
    pthread_join(second, NULL);
    pthread_join(third, NULL);
}
