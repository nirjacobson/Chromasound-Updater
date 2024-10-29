#ifndef SERIAL_H
#define SERIAL_H

#include <QString>

class Serial
{
public:
    Serial();

    virtual void setPort(const QString& port) = 0;
    virtual void writeData(const void* data, size_t size) = 0;
    virtual void readData(void* data, size_t size) = 0;
};

#endif // SERIAL_H
