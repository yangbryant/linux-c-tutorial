#ifndef __BROADCAST_HEADER_H__
#define __BROADCAST_HEADER_H__

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BCAST_PORT 50001
#define BCAST_ADDR "10.10.61.255"

#define BUFF_SIZE 1024

#endif /* __BROADCAST_HEADER_H__ */