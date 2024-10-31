#ifndef WINDOWSSERIAL_H
#define WINDOWSSERIAL_H

#include <windows.h>
#include <locale>
#include <codecvt>
#include <string>

#include <QStringList>

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

    QStringList serialPorts();

private:
    HANDLE _handle;

    void setupPort();
    bool testPort(int i);
};

#endif // WINDOWSSERIAL_H
