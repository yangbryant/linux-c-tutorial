#include "broadcast_header.h"

int main()
{
    int sockfd;
    struct sockaddr_in bcastaddr;
    char buff[BUFF_SIZE];
    int f_isbcast;
    int i;

    // 打开用户数据报套接字 UDP Socket
    if (-1 == (sockfd = socket(AF_INET, SOCK_DGRAM, 0)))
    {
        printf("error socket!\n");
        exit(1);
    }

    // 开启广播属性
    f_isbcast = 1;
    if (-1 == setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &f_isbcast, sizeof(f_isbcast)))
    {
        printf("setsockopt error!\n");
        exit(1);
    }

    // 填充地址结构体
    bcastaddr.sin_family = AF_INET;
    bcastaddr.sin_port = htons(BCAST_PORT);
    bcastaddr.sin_addr.s_addr = inet_addr(BCAST_ADDR);

    // 一秒一次循环发送广播
    i = 0;
    while (1)
    {
        sprintf(buff, "this is a broadcast message! --- %d", i++);
        printf("sendto: <%s>\n", buff);
        if (-1 == sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&bcastaddr, sizeof(bcastaddr)))
        {
            printf("sendto error!\n");
            exit(1);
        }
        sleep(1);
    }

    close(sockfd);
    return 0;
}