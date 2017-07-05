#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 2048

int main()
{
    pid_t pid;
    int shmid;

    char *shm_addr;
    char flag[] = "WROTE";
    char *buff;

    if ((shmid = shmget(IPC_PRIVATE, BUFFER_SIZE, 0666)) < 0)
    {
        perror("shmget");
        exit(1);
    }
    else
    {
        printf("Create shared memory : %d\n", shmid);
    }

    system("ipcs -m");

    pid = fork();

    if (pid == -1)
    {
        perror("error\n");
        exit(1);
    }
    else if (pid == 0)
    {
        if ((shm_addr = shmat(shmid, 0, 0)) == (void *) -1)
        {
            perror("Child : shmat\n");
            exit(1);
        }
        else
        {
            printf("Child: Attach shared-memory: %p\n", shm_addr);
        }

        system("ipcs -m");

        while(strncmp(shm_addr, flag, strlen(flag)))
        {
            printf("Child: Wait for enable data...\n");
            sleep(5);
        }

        strcpy(buff, shm_addr + strlen(flag));
        printf("Child: shared-memory: %s\n", buff);

        if ((shmdt(shm_addr)) < 0)
        {
            perror("error shmdt\n");
            exit(1);
        }
        else
        {
            printf("Child: deattach shared-memory\n");
        }
        system("ipcs -m");

        if (shmctl(shmid, IPC_RMID, NULL) == -1)
        {
            perror("Child: shmctl (IPC_RMID)\n");
            exit(1);
        }
        else
        {
            printf("delete shared-memory\n");
        }
        system("ipcs -m");
    }
    else
    {
        if ((shm_addr = shmat(shmid, 0, 0)) == (void *) -1)
        {
            perror("Parent : shmat\n");
            exit(1);
        }
        else
        {
            printf("Parent: Attach shared-memory: %p \n", shm_addr);
        }

        sleep(1);
        printf("Input some String : \n");
        fgets(buff, BUFFER_SIZE, stdin);
        strncpy(shm_addr + strlen(flag), buff, strlen(buff));
        strncpy(shm_addr, flag, strlen(flag));

        if ((shmdt(shm_addr)) < 0)
        {
            perror("Parent : shmdt\n");
            exit(1);
        }
        else
        {
            printf("parent deattach shared-memory\n");
        }
        system("ipcs -m");

        waitpid(pid, NULL, 0);
        printf("Finished\n");
    }
    exit(0);
}