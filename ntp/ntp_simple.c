#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SUB70 0x83aa7e80U       //1900-1970之间的时间差
#define NTP_PORT 123
#define NTP_SERVER "17.253.84.253"

#define uint32 unsigned int
#define uint64 unsigned long long

#define LI 0
#define VN 3
#define MODE 3
#define STRATUM 0
#define POLL 4
#define PREC -6


typedef struct _ntppack
{
    uint32 li_vn;           //LI VN MODE STRATUM POLL PRECISION
    uint32 root_delay;      //本地到主参考时钟源的往返时间
    uint32 root_dispersion;     //系统时钟相对于主参考时钟的最大误差
    uint32 reference_identifier;  //参考时钟源标识

    uint64 reference_time;    //T4  系统时间最后一次被改变
    uint64 originate_time;    //T1  离开发送端时发送端的时间
    uint64 receive_time;      //T2  到达服务器时服务器的时间
    uint64 transmit_time;     //T3  离开服务器时服务器的时间
}NTP;

NTP send_pack,recv_pack;
uint64 firsttime,finaltime;
uint64 delaytime,diftime;

void init(void)
{
    send_pack.li_vn = htonl((LI << 30)|(VN << 27)|(MODE << 24)|(STRATUM << 16)|(POLL << 8)|(PREC & 0xff));
    firsttime = (long long)time(NULL)+SUB70;

    send_pack.reference_time = htonl(firsttime);
}

int main(void)
{
    int sock;
    struct sockaddr_in addr;
    struct timeval tv1;

    if((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        perror("socket");
        exit(1);
    }
    
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(NTP_PORT);
    addr.sin_addr.s_addr = inet_addr(NTP_SERVER);

    init();

    sendto(sock, &send_pack, sizeof(send_pack), 0, (struct sockaddr *)&addr, sizeof(struct sockaddr));

    recv(sock, &recv_pack, sizeof(recv_pack), 0);

    /****转换字节序****/
    recv_pack.root_delay = ntohl(recv_pack.root_delay);
    recv_pack.root_dispersion = ntohl(recv_pack.root_dispersion);
    recv_pack.reference_identifier = ntohl(recv_pack.reference_identifier);
    recv_pack.reference_time = ntohl(recv_pack.reference_time);
    recv_pack.originate_time = ntohl(recv_pack.originate_time);
    recv_pack.receive_time = ntohl(recv_pack.receive_time);
    recv_pack.transmit_time = ntohl(recv_pack.transmit_time);

    diftime = ((recv_pack.receive_time - firsttime) +(recv_pack.transmit_time - finaltime)) >> 1;
    
    delaytime = ((finaltime - firsttime) - (recv_pack.transmit_time - recv_pack.receive_time)) >> 1;
    
    tv1.tv_sec = time(NULL)+diftime+delaytime;
    tv1.tv_usec = 0;

    // if(settimeofday(&tv1, NULL) < 0)
    // {
    //     perror("settimeofday");
    //     exit(1);
    // }

    // system("date");

    printf("%ld\n", tv1.tv_sec);

    return 0;
}
