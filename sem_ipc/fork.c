#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "sem_com.h"

#define DELAY_TIME 3
int main(void)
{
    pid_t result;
    int sem_id;

    sem_id = semget(ftok(".", 'a'), 1, 0666 | IPC_CREAT);
    init_sem(sem_id, 0);

    result = fork();

    if (result == -1)
    {
        printf("Fork\n");
    }
    else if (result == 0)
    {
        printf("Child process \n");
        sleep(DELAY_TIME);
        printf("the returned value is child (%d) = %d\n", getpid(), result);
        sem_v(sem_id);
    }
    else
    {
        sem_p(sem_id);
        printf("the returned value is father (%d) = %d\n", getpid(), result);
        sem_v(sem_id);
        del_sem(sem_id);
    }
    exit(0);
}