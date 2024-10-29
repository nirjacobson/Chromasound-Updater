#ifndef LINUXSERIAL_H
#define LINUXSERIAL_H

#include <QString>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "serial.h"

class LinuxSerial : public Serial
{
public:
    LinuxSerial();
    ~LinuxSerial();

    void setPort(const QString& port);
    void writeData(const void* data, size_t size);
    void readData(void* data, size_t size);

private:
    int _fd;
    QString _port;

    void setupPort();
};

#endif // LINUXSERIAL_H
