#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <poll.h>

#define MAX_BUFFER_SIZE 1024
#define IN_FILES 3
#define TIME_DELAY 60000
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int main(void)
{
    struct pollfd fds[IN_FILES];
    char buf[MAX_BUFFER_SIZE];
    int i, res, real_read, maxfd;

    fds[0].fd = 0;

    if ((fds[1].fd = open("in1", O_RDONLY | O_NONBLOCK)) < 0)
    {
        printf("Open in1 error.\n");
        return 1;
    }

    if ((fds[2].fd = open("in2", O_RDONLY | O_NONBLOCK)) < 0)
    {
        printf("Open in2 error.\n");
        return 1;
    }

    for (i = 0; i < IN_FILES; i++)
    {
        fds[i].events = POLLIN;
    }

    while (fds[0].events || fds[1].events || fds[2].events)
    {
        if (poll(fds, IN_FILES, 0) < 0)
        {
            printf("Poll error or Time out.\n");
            return 1;
        }

        for (i = 0; i < IN_FILES; i++)
        {
            if (fds[i].revents)
            {
                memset(buf, 0, MAX_BUFFER_SIZE);
                real_read = read(fds[i].fd, buf, MAX_BUFFER_SIZE);
                if (real_read < 0)
                {
                    if (errno != EAGAIN)
                    {
                        return 1;
                    }
                }
                else if (!real_read)
                {
                    close(fds[i].fd);
                    fds[i].events = 0;
                }
                else {
                    if (i == 0)
                    {
                        if ((buf[0] == 'q') || (buf[0] == 'Q'))
                        {
                            return 1;
                        }
                    }
                    else {
                        buf[real_read] = '\0';
                        printf("%s\n", buf);
                    }
                }
            }
        }
    }
    exit(0);
}
