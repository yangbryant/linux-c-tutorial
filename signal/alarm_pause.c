#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int ret = alarm(5);
    pause();
    printf("wake up, %d.\n", ret);
}