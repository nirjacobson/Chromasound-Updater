#ifndef GPIO_H
#define GPIO_H

#include <QProcess>

#include <linux/gpio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define GPIO_DEV_NAME     "/dev/gpiochip0"
#define GPIO_DEV_NAME_PI5 "/dev/gpiochip4"

class GPIO {
public:
    GPIO();
    ~GPIO();

    void write(int pin, int value);

private:
    int _fd;
};

#endif // GPIO_H
