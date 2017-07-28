#include "multicast_header.h"

int main()
{
    int sockfd;
    struct sockaddr_in peeraddr, mcast;
    char buff[BUFF_SIZE];
    int nbytes;
    socklen_t addrlen = sizeof(struct sockaddr);
    struct ip_mreqn mcaddr;

    // 打开用户数据报套接字 UDP Socket
    if (-1 == (sockfd = socket(AF_INET, SOCK_DGRAM, 0)))
    {
        printf("error socket!\n");
        exit(1);
    }

    // 填充地址结构体
    mcast.sin_family = AF_INET;
    mcast.sin_port = htons(MCAST_PORT);
    mcast.sin_addr.s_addr = inet_addr(MCAST_ADDR);

    // 接收方绑定广播地址
    if (-1 == bind(sockfd, (struct sockaddr *) &mcast, sizeof(mcast)))
    {
        printf("bind error!\n");
        exit(1);
    }

    // 填充组播组结构体, 用作setsockopt()函数的参数
    mcaddr.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR);
    // 如果本机只有一个网络接口,则置为INADDR_ANY即可,让系统自动选择
    mcaddr.imr_address.s_addr = INADDR_ANY;
    mcaddr.imr_ifindex = 0;

    // 此处的参数在Linux系统下可以查询man手册"man 7 ip"
    if (-1 == setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mcaddr, sizeof(mcaddr)))
    {
        printf("setsockopt error!\n");
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