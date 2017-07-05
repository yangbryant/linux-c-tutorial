#ifndef _UART_API_H_
#define  _UART_API_H_

int set_com_config(int fd,int baud_rate,int data_bits,char parity,int stop_bits);

int open_port(int com_port);



#endif
