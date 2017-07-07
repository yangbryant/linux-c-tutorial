#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "pwm_music.h"
#include "gpio_drv.h"

int main()
{
    int num = 1;
    int i = 0;
    int n = 2;
    int beep_fd;
    int div;
    int pre = 255;
    beep_fd = open("/dev/gpio", O_RDWR | O_NONBLOCK);
    if (beep_fd == -1)
    {
        perror("open");
        exit(1);
    }
    while (1)
    {
        ioctl(beep_fd, PWM_ON);
        ioctl(beep_fd, SET_PRE, &pre);

        for (i = 0; i < sizeof(MumIsTheBestInTheWorld) / sizeof(Note); i++)
        {
            ioctl(beep_fd, LED_ON, &num);
            usleep(40000);
            ioctl(beep_fd, LED_OFF, &num);
            usleep(40000);
            if(++num == 5)
                num = 1;

            div = (PCLK/256/4)/(MumIsTheBestInTheWorld[i].pitch);
            ioctl(beep_fd, SET_CNT, &div);
            usleep(MumIsTheBestInTheWorld[i].dimation * 50);
        }

        for(i = 0; i < sizeof(GreatlyLongNow)/sizeof(Note); i++)
        {
            ioctl(beep_fd, LED_ON, &num);
            usleep(40000);
            ioctl(beep_fd, LED_OFF, &num);
            usleep(40000);
            if (++num == 5)
                num = 1;

            div = (PCLK/256/4)/(GreatlyLongNow[i].pitch);
            ioctl(beep_fd, SET_CNT, &div);
            usleep(GreatlyLongNow[i].dimation * 50);
        }

        for (i = 0; i < sizeof(FishBoat)/sizeof(Note); ++i)
        {
            ioctl(beep_fd, LED_ON, &num);
            usleep(40000);
            ioctl(beep_fd, LED_OFF, &num);
            usleep(40000);
            if (++num == 5)
                num = 1;

            div = (PCLK/256/4)/(FishBoat[i].pitch);
            ioctl(beep_fd, SET_CNT, &div);
            usleep(FishBoat[i].dimation * 50);
        }
    }
    return 0;
}