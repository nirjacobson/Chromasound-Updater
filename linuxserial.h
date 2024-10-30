#ifndef LINUXSERIAL_H
#define LINUXSERIAL_H

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <QString>
#include <QStringList>
#include <QDir>

#include "serial.h"

class LinuxSerial : public Serial
{
public:
    LinuxSerial();
    ~LinuxSerial();

    void setPort(const QString& port);
    void writeData(const void* data, size_t size);
    void readData(void* data, size_t size);

    QStringList serialPorts();

private:
    int _fd;
    QString _port;

    void setupPort();
};

#endif // LINUXSERIAL_H
