#include <linux/init.h>
#include <linux/module.h>
#include <asm/string.h>

#include "gpio_drv.h"

struct fs4412_gpio
{
    unsigned int *gpdcon;
    void __iomem *timer_base;
    struct cdev cdev;
};

static struct fs4412_gpio *gpio;

static struct file_operations fs4412_gpio_fops =
{
    /* GPIO 设备的 file_operations 结构定义 */
    .owner = THIS_MODULE,
    .open = fs4412_gpio_open, /* 初始化配置 */
    .release = fs4412_gpio_release, /* 关闭设备 */
    .unlocked_ioctl = fs4412_gpio_ioctl, /* 实现主要控制功能 */
};

void fs4412_led_io_init(void)
{
    writel((readl(gpf3con) & ~(0xff << 16)) | (0x11 << 16), gpf3con);
    writel(readl(gpx2dat) & ~(0x3 << 4), gpf3dat);

    writel((readl(gpx1con) & ~(0xf << 0)) | (0x1 << 0), gpx1con);
    writel(readl(gpx1dat) & ~(0x1 << 0), gpx1dat);

    writel((readl(gpx2con) & ~(0xf << 28)) | (0x1 << 28), gpx2con);
    writel(readl(gpx2dat) & ~(0x1 << 7), gpx2dat);
}

static int fs4412_pwm_io_init(void)
{
    int ret;
    dev_t devno = MKDEV(gpio_major, gpio_minor);

    ret = register_chrdev_region(devno, number_of_device, "gpio");
    if (ret < 0)
    {
        printk("faigpio: register chrdev region\n");
        return ret;
    }

    gpio = kmalloc(sizeof(*gpio), GFP_KERNEL);
    if (gpio == NULL)
    {
        ret = -ENOMEM;
        printk("faigpio: kmalloc\n");
        goto err1;
    }

    memset(gpio, 0, sizeof(*gpio));
    cdev_init(&gpio->cdev, &fs4412_gpio_fops);
    gpio->cdev.owner = THIS_MODULE;
    ret = cdev_add(&gpio->cdev, devno, number_of_device);
    if (ret < 0)
    {
        printk("faigpio: cdev_add\n");
        goto err2;
    }

    gpio->gpdcon = ioremap(GPDCON, 4);
    if (gpio->gpdcon == NULL)
    {
        ret = -ENOMEM;
        printk("faigpio: ioremap gpdcon\n");
        goto err3;
    }

    gpio->timer_base = ioremap(TIMER_BASE, 0x20);
    if (gpio->timer_base == NULL)
    {
        ret = -ENOMEM;
        printk("failed: ioremap tiemr_base\n");
        goto err4;
    }

    return 0;

err4:
    iounmap(gpio->gpdcon);

err3:
    cdev_del(&gpio->cdev);

err2:
    kfree(gpio);

err1:
    unregister_chrdev_region(devno, number_of_device);
}

void fs4412_led_on(int nr)
{
    switch (nr)
    {
        case 1:
        writel(readl(gpx2dat) | (1 << 7), gpx2dat);
        printk("LED 1 \n");
        break;

        case 2:
        writel(readl(gpx1dat) | (1 << 0), gpx1dat);
        printk("LED 2 \n");
        break;

        case 3:
        writel(readl(gpf3dat) | (1 << 4), gpf3dat);
        printk("LED 3 \n");
        break;

        case 4:
        writel(readl(gpf3dat) | (1 << 5), gpf3dat);
        printk("LED 4 \n");
        break;
    }
}

void fs4412_led_off(int nr)
{
    switch (nr)
    {
        case 1:
        writel(readl(gpx2dat) & ~(1 << 7), gpx2dat);
        break;

        case 2:
        writel(readl(gpx1dat) & ~(1 << 0), gpx1dat);
        break;

        case 3:
        writel(readl(gpf3dat) & ~(1 << 4), gpf3dat);
        break;

        case 4:
        writel(readl(gpf3dat) & ~(1 << 5), gpf3dat);
        break;
    }
}

