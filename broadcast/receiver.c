#include "broadcast_header.h"

int main()
{
    int sockfd;
    struct sockaddr_in peeraddr, bcast;
    char buff[BUFF_SIZE];
    int nbytes;
    socklen_t addrlen = sizeof(struct sockaddr);

    // 打开用户数据报套接字 UDP Socket
    if (-1 == (sockfd = socket(AF_INET, SOCK_DGRAM, 0)))
    {
        printf("error socket!\n");
        exit(1);
    }

    // 填充地址结构体
    bcast.sin_family = AF_INET;
    bcast.sin_port = htons(BCAST_PORT);
    bcast.sin_addr.s_addr = inet_addr(BCAST_ADDR);

    // 接收方绑定广播地址
    if (-1 == bind(sockfd, (struct sockaddr *) &bcast, sizeof(bcast)))
    {
        printf("bind error!\n");
        exit(1);
    }

    // 循环接收并打印
    while (1)
    {
        memset(buff, 0, BUFF_SIZE);
        if (0 >= (nbytes = recvfrom(sockfd, buff, BUFF_SIZE, 0, (struct sockaddr *)&peeraddr, &addrlen)))
        {
            printf("recvfrom error!\n");
            exit(1);
        }
        printf("[%d]<%s>\n", nbytes, buff);
    }

    close(sockfd);
    return 0;
}