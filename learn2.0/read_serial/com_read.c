/**************************************************************
**************************************************************
*功能：
*  波特率：115200;
*    通过串口读取数据
*  
*
***************************************************************
**************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "uart_api.h"
#include <unistd.h>


#define BUFFER_SIZE    1024
#define HOST_COM_PORE  1             /*第一个USB设备*/


int main(void)
{
	int fd;
	char buff[BUFFER_SIZE];
	printf("*************receive data*****************\n");
	if((fd = open_port(HOST_COM_PORE))<0)               /*打开USB端口*/
	{
		perror("open_port");                       /*如果出错，则打印出错内容*/
		return 1;
	}
	if(set_com_config(fd,115200,8,'N',1) < 0)          /*设置串口通信波特率等配置信息*/
	{
		perror("set_com_config");
		return 1;
	}
	do                                                 /*设置成功，开始输入字符*/
	{
		memset(buff,0,BUFFER_SIZE);                /* 将缓存buff 清除*/
		if(read(fd,buff,BUFFER_SIZE)>0)
		{
			printf("The received words are : %s\n",buff);
		}
	}while(strncmp(buff,"quit",4));                     /*  检测buff缓存中的数据为“ quit” 则退出编辑,strncmp的功能为比较两个字符串是否一致*/
	close(fd);
	return 0;

}
