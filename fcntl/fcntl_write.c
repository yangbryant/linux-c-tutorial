#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include "lock_set.h"

int main() {
    int fd = open("hello", O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        printf("Open file error!\n");
        exit(1);
    }

    lock_set(fd, F_WRLCK);
    getchar();
    lock_set(fd, F_UNLCK);
    getchar();
    close(fd);
    exit(0);
}