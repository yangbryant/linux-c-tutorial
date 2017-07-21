#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXNAMELEN 256

int main()
{
    struct addrinfo hints, *res = NULL;

    char host_name[MAXNAMELEN], addr_str[INET_ADDRSTRLEN], *addr_str1;
    int rc;
    struct in_addr addr;

    memset(&hints, 0, sizeof(hints));

    hints.ai_flags = AI_CANONNAME;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    if ((gethostname(host_name, MAXNAMELEN)) == -1)
    {
        printf("gethostname\n");
        exit(1);
    }

    rc = getaddrinfo(host_name, NULL, &hints, &res);
    if (rc != 0)
    {
        printf("getaddrinfo\n");
        exit(1);
    }
    else
    {
        addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr;
        inet_ntop(res->ai_family, &(addr.s_addr), addr_str, INET_ADDRSTRLEN);
        printf("host_name: %s\nIp address: %s\n", res->ai_canonname, addr_str);
    }
    exit(0);
}