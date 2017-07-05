#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main()
{
    pid_t pid;
    int ret;

    if ((pid = fork()) < 0)
    {
        printf("error.\n");
        exit(1);
    }
    else if (pid == 0)
    {
        printf("Child (pid = %d) is waiting for any signal.\n", getpid());
        raise(SIGSTOP);
        exit(0);
    }
    else {
        if ((waitpid(pid, NULL, WNOHANG)) == 0)
        {
            if ((ret = kill(pid, SIGKILL)) == 0)
            {
                printf("Parent kill %d\n", pid);
            }
        }
        waitpid(pid, NULL, 0);
        exit(0);
    }
}