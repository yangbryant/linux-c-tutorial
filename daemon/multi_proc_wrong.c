#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t child1, child2, child;

    child1 = fork();

    if (child1 == -1)
    {
        printf("child 1 fork error.\n");
        exit(1);
    }
    else if (child1 == 0)
    {
        printf("child1 : exec 'ls -l'.\n");
        if (execlp("ls", "ls", "-l", NULL) < 0)
        {
            printf("child 1 ececlp error.\n");
        }
    }

    child2 = fork();
    if (child2 == -1)
    {
        printf("child 2 fork error.\n");
        exit(1);
    }
    else if (child2 == 0)
    {
        printf("child 2 sleep 5s.\n");
        sleep(5);
        exit(0);
    }
    else {
        printf("father process:\n");
        child = waitpid(child1, NULL, 0);
        if (child == child1)
        {
            printf("get child1 exit code\n");
        }
        else {
            printf("error occured!\n");
        }

        do {
            child = waitpid(child2, NULL, WNOHANG);
            if (child == 0)
            {
                printf("child 2 process not exit.\n");
                sleep(1);
            }
        } while (child == 0);

        if (child == child2) {
            printf("exit code.\n");
        }
        else {
            printf("error occured.\n");
        }
    }
    exit(0);
}