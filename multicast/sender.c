#include "multicast_header.h"

int main()
{
    int sockfd;
    struct sockaddr_in mcastaddr;
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

    // 填充目标组播地址结构体
    mcastaddr.sin_family = AF_INET;
    mcastaddr.sin_port = htons(MCAST_PORT);
    mcastaddr.sin_addr.s_addr = inet_addr(MCAST_ADDR);

    // 一秒一次循环发送广播
    i = 0;
    while (1)
    {
        sprintf(buff, "this is a multicast message! --- %d", i++);
        if (-1 == sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&mcastaddr, sizeof(mcastaddr)))
        {
            printf("sendto error!\n");
            exit(1);
        }
        puts(buff);
        sleep(1);
    }

    close(sockfd);
    return 0;
}