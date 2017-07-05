#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    pid_t pid;
    int result = fork();

    if (result == -1)
    {
        printf("fork Error!!!\n");
    }
    else if (result == 0)
    {
        printf("child process %d, pid = %d.\n", result, getpid());
    }
    else 
    {
        printf("father process %d, pid = %d.\n", result, getpid());
    }
}