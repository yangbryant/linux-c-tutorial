#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>

#define PORT 4321
#define BUFFER_SIZE 1024
#define MAX_QUE_CONN_NM 5

struct sockaddr_in server_sockaddr, client_sockaddr;
int sockfd, client_fd;
char buf[BUFFER_SIZE];
int sin_size, recvbytes;

void do_work()
{
    while (1)
    {
        sleep(1);
        printf("Im working ...\n");
    }
}

// 异步信号处理函数,处理新的套接字的连接和数据
void accept_async(int sig_num)
{
    sin_size = sizeof(client_sockaddr);
    if ((client_fd = accept(sockfd, (struct sockaddr *)&client_sockaddr, &sin_size)) == -1)
    {
        printf("accept\n");
        exit(1);
    }

    // 调用recv()函数接收客户端的请求
    memset(buf, 0, sizeof(buf));
    if ((recvbytes = recv(client_fd, buf, BUFFER_SIZE, 0)) == -1)
    {
        printf("recv error!\n");
        exit(1);
    }
    printf("Async cronous method: received a message: %s\n", buf);
}

int main()
{
    int flags;

    // 建立 socket 连接
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("socket\n");
        exit(1);
    }
    // 设置 sockaddr_in 结构体中相关参数
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(PORT);
    server_sockaddr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_sockaddr.sin_zero), 8);

    int i = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));

    if (bind(sockfd, (struct sockaddr *)&server_sockaddr, sizeof(struct sockaddr)) == -1)
    {
        printf("bind error!\n");
        exit(1);
    }

    if (listen(sockfd, MAX_QUE_CONN_NM) == -1)
    {
        printf("listen\n");
        exit(1);
    }

    // 设置异步方式
    signal(SIGIO, accept_async); // SIGIO信号处理函数的注册
    fcntl(sockfd, F_SETOWN, getpid()); // 使套接字属于该进程
    flags = fcntl(sockfd, F_GETFL); //获得套接字的状态标志位
    // 设置成异步访问模式
    if (flags < 0 || fcntl(sockfd, F_SETFL, flags | O_ASYNC) < 0)
    {
        printf("fcntl\n");
    }

    // 继续完成自己的工作, 不再需要等待了
    do_work();

    close(sockfd);
    exit(0);
}
