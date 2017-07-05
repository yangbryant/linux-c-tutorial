#include "shm_com.h"
#include "../sem_ipc/sem_com.h"

#include <signal.h>

int ignore_signal(void)
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGSTOP, SIG_IGN);
    return 0;
}

int main()
{
    void *shared_memory = NULL;
    struct shm_buff *shm_buff_inst;
    char buffer[BUFSIZ];
    int shmid, semid;

    ignore_signal();
    semid = semget(ftok(".", 'a'), 1, 0666 | IPC_CREAT);
    init_sem(semid, 0);

    shmid = shmget(ftok(".", 'b'), sizeof(struct shm_buff), 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        perror("shmget failed");
        del_sem(semid);
        exit(1);
    }
    shared_memory = shmat(shmid, (void *)0, 0);
    if (shared_memory == (void *) -1)
    {
        perror("shmat");
        del_sem(semid);
        exit(1);
    }

    printf("Memory attached at %x\n", (int)shared_memory);

    shm_buff_inst = (struct shm_buff *) shared_memory;
    do
    {
        sem_p(semid);
        printf("Enter some text to the shared Memory (enter 'quit' to exit):");
        if (fgets(shm_buff_inst->buffer, SHM_BUFF_SZ, stdin) == NULL)
        {
            perror("fgets");
            sem_v(semid);
            break;
        }
        shm_buff_inst->pid = getpid();
        sem_v(semid);
    } while(strncmp(shm_buff_inst->buffer, "quit", 4) != 0);

    del_sem(semid);
    if (shmdt(shared_memory) == 1)
    {
        perror("shmdt");
        exit(1);
    }
    exit(0);
}