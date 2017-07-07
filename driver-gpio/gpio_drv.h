#ifndef __S5PC100_LED_H__
#define __S5PC100_LED_H__

#define PWM_ON _IO('K', 0)
#define PWM_OFF _IO('K', 1)

#define SET_PRE _IOW('K', 2, int)
#define SET_CNT _IOW('K', 3, int)

#define LED_ON _IOW('K', 4, int)
#define LED_OFF _IOW('K', 5, int)

#define TCFG0 0x00
#define TCFG1 0x04

#define TCON 0x08

#define TCNTB1 0x0C
#define TCMPB1 0x10

#define GPDCON 0x114000A0
#define TIMER_BASE 0x139D0000

#define FS4412_GPF3CON 0x114001E0
#define FS4412_GPF3DAT 0x114001E4

#define FS4412_GPX1CON 0x11000C20
#define FS4412_GPX1DAT 0x11000C24

#define FS4412_GPX2CON 0x11000C40
#define FS4412_GPX2DAT 0x11000C44

#endif