int fs4412_led_ioremap(void)
{
    int ret;

    gpf3con = ioremap(FS4412_GPF3CON, 4);
    if (gpf3con == NULL)
    {
        printk("ioremap gpf3con\n");
        ret = -ENOMEM;
        return ret;
    }

    gpf3dat = ioremap(FS4412_GPF3DAT, 4);
    if (gpf3dat == NULL)
    {
        printk("ioremap gpf3dat\n");
        ret = -ENOMEM;
        return ret;
    }

    gpx1con = ioremap(FS4412_GPF1CON, 4);
    if (gpx1con == NULL)
    {
        printk("ioremap gpx1con\n");
        ret = -ENOMEM;
        return ret;
    }

    gpx1dat = ioremap(FS4412_GPF1DAT, 4);
    if (gpx1dat == NULL)
    {
        printk("ioremap gpx1dat\n");
        ret = -ENOMEM;
        return ret;
    }

    gpx2con = ioremap(FS4412_GPF2CON, 4);
    if (gpx2con == NULL)
    {
        printk("ioremap gpx2con\n");
        ret = -ENOMEM;
        return ret;
    }

    gpx2dat = ioremap(FS4412_GPF2DAT, 4);
    if (gpx2dat == NULL)
    {
        printk("ioremap gpx2dat\n");
        ret = -ENOMEM;
        return ret;
    }
    return 0;
}

int fs4412_led_iounmap(void)
{
    iounmap(gpf3con);
    iounmap(gpf3dat);
    iounmap(gpx1con);
    iounmap(gpx1dat);
    iounmap(gpx2con);
    iounmap(gpx2dat);
}

static int fs4412_gpio_open(struct inode *inode, struct file *file)
{
    writel((readl(gpio->gpdcon) & ~0xf) | 0x2, gpio->gpdcon);
    writel(readl(gpio->timer_base + TCFG0) | 0xff, gpio->timer_base + TCFG0);
    writel((readl(gpio->timer_base + TCFG1) & ~0xf) | 0x2, gpio->timer_base + TCFG1);
    writel(300, gpio->timer_base + TCNTB1);
    writel(150, gpio->tiemr_base + TCMPB1);
    writel((readl(gpio->timer_base + TCON) & ~0x1f) | 0x2, gpio->timer_base + TCON);

    return 0;
}

static long fs4412_gpio_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int data;

    if (_IOC_TYPE(cmd) != 'K')
    {
        return -ENOTTY;
    }
    if (_IOC_TYPE(cmd) > 5)
    {
        return -ENOTTY;
    }
    if (_IOC_DIR(cmd) == _IOC_WRITE)
    {
        if (copy_from_user(&data, (void *)arg, sizeof(data)))
        {
            return -EFAULT;
        }
    }

    switch(cmd)
    {
        case LED_ON:
            fs4412_led_on(data);
            break;
        case LED_OFF:
            fs4412_led_off(data);
            break;

        case PWM_ON:
            writel((readl(gpio->timer_base + TCON) & 0x1f) | 0x9, gpio->timer_base + TCON);
            break;
        case PWM_OFF:
            writel(readl(gpio->timer_base + TCON) & ~0x1f, gpio->timer_base + TCON);
            break;

        case SET_PRE:
            writel(readl(gpio->timer_base + TCON) & ~0x1f, gpio->timer_base + TCON);
            writel((readl(gpio->timer_base + TCFG0) & ~0xff) | (data & 0xff), gpio->timer_base + TCFG0);
            writel((readl(gpio->timer_base + TCON) & ~0x1f) | 0x9, gpio->timer_base + TCON);
            break;
        case SET_CNT:
            writel(data, gpio->timer_base + TCNTB1);
            writel(data >> 1, gpio->timer_base + TCMPB1);
            break;

        default:
            printk("Invalid argument");
            return -EINVAL;
    }
    return 0;
}

static int fs4412_gpio_release(struct inode *inode, struct file *file)
{
    fs4412_led_off(1);
    fs4412_led_off(2);
    fs4412_led_off(3);
    fs4412_led_off(4);
    writel(readl(gpio->timer_base + TCON) & ~0xf, gpio->timer_base + TCON);
    return 0;
}

static int __init fs4412_gpio_init(void)
{
    int ret;
    fs4412_pwm_io_init();

    ret = fs4412_led_ioremap();
    if (ret < 0)
        goto err2;

    fs4412_led_io_init();

    printk("led init\n");

    return 0;

err2:
    cdev_del(&gpio->cdev);
}

static void __exit fs4412_gpio_exit(void)
{
    dev_t devno = MKDEV(gpio_major, gpio_minor);
    fs4412_led_iounmap();

    iounmap(gpio->timer_base);
    iounmap(gpio->gpdcon);
    cdev_del(&gpio->cdev);
    kfree(gpio);
    unregister_chrdev_region(devno, number_of_device);
}

module_init(fs4412_gpio_init);
module_exit(fs4412_gpio_exit);

MODULE_AUTHOR("Srefan");
MODULE_DESCRIPTION("A simple module program");
MODULE_VERSION("1.0.0");