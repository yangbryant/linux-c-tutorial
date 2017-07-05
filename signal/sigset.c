#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void my_func(int signum)
{
    printf("if you want to quit, please try sigquit!\n");
}

int main()
{
    sigset_t set, pendset;
    struct sigaction action1, action2;

    if (sigemptyset(&set) < 0)
    {
        printf("error!\n");
        exit(1);
    }
    if (sigaddset(&set, SIGQUIT) < 0)
    {
        printf("sigaddset\n");
        exit(1);
    }
    if (sigaddset(&set, SIGINT) < 0)
    {
        printf("sigaddset\n");
        exit(1);
    }
    if (sigismember(&set, SIGINT))
    {
        sigemptyset(&action1.sa_mask);
        action1.sa_handler = my_func;
        action1.sa_flags = 0;
        sigaction(SIGINT, &action1, NULL);
    }
    if (sigismember(&set, SIGQUIT))
    {
        sigemptyset(&action2.sa_mask);
        action2.sa_handler = my_func;
        action2.sa_flags = 0;
        sigaction(SIGQUIT, &action2, NULL);
    }
    if (sigprocmask(SIG_BLOCK, &set, NULL) < 0)
    {
        printf("sigprocmask\n");
        exit(1);
    }
    else
    {
        printf("Signal set was blocked, please any key.\n");
        getchar();
    }
    if (sigprocmask(SIG_UNBLOCK, &set, NULL) < 0)
    {
        printf("sigprocmask\n");
        exit(1);
    }
    else
    {
        printf("Singal set is in unblock state.\n");
    }
    while(1);
    exit(0);
}