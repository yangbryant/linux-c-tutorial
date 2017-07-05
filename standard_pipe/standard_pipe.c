#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define BUFSIZE 1024

int main()
{
    FILE *fp;
    char *cmd = "ps -ef";
    char buf[BUFSIZE];

    if ((fp = popen(cmd, "r")) == NULL)
    {
        printf("Popen error.\n");
        exit(1);
    }

    memset(buf, 0, sizeof(buf));
    while ((fgets(buf, BUFSIZE, fp)) != NULL)
    {
        printf("%s", buf);
    }
    pclose(fp);
    exit(0);
}