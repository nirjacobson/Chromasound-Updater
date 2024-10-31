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
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wide = converter.from_bytes(portStr.c_str());
    LPCWSTR portName = wide.c_str();

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

    while (bytesWritten < size) {
        DWORD temp = 0;
        if (!WriteFile(_handle, (const char*)data + bytesWritten, size - bytesWritten, &temp, NULL)) {
            printf("Could not write data from serial port.\n");
        }
        bytesWritten += temp;
    }
}

void WindowsSerial::readData(void *data, size_t size)
{
    DWORD bytesRead = 0;

    while (bytesRead < size) {
        DWORD temp = 0;
        if (!ReadFile(_handle, (char*)data + bytesRead, size - bytesRead, &temp, NULL)) {
            printf("Could not read data from serial port.\n");
        }
        bytesRead += temp;
    }
}

QStringList WindowsSerial::serialPorts()
{
    QStringList ports;

    TCHAR lpTargetPath[1000]; // buffer to store the path of the COMPORTS
    DWORD test;

    for(int i=0; i<255; i++) // checking ports from COM0 to COM255
    {
        QString commName = QString("COM%1").arg(i);

        if (testPort(i)) {
            ports.append(commName);
        }
    }

    return ports;
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

bool WindowsSerial::testPort(int i)
{
    TCHAR lpTargetPath[1000]; // buffer to store the path of the COMPORTS

    std::string commName = "COM" + std::to_string(i);
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wide = converter.from_bytes(commName.c_str());
    return QueryDosDeviceW((LPCWSTR)wide.c_str(), (LPWSTR)lpTargetPath, 1000);
}
