#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int ret;
    if (fork() == 0)
    {
        if ((ret = execlp("ps", "ps", "-ef", NULL)) < 0)
        {
            printf("Execlp error!\n");
        }
    }
}