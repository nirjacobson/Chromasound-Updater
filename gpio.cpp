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

    struct gpio_v2_line_request rq;
    memset(&rq, 0, sizeof(rq));

    rq.offsets[0] = pin;
    rq.num_lines = 1;
    rq.config.flags = GPIO_V2_LINE_FLAG_OUTPUT;

    ret = ioctl(_fd, GPIO_V2_GET_LINE_IOCTL, &rq);

    if (ret == -1) {
        printf("Unable to get line handle from ioctl: %s", strerror(errno));
        return;
    }

    struct gpio_v2_line_values data;
    data.bits = value;
    data.mask = 1;
    ret = ioctl(rq.fd, GPIO_V2_LINE_SET_VALUES_IOCTL, &data);

    if (ret == -1) {
        printf("Unable to set line value using ioctl: %s", strerror(errno));
    }

    close(rq.fd);
}
