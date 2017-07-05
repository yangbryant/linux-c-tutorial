#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "lock_set.h"

#define MAX_FILE_SIZE 100 * 1024 * 1024

const char *fifo_file = "../fifo_producer/mfifo";
const char *tmp_file = "../fifo_producer/tmp";

int customing(const char *mfifo, int need)
{
    int fd;
    char buff;
    int counter = 0;

    if ((fd = open(mfifo, O_RDONLY)) < 0)
    {
        printf("Open fifo error.\n");
        return -1;
    }

    printf("Enjoy:");
    lseek(fd, SEEK_SET, 0);
    while (counter < need) {
        while ((read(fd, &buff, 1) == 1) && (counter < need))
        {
            fputc(buff, stdout);
            counter++;
        }
    }

    fputs("\n", stdout);
    close(fd);
    return 0;
}

int mfilecopy(const char *sour_file, const char *dest_file, int offset, int count, int copy_mode)
{
    int in_file, out_file;
    int counter = 0;

    char buff_unit;

    if ((in_file = open(sour_file, O_RDONLY | O_NONBLOCK)) < 0)
    {
        printf("mfilecopy error in source file.\n");
        return -1;
    }

    if ((out_file = open(dest_file, O_CREAT | O_RDWR | O_TRUNC | O_NONBLOCK, 0644)) < 0)
    {
        printf("mfilecopy error in destination file.\n");
        return -1;
    }

    lseek(in_file, offset, SEEK_SET);
    while((read(in_file, &buff_unit,  1) == 1) && (counter < count))
    {
        write(out_file, &buff_unit, 1);
        counter++;
    }

    close(in_file);
    close(out_file);
    return 0;
}

int custom(int need)
{
    int fd;
    customing(fifo_file, need);

    if ((fd = open(fifo_file, O_RDWR)) < 0)
    {
        printf("error. source.\n");
        return -1;
    }

    lock_set(fd, F_WRLCK);
    mfilecopy(fifo_file, tmp_file, need, MAX_FILE_SIZE, 0);
    mfilecopy(tmp_file, fifo_file, 0, MAX_FILE_SIZE, 0);
    lock_set(fd, F_UNLCK);
    unlink(tmp_file);
    close(fd);
    return 0;
}

int main(int argc, char *argv[])
{
    int customer_capacity = 10;

    if (argc > 1)
    {
        sscanf(argv[1], "%d", &customer_capacity);
    }

    if (customer_capacity > 0)
    {
        custom(customer_capacity);
    }

    exit(EXIT_SUCCESS);
}
