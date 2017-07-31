#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/time.h>

#include<sys/stat.h>
#include<fcntl.h>

#define NTP_PORT 123
#define TIME_PORT 37
// #define NTP_SERVER_IP "210.72.145.44"
#define NTP_SERVER_IP "17.253.84.253"
#define NTP_PORT_STR "123"

#define NTPV1 "NTP/V1"
#define NTPV2 "NTP/V2"
#define NTPV3 "NTP/V3"
#define NTPV4 "NTP/V4"
#define TIME "TIME/UDP"

#define NTP_PCK_LEN 48
#define LI 0
#define VN 3
#define MODE 3
#define STRATUM 0
#define POLL 4
#define PREC -6

#define JAN_1970 0x83aa7e80 // 从1900年~1970年时间的秒数
#define NTPFRAC(x) (4294 * (x) + ((1980 * (x)) >> 11))
#define USEC(x) (((x) >> 12) - 759 * ((((x) >> 10) + 32768) >> 16))

typedef struct _ntp_time
{
    unsigned int coarse;
    unsigned int fine;
} ntp_time;

struct ntp_packet
{
    unsigned char leap_ver_mode;
    unsigned char startum;
    char poll;
    char precision;
    int root_delay;
    int root_dispersion;
    int reference_identifier;
    ntp_time reference_timestamp;
    ntp_time originage_timestamp;
    ntp_time receive_timestamp;
    ntp_time transmit_timestamp;
};

char protocol[32];

// 构建NTP协议包
int construct_packet(char *packet)
{
    char version = 1;
    uint32_t tmp_wrd;
    int port;
    time_t timer;

    strcpy(protocol, NTPV3);

    // 判断协议版本
    if (!strcmp(protocol, NTPV1) || !strcmp(protocol, NTPV2) || !strcmp(protocol, NTPV3) || !strcmp(protocol, NTPV4))
    {
        memset(packet, 0, NTP_PCK_LEN);
        port = NTP_PORT;

        // 设置16字节包头
        version = protocol[5] - 0x30;
        tmp_wrd = htonl((LI << 30) | (version << 27) | (MODE << 24) | (STRATUM << 16) | (POLL << 8) | (PREC & 0xff));
        memcpy(packet, &tmp_wrd, sizeof(tmp_wrd));

        // 设置Root Delay, Root Dispersion, Reference Indentifier
        tmp_wrd = htonl(1 << 16);
        memcpy(&packet[4], &tmp_wrd, sizeof(tmp_wrd));
        memcpy(&packet[8], &tmp_wrd, sizeof(tmp_wrd));

        // 设置Timestamp部分
        time(&timer);
        // 设置Transmit Timestamp Coarse
        tmp_wrd = htonl(JAN_1970 + (long)timer);
        printf("%d\n", tmp_wrd);
        memcpy(&packet[40], &tmp_wrd, sizeof(tmp_wrd));

        tmp_wrd = htonl((long)NTPFRAC(timer));
        memcpy(&packet[44], &tmp_wrd, sizeof(tmp_wrd));

        return NTP_PCK_LEN;
    }
    else if (!strcmp(protocol, TIME))
    {
        port = TIME_PORT;
        memset(packet, 0, 4);
        return 4;
    }
    return 0;
}

