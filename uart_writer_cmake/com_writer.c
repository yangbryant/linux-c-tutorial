#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

#include "set_com_config/set_com_config.h"

#define BUFFER_SIZE 1024
#define HOST_COM_PORT 0

int main(void)
{
    int fd;
    char buff[BUFFER_SIZE];

    if ((fd = open_port(HOST_COM_PORT)) < 0)
    {
        printf("Open port error.\n");
        return 1;
    }

    if (set_com_config(fd, 115200, 8, 'N', 1) < 0)
    {
        printf("set com config error.\n");
        return 1;
    }

    do {
        memset(buff, 0, BUFFER_SIZE);
        if (fgets(buff, BUFFER_SIZE, stdin) == NULL)
        {
            printf("fgets error.\n");
            break;
        }
        write(fd, buff, strlen(buff));
    }while(strncmp(buff, "quit", 4));

    close(fd);
    return 0;
}