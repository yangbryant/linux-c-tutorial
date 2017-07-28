#ifndef __MULTICAST_HEADER_H__
#define __MULTICAST_HEADER_H__

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MCAST_PORT 60001
#define MCAST_ADDR "239.1.2.3"

#define BUFF_SIZE 1024

#endif /* __MULTICAST_HEADER_H__ */