int get_ntp_time(int sk, struct addrinfo *addr, struct ntp_packet *ret_time)
{
    fd_set pending_data;
    struct timeval block_time;
    char data[NTP_PCK_LEN * 8];
    int packet_len, result, i, re, count = 0;
    socklen_t data_len = addr->ai_addrlen;

    if (!(packet_len = construct_packet(data)))
    {
        printf("construct_packet error!\n");
        return 0;
    }

    // 客户端给服务器端发送NTP协议数据包
    // 数据包格式范例: 1b0004fa000100000001000000000000000000000000000000000000000000000000000000000000dd26aaa74f5022d9
    if ((result = sendto(sk, data, packet_len, 0, addr->ai_addr, data_len)) < 0)
    {
        printf("sendto error!\n");
        return 0;
    }

    // 调用SELECT()函数,并设定超时时间为1s
    FD_ZERO(&pending_data);
    FD_SET(sk, &pending_data);
    block_time.tv_sec = 10;
    block_time.tv_usec = 0;
    if (select(sk + 1, &pending_data, NULL, NULL, &block_time) > 0)
    {
        // 接收服务器端的信息
        if ((count = recvfrom(sk, data, NTP_PCK_LEN * 8, 0, addr->ai_addr, &data_len)) < 0)
        {
            printf("recvfrom error!\n");
            return 0;
        }
        if (protocol == TIME)
        {
            memcpy(&ret_time->transmit_timestamp, data, 4);
            return 1;
        }
        else if (count < NTP_PCK_LEN)
        {
            printf("count error!\n");
            return 0;
        }

        // 设置接收NTP协议包的数据结构
        // 接收包格式范例: 1c0104ec000000000000004847505373dd26aa9ff7e47f4edd26aaa74f5022d9dd26aaa75f6f1524dd26aaa75f716a6a
        ret_time->leap_ver_mode = ntohl(data[0]);
        ret_time->startum = ntohl(data[1]);
        ret_time->poll = ntohl(data[2]);
        ret_time->precision = ntohl(data[3]);
        ret_time->root_delay = ntohl(*(int*)&(data[4]));
        ret_time->root_dispersion = ntohl(*(int*)&(data[8]));
        ret_time->reference_identifier = ntohl(*(int*)&(data[12]));
        ret_time->reference_timestamp.coarse = ntohl(*(int*)&(data[16]));
        ret_time->reference_timestamp.fine = ntohl(*(int*)&(data[20]));
        ret_time->originage_timestamp.coarse = ntohl(*(int*)&(data[24]));
        ret_time->originage_timestamp.fine = ntohl(*(int*)&(data[28]));
        ret_time->receive_timestamp.coarse = ntohl(*(int*)&(data[32]));
        ret_time->receive_timestamp.fine = ntohl(*(int*)&(data[36]));
        ret_time->transmit_timestamp.coarse = ntohl(*(int*)&(data[40]));
        ret_time->transmit_timestamp.fine = ntohl(*(int*)&(data[44]));

        return 1;
    }
    else {
        printf("select error!\n");
    }

    printf("data error!\n");
    return 0;
}

int set_local_time(struct ntp_packet * pnew_time_packet)
{
    struct timeval tv;
    tv.tv_sec = pnew_time_packet->transmit_timestamp.coarse - JAN_1970;
    tv.tv_usec = USEC(pnew_time_packet->transmit_timestamp.fine);
    printf("%ld\n", tv.tv_sec);
    time_t timer = tv.tv_sec;
    struct tm *tblock = localtime(&timer);
    printf("Local time is: %s",asctime(tblock));
    return 0;
    // return settimeofday(&tv, NULL);
}

int main()
{
    int sockfd, rc;
    struct addrinfo hints, *res = NULL;
    struct ntp_packet new_time_packet;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    // 调用getaddrinfo函数,获取地址信息
    rc = getaddrinfo(NTP_SERVER_IP, NTP_PORT_STR, &hints, &res);
    if (rc != 0)
    {
        printf("getaddrinfo error!\n");
        exit(1);
    }

    // 创建套接字
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0)
    {
        printf("sockett error!\n");
        exit(1);
    }

    // 调用取得NTP时间的函数
    if (get_ntp_time(sockfd, res, &new_time_packet))
    {
        // 设置本地时间
        if (!set_local_time(&new_time_packet))
        {
            printf("NTP client success!\n");
        }
        printf("set local time finished!\n");
    }
    printf("get ntp time finished!\n");

    close(sockfd);
    return 0;
}

