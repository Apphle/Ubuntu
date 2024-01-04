#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int sumRange(int start, int end)
{
    int sum = 0;
    for (int i = start; i <= end; i++)
    {
        sum += i;
    }
    return sum;
}

void *calculateAndPrintSum(void *arg)
{
    int *range = (int *)arg;
    int start = range[0];
    int end = range[1];

    pthread_mutex_lock(&mutex);
    int result = sumRange(start, end);
    printf("%d에서 %d까지의 합: %d\n", start, end, result);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main()
{
    pthread_t tid[4];
    int ranges[4][2] = {{1, 100}, {101, 200}, {201, 300}, {1, 300}};

    for (int i = 0; i < 4; i++)
    {
        if (pthread_create(&tid[i], NULL, calculateAndPrintSum, ranges[i]) != 0)
        {
            perror("스레드 생성 오류");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (pthread_join(tid[i], NULL) != 0)
        {
            perror("스레드 조인 오류");
            exit(EXIT_FAILURE);
        }
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}

