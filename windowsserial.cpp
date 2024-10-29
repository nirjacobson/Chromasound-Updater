#include "windowsserial.h"

WindowsSerial::WindowsSerial()
    : _handle(INVALID_HANDLE_VALUE)
{

}

WindowsSerial::~WindowsSerial()
{
    if (_handle != INVALID_HANDLE_VALUE) {
        CloseHandle(_handle);
    }
}

void WindowsSerial::setPort(const QString &port)
{
    if (_handle != INVALID_HANDLE_VALUE) {
        CloseHandle(_handle);
    }

    std::string portStr = port.toStdString();
    std::wstring temp = std::wstring(portStr.begin(), portStr.end());
    LPCWSTR portName = temp.c_str();

    _handle = CreateFile(portName,
                         GENERIC_READ | GENERIC_WRITE,
                         0,
                         0,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         0);

    if (_handle == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            printf("Serial port %s does not exist.\n", port.toStdString().c_str());
        }
        printf("Could not obtain handle for serial port %s.\n", port.toStdString().c_str());
    } else {
        setupPort();
    }
}

void WindowsSerial::writeData(const void *data, size_t size)
{
    DWORD bytesWritten = 0;

    if (!WriteFile(_handle, data, size, &bytesWritten, NULL)) {
        printf("Could not write data from serial port.\n");
    }
}

void WindowsSerial::readData(void *data, size_t size)
{
    DWORD bytesRead = 0;

    if (!ReadFile(_handle, data, size, &bytesRead, NULL)) {
        printf("Could not read data from serial port.\n");
    }
}

void WindowsSerial::setupPort()
{
    DCB serialParams = { 0 };

    serialParams.DCBlength = sizeof(serialParams);

    if (!GetCommState(_handle, &serialParams)) {
        printf("Could not get serial port state.\n");
    }

    serialParams.BaudRate = CBR_57600;
    serialParams.ByteSize = 8;
    serialParams.StopBits = ONESTOPBIT;
    serialParams.Parity = NOPARITY;

    if (!SetCommState(_handle, &serialParams)) {
        printf("Could not set serial port state.\n");
    }

    COMMTIMEOUTS timeouts = { 0 };

    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(_handle, &timeouts)) {
        printf("Could not set serial port timeouts.\n");
    }
}
