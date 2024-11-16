#include "gpio.h"

GPIO::GPIO()
    : _fd(-1)
{
    if (access(GPIO_DEV_NAME_PI5, F_OK) != -1) {
        _fd = open(GPIO_DEV_NAME_PI5, O_RDONLY);
        if (_fd < 0) {
            printf("Unable to open %s: %s", GPIO_DEV_NAME_PI5, strerror(errno));
        }
    } else {
        _fd = open(GPIO_DEV_NAME, O_RDONLY);
        if (_fd < 0) {
            printf("Unable to open %s: %s", GPIO_DEV_NAME, strerror(errno));
        }
    }
}

GPIO::~GPIO()
{
    if (_fd >= 0) {
        close(_fd);
    }
}

void GPIO::write(int pin, int value)
{
    int ret;

    struct gpiohandle_request rq;
    rq.lineoffsets[0] = pin;
    rq.lines = 1;
    rq.flags = GPIOHANDLE_REQUEST_OUTPUT;

    ret = ioctl(_fd, GPIO_GET_LINEHANDLE_IOCTL, &rq);

    if (ret == -1) {
        printf("Unable to get line handle from ioctl: %s", strerror(errno));
        return;
    }

    struct gpiohandle_data data;
    data.values[0] = value;
    ret = ioctl(rq.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);

    if (ret == -1) {
        printf("Unable to set line value using ioctl: %s", strerror(errno));
    }

    close(rq.fd);
}
