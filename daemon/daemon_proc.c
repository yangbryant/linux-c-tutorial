#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <syslog.h>

int main(void)
{
    pid_t child1, child2, child;

    // daemon process
    if (-1 == (child1 = fork())) {
        syslog(LOG_INFO, "child 1 error.\n");
        exit(1);
    }
    else if (child1 > 0)
    {
        exit(0);
    }


    openlog("daemon_proc_info", LOG_PID, LOG_DAEMON);

    setsid();
    chdir("/");
    umask(0);

    int i;
    for (i = 0; i < getdtablesize(); i++)
    {
        close(i);
    }

    // new process
    if (-1 == (child2 = fork()))
    {
        syslog(LOG_INFO, "child 2 error.\n");
    }
    else if (child2 == 0)
    {
        syslog(LOG_INFO, "child2 will sleep 10s.");
        sleep(10);
        syslog(LOG_INFO, "child2 is going to exit!");
        exit(0);
    }
    else
    {
        waitpid(child2, NULL, 0);
        syslog(LOG_INFO, "child1 noticed that child2 has exited!");
        closelog();

        while(1)
        {
            sleep(10);
        }
    }
}