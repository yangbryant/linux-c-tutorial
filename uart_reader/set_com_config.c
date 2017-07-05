#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


#define MAX_COM_NUM 10

int open_port(int com_port)
{
    int fd;
    char *dev[] = {"/dev/cu.usbserial", "/dev/tty.usbserial"};
    if ((com_port < 0) || (com_port > MAX_COM_NUM))
    {
        return -1;
    }

    fd = open(dev[com_port], O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0)
    {
        printf("open serial port error.\n");
        return -1;
    }

    if (fcntl(fd, F_SETFL, 0) < 0)
    {
        printf("fcntl F_SETTFL\n");
    }

    if (isatty(fd) == 0)
    {
        printf("This is not a terminal device.\n");
    }
    return fd;
}

int set_com_config(int fd, int baud_rate, int data_bits, char parity, int stop_bits)
{
    struct termios new_cfg, old_cfg;
    int speed;

    if (tcgetattr(fd, &old_cfg) != 0) {
        printf("tcgetattr error.\n");
        return -1;
    }

    new_cfg = old_cfg;
    cfmakeraw(&new_cfg);

    new_cfg.c_cflag &= ~CSIZE;

    switch(baud_rate)
    {
        case 2400:
            speed = B2400;
            break;

        case 4800:
            speed = B4800;
            break;

        case 9600:
            speed = B9600;
            break;

        case 19200:
            speed = B19200;
            break;

        case 38400:
            speed = B38400;
            break;

        case 115200:
        default:
            speed = B115200;
            break;
    }

    cfsetispeed(&new_cfg, speed);
    cfsetospeed(&new_cfg, speed);

    switch(data_bits)
    {
        case 7:
            new_cfg.c_cflag |= CS7;
            break;

        case 8:
        default:
            new_cfg.c_cflag |= CS8;
            break;
    }

    switch(parity)
    {
        case 'o':
        case 'O':
            new_cfg.c_cflag |= (PARODD | PARENB);
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

        case 'N':
        case 'n':
        default:
            new_cfg.c_cflag &= ~PARENB;
            new_cfg.c_iflag &= ~INPCK;
            break;
    }

    switch(stop_bits)
    {
        case 1:
        default:
            new_cfg.c_cflag &= ~CSTOPB;
            break;

        case 2:
            new_cfg.c_cflag |= CSTOPB;
            break;
    }

    new_cfg.c_cc[VTIME] = 0;
    new_cfg.c_cc[VMIN] = 1;

    tcflush(fd, TCIFLUSH);
    if ((tcsetattr(fd, TCSANOW, &new_cfg)) != 0)
    {
        printf("tcsetattr\n");
        return -1;
    }

    return 0;
}