#ifndef POSIXSERIAL_H
#define POSIXSERIAL_H

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <QString>
#include <QStringList>
#include <QDir>

#include "serial.h"

class POSIXSerial : public Serial
{
public:
    POSIXSerial();
    ~POSIXSerial();

    void setPort(const QString& port);
    void writeData(const void* data, size_t size);
    void readData(void* data, size_t size);

    QStringList serialPorts();

private:
    int _fd;
    QString _port;

    void setupPort();
};

#endif // POSIXSERIAL_H
