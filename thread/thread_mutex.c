#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_NUMBER 3
#define REPEAT_NUMBER 3

#define DELAY_TIME_LEVELS 10.0
pthread_mutex_t mutex;

void *thrd_func(void *arg)
{
    int thrd_num = (int) arg;
    int delay_time = 0;
    int count = 0;

    int res = pthread_mutex_lock(&mutex);
    if (res)
    {
        printf("Thread %d lock failed.\n", thrd_num);
        pthread_exit(NULL);
    }

    printf("Thread %d is starting\n", thrd_num);
    for (count = 0; count < REPEAT_NUMBER; count++)
    {
        delay_time = (int)(rand() * DELAY_TIME_LEVELS/(RAND_MAX)) + 1;
        sleep(delay_time);
        printf("thread %d: job %d delay = %d\n", thrd_num, count, delay_time);
    }
    printf("Thread %d finished\n", thrd_num);
    pthread_exit(NULL);
}

int main()
{
    pthread_t thread[THREAD_NUMBER];
    int no = 0, res;
    void * thrd_ret;

    srand(time(NULL));

    pthread_mutex_init(&mutex, NULL);

    for (no = 0; no < THREAD_NUMBER; no++)
    {
        res = pthread_create(&thread[no], NULL, thrd_func, (void *)no);
        if (res != 0)
        {
            printf("Create thread %d failed\n", no);
            exit(res);
        }
    }

    printf("Create treads success \nWaiting for threads to finish...\n");
    for (no = 0; no < THREAD_NUMBER; no++)
    {
        res = pthread_join(thread[no], &thrd_ret);
        if (!res)
        {
            printf("Thread %d joined\n", no);
        }
        else
        {
            printf("Thread %d join failed\n", no);
        }
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}