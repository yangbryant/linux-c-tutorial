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

#define MAX_SOCK_FD FD_SETSIZE

int main()
{
    struct sockaddr_in server_sockaddr, client_sockaddr;
    int sin_size, count;
    fd_set inset, tmp_inset;
    int sockfd, client_fd, fd;
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

    // 将调用socket()函数的描述符作为文件描述符
    FD_ZERO(&inset);
    FD_SET(sockfd, &inset);
    while (1)
    {
        // 文件描述符集合的备份,这样可以避免每次都进行初始化
        tmp_inset = inset;
        sin_size = sizeof(struct sockaddr_in);
        memset(buf, 0, sizeof(buf));

        // 调用SELECT()函数
        if (!(select(MAX_SOCK_FD, &tmp_inset, NULL, NULL, NULL) > 0))
        {
            printf("select error!\n");
        }

        for (fd = 0; fd < MAX_SOCK_FD; fd++)
        {
            if (FD_ISSET(fd, &tmp_inset) > 0)
            {
                if (fd == sockfd)
                {
                    // 服务器端接收客户端的连接请求
                    if ((client_fd = accept(sockfd, (struct sockaddr *) &client_sockaddr, &sin_size)) == -1)
                    {
                        printf("error accept!\n");
                        exit(1);
                    }
                    // 将新连接的客户端套接字加入观察列表中
                    FD_SET(client_fd, &inset);
                    printf("New connection from %d(socket)\n", client_fd);
                }
                else {
                    // 处理客户端发来的消息
                    if ((count = recv(client_fd, buf, BUFFER_SIZE, 0)) > 0)
                    {
                        printf("Received a message from %d: <%s>\n", client_fd, buf);
                    }
                    else {
                        close(fd);
                        FD_CLR(fd, &inset);
                        printf("CLient %d(socket) has left\n", fd);
                    }
                }
            }
        }
    }
    close(sockfd);
    exit(0);
}

