#ifndef WINDOWSSERIAL_H
#define WINDOWSSERIAL_H

#include <windows.h>

#include "serial.h"

class WindowsSerial : public Serial
{
public:
    WindowsSerial();
    ~WindowsSerial();

    // Serial interface
public:
    void setPort(const QString &port);
    void writeData(const void *data, size_t size);
    void readData(void *data, size_t size);

private:
    HANDLE _handle;

    void setupPort();
};

#endif // WINDOWSSERIAL_H
