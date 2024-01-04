#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *firstRun(void* arg)
{
    int sum = 0;
    for (int i = 1; i <= 100; i++) 
    {
        sum += i;
    }
    pthread_exit((void*)sum);
}

void *secondRun(void* arg)
{
    int sum = 0;
    for (int i = 101; i <= 200; i++) 
    {
        sum += i;
    }
    pthread_exit((void*)sum);
}

void *thirdRun(void* arg)
{
    int sum = 0;
    for (int i = 201; i <= 300; i++) 
    {
        sum += i;
    }
    pthread_exit((void*)sum);
}

void addSums(int sum_first, int sum_second, int sum_third) 
{
    int totalSum = sum_first + sum_second + sum_third;
    printf("Total Sum: %d\n", totalSum);
}

int main()
{
    pthread_t first, second, third;
    int threadErr;

    int sum_first, sum_second, sum_third;

    if (threadErr = pthread_create(&first, NULL, firstRun, NULL)) 
    {
        printf("Thread Err = %d", threadErr);
    }

    if (threadErr = pthread_create(&second, NULL, secondRun, NULL))  
    {
        printf("Thread Err = %d", threadErr);
    }

    if (threadErr = pthread_create(&third, NULL, thirdRun, NULL))    
    {
        printf("Thread Err = %d", threadErr);
    }

    pthread_join(first, (void**)&sum_first);
    pthread_join(second, (void**)&sum_second);
    pthread_join(third, (void**)&sum_third);

    addSums(sum_first, sum_second, sum_third);

    return 0;
}

