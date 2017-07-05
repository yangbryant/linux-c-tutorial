#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

// MacOS本身有回收机制,不能验证,请到Linux下验证.

void signchld_handler(int signo)
{
    wait(NULL);
    puts("SIGCHLD Received!!!!");
}

int main()
{
    pid_t pid;

    if (SIG_ERR == signal(SIGCHLD, signchld_handler)) {
        printf("signnal\n");
        return -1;
    }

    if (-1 == (pid = fork())) {
        printf("fork\n");
        return -1;
    }

    if (0 == pid) {
        sleep(5);
        exit(EXIT_SUCCESS);
    }
    else {
        while(1)
        {
            puts("I am your father.\n");
            sleep(1);
        }
    }
    return 0;
}