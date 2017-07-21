#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 4321
#define BUFFER_SIZE 1024
#define MAX_QUE_CONN_NM 5

int main()
{
    struct sockaddr_in server_sockaddr, client_sockaddr;
    int sin_size, recvbytes;
    int sockfd, client_fd;
    char buf[BUFFER_SIZE];

    // 建立socket连接
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("socket\n");
        exit(1);
    }
    printf("Socket id = %d\n", sockfd);

    // 设置sockaddr_in参数
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(PORT);
    server_sockaddr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_sockaddr.sin_zero), 8);

    int i = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));

    // 绑定函数bind()
    if (bind(sockfd, (struct sockaddr *)&server_sockaddr, sizeof(struct sockaddr)) == -1)
    {
        printf("bind\n");
        exit(1);
    }

    printf("Bind Success!\n");

    // 调用listen()函数,创建未处理请求的队列
    if (listen(sockfd, MAX_QUE_CONN_NM) == -1)
    {
        printf("listen\n");
        exit(1);
    }
    printf("Listening...\n");

    // 调用accept()函数,等待客户端的连接
    sin_size = sizeof(client_sockaddr);
    if ((client_fd = accept(sockfd, (struct sockaddr *) &client_sockaddr, &sin_size)) == -1)
    {
        printf("accept\n");
        exit(1);
    }

    // 调用recv()函数,接收客户端的请求
    memset(buf, 0, sizeof(buf));
    if ((recvbytes = recv(client_fd, buf, BUFFER_SIZE, 0)) == -1)
    {
        printf("recv\n");
        exit(1);
    }
    printf("Received a message: <%s>.\n", buf);
    close(sockfd);
    exit(0);
}

