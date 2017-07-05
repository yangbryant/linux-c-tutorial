#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "uart_api.h"

/*
struct termios
{
	unsigned short c_iflag;
	unsigned short c_oflag;
	unsigned short c_cflag;
	unsigned short c_lflag;
	unsigned char  c_line;
	unsigned char  c_cc[NCC];
	speed_t     c_ispeed;
	speed_t     c_ospeed;
}
*/
#define MAX_COM_NUM     1024
int set_com_config(int fd,int baud_rate,int data_bits,char parity,int stop_bits)
{
	struct termios new_cfg,old_cfg;
	int speed;
	if(tcgetattr(fd,&old_cfg) != 0)    /*将原先端口配置信息保存在old_cfg 中,并测试该端口是否可用*/
	{
		perror("tcgetattr");
		return -1;
	}
	new_cfg = old_cfg;
	cfmakeraw(&new_cfg);                  /*将端口设置为原始模式*/
	new_cfg.c_cflag &= ~CSIZE;
	
	switch(baud_rate)
	{
		case 2400:
			speed = B2400;
		break;
		
		case 9600:
			speed = B9600;
		break;
		
		case 115200:
			speed = B115200;
		break;
	}
	
	cfsetispeed(&new_cfg,speed);        /*设置输入波特率 */	
	cfsetospeed(&new_cfg,speed);        /*设置输出波涛率*/
	
	switch(data_bits)                   /*设置数据位*/
	{
		case 7:
			new_cfg.c_cflag |= CS7;
		break;
		
		case 8:
			new_cfg.c_cflag |= CS8;
		break;

	}
	
	switch(parity)                     /*设置校验位*/
	{
		default:
		case 'n':
		case 'N':
			new_cfg.c_cflag &= ~PARENB;
			new_cfg.c_iflag &= ~INPCK;
		break;
		
		case 'o':
		case 'O':
			new_cfg.c_cflag |= (PARODD|PARENB);
			new_cfg.c_iflag |= INPCK;
		break;

		case 'e':
		case 'E':
			new_cfg.c_cflag |= PARENB;
			new_cfg.c_cflag &= ~PARODD;
			new_cfg.c_iflag |= INPCK;
		break;

		case 's':
		case 'S':
			new_cfg.c_cflag &= ~PARENB;
			new_cfg.c_cflag &= ~CSTOPB;
		break;


	}
	
	new_cfg.c_cc[VTIME] = 0;                  /* 设置至少字符和等待时间*/
	new_cfg.c_cc[VMIN] = 0;
	tcflush(fd,TCIFLUSH);                     /*清除串口缓存*/
	if((tcsetattr(fd,TCSANOW,&new_cfg)) != 0)/*激活配置*/
	{
		perror("tcsetattr");
		return -1;
	}
	return 0;
}
	
			
int open_port(int com_port)
{
	int fd;
/*
#if(COM_TYPE == GNR_COM)
	char *dev[] = {"/dev/ttyS0","/dev/ttyS1","/dev/ttyS"};
#else
	char *dev[] = {"/dev/ttyUSB0","/dev/ttyUSB1","/dev/ttyUSB2"};
#endif
*/
	char *dev[] = {"/dev/ttyUSB0","/dev/ttyUSB1","/dev/ttyUSB2"};/* 定义USB端口*/

	if((com_port <0)||(com_port>MAX_COM_NUM))
	{
		return -1;
	}

	fd = open(dev[com_port - 1],O_RDWR|O_NOCTTY|O_NDELAY); /* 打开USB端口，以读写方式打开    */
	
	if(fd < 0)
	{
		perror("open serial port");
		return (-1);
	}
	
	if(fcntl(fd,F_SETFL,0)<0)                                 /* 设置fd文件标识符 锁 */
	{
		perror("fcntl F_SETFL\n");
	}
	
	if(isatty(fd) == 0 )                                      /*用于验证端口是否正确打开 */
	{
		perror("This is not a terminal device");
	}
	return fd;
}